#*******************************************************************************
# File: part_b_job2.py
#    Based on Part_B_VPol_job2.sh
#
#  Programmer: Jason Yao (yao.966@osu.edu)
#
#  Revision history:
#     07/27/23  Jason Yao, Original version
#
#  Notes:
#       * vertical ruler at column 80
#  TODO:
#       1 MOVE SECTION is not great; clean this up
#
#*******************************************************************************
'''
This is Part B2 of the loop, which prepares and runs output.xmacro with the
relevant parameters (antenna type, population number, grid size, etc.)
Output.xmacro writes the XFdtd simulation data to output files (.uan)
The xmacro is mainly made of a xmacro-skeleton text file (see CAT SECTION).

Usage: python3 part_b_job2.py <indiv> <gen> <npop> <working_dir> ... <nsections>

Outout: XmacrosDir/output.xmacro

For more information on the arguments: 
  >> python3 part_b_job2.py -h
'''


import argparse
import subprocess as sp

from pathlib import Path
from time import sleep


'''
This function counts the number of files that don't start with "." (hidden) 
inside a folder "where". Subrirectories are not counted.
Input: where (Path object)
Output: counts (int)
'''
def count_flags(where=Path):
    dummy = sp.run(f'cd {where}; ls | wc -l',
                   shell=True, capture_output=True, text=True)
    return int(dummy.stdout)


def main(indiv, gen, npop, working_dir, run_name, xmacros_dir, xf_proj, 
         geo_factor, num_keys, curved, nsections):
## FLAG COUNT    

    # Count the number of flags that indicate succesful simulations
    flagpole = working_dir / f'Run_Outputs' / f'{run_name}/GPUFlags'
    num_flags = count_flags(flagpole)
    # Sleep for a bit before counting again. Count until all jobs are finished.
    while num_flags < npop:
        sleep(60)
        num_flags = count_flags(flagpole)
    
    # Upon XF job completion for all individuals, remove all flags (to prepare
    # the directory for counting again for the next generation)
    sp.run(f'rm {flagpole}/*', shell=True) # shell=True for wildcard ("*")


## INITIALIZE XMACRO

    outmacro = xmacros_dir / 'output.xmacro'

    # Begin writing some run-specific variables to the xmacro
    with open (outmacro, "w") as f:
        f.write(f'var NPOP = {npop};\n'
                f'for (var k = {gen*npop + 1}; k <= {gen*npop+npop}; k++){{\n')
    
    outmacro.chmod(0o775 ) # This is the same as Bash chmod 775 ${sim_path}.


## CAT SECTION

    # The rest of output.xmacro is built from two skeleton text files.
    if nsections == 1:
        skeleton = 'shortened_outputmacroskeleton.txt'
    else: 
        skeleton = 'shortened_outputmacroskeleton_Asym.txt'
    with open(outmacro, 'a') as outfile:
        outfile.write((xmacros_dir / skeleton).read_text())


## SED-TION

    # sed swaps out the hardcoded word "fileDirectory" inside the xmacro.
    # option "i" of sed: swap "in-place" instead of outputting to terminal.
    # The empty quotes after sed -i are for MacOS compatibility:
    # https://stackoverflow.com/questions/12272065/sed-undefined-label-on-macos
    sp.run(f'sed -i "" "s|fileDirectory|{working_dir}|" {outmacro}', shell=True)


## XF SECTION

    xmacros_dir.chmod(0o775)
    # load xfdtd on OSC and load output.xmacro into XFdtd
    p1 = sp.run(f'module load xfdtd/7.9.2.2\n'
                f'xfdtd {xf_proj} --execute-macro-script={outmacro}', 
                shell=True, capture_output=True) 

    if p1.stderr: # print out the error message
        print('\nError at part_b_job2.py XF SECTION while loading xmacro\n'+
              p1.stderr.decode())


## MOVE SECTION

    # Move all .uan files from Antenna_Performance_Metric to Run_Outputs
    # Sort these files based on the generation index and individual index
    # eg. Run_Outputs/ run_name / uan_files / ?_uan_files / # / ?_#_freq.uan
    #     where ? is the generation, # is the individual inside that generation,
    #     and freq goes from 1 to 60 (60 different frequencies)
    for i in range(gen*npop + indiv, gen*npop + npop +1):
        pop_ind_num = i - gen*npop

        for freq in range(1,61):

            desti  = (working_dir / 'Run_Outputs' / f'{run_name}' /
                      'uan_files' / f'{gen}_uan_files' / f'{pop_ind_num}')
            nation = (f'{gen}_{pop_ind_num}_{freq}.uan')

            p2 = sp.run(f'mkdir -p {desti};',
                        shell=True, capture_output=True)
            if p2.returncode:
                print('\nError at the end of part_b_job2.py when mkdir\n'+
                      p2.stderr.decode())

            p3 = sp.run(f'mv {working_dir}/Antenna_Performance_Metric/'
                        f'{gen}_{pop_ind_num}_{freq}.uan {desti}/{nation}',
                        shell=True, capture_output=True)
            if p3.returncode:
                print('\nError at the end of part_b_job2.py when moving uans\n'+
                      p3.stderr.decode())


### END OF MAIN



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("indiv", type=int,
                        help="index of individual (starts from 1)")
    parser.add_argument("gen", type=int,
                        help="index of gen (starts from 0)")
    parser.add_argument("npop", type=int,
                        help="number of individuals per generation (NPOP)")
    parser.add_argument("working_dir", type=Path,
                        help="working directory")
    parser.add_argument("run_name", type=str,
                        help="run name")
    parser.add_argument("xmacros_dir", type=Path,
                        help="location of all the xmacros")
    parser.add_argument("xf_proj", type=Path,
                        help="xf project directory (has a dot in it: run_name.xf)")
    parser.add_argument("geo_factor", type=float,
                        help="factor to scale down antenna size")
    parser.add_argument("num_keys", type=int,
                        help="number of XFdtd licenses")
    parser.add_argument("curved", type=int,
                        help="curved switch (0 for straight side bicones)")
    parser.add_argument("nsections", type=int,
                        help="symmetry switch (1 for symmetric bicone, 2 asym.)")
    args = parser.parse_args()

    main(args.indiv, args.gen, args.npop, args.working_dir, args.run_name,
         args.xmacros_dir, args.xf_proj, args.geo_factor, args.num_keys,
         args.curved, args.nsections)
