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
#       * MOVE SECTION is not great; clean this up
#
#*******************************************************************************
'''
This is Part B2 of the loop, which prepares and runs output.xmacro with the
relevant parameters (antenna type, population number, grid size, etc.)
Output.xmacro writes the XFdtd simulation data to output files (.uan)
The xmacro is mainly made of two xmacro-skeleton text files (see CAT SECTION)

Usage: python3 part_b_job1.py <indiv> <gen> <npop> <working_dir> ... <nsections>

Outout: XmacrosDir/simulation_PEC.xmacro

For more information on the arguments: 
  >> python3 part_b_job1.py -h
'''

import argparse
import os
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
    flagpole = f'{working_dir}/Run_Outputs/{run_name}/GPUFlags'
    num_flags = count_flags(flagpole)

    while num_flags < npop:
        sleep(20)
        num_flags = count_flags(flagpole)
    
    # once all XF simulations for the same generation are done, remove all flags
    sp.run(f'rm {flagpole}/*', shell=True) # shell=True for wildcard ("*")


    outmacro = xmacros_dir / 'output.xmacro'
    # If output.xmacro exists, empty it (without changing permission)
    sp.run(f'[ -f {outmacro} ] && > {outmacro}', shell=True)

    # Begin writing some run-specific variables to the xmacro
    with open (f'{xmacros_dir}/output.xmacro', "w+") as f:
        f.write(f'var NPOP = {npop};\n'
                f'for (var k = {gen*npop + 1}; k <= {gen*npop+npop}; k++){{\n')
    
    os.umask(0)                # umask(0) needed to ensure chmod works properly.
    os.chmod(outmacro, 0o775 ) # This is the same as Bash chmod 775 ${sim_path}.

## CAT SECTION
    # The rest of output.xmacro is built from two skeleton text files.
    if nsections == 1:
        p1 = sp.run(f'cat {xmacros_dir}/shortened_outputmacroskeleton.txt '
                    f'>> {outmacro}', shell=True, capture_output=True)
        if p1.stderr:
            print('Error at part_b_job2.py: failed to cat the skeleton')

    else: 
        p1 = sp.run(f'cat {xmacros_dir}/shortened_outputmacroskeleton_Asym.txt '
                    f'>> {outmacro}', shell=True, capture_output=True)
        if p1.stderr:
            print('Error at part_b_job2.py: failed to cat the skeleton')


## SED-TION

    # sed swaps out the hardcoded word "fileDirectory" inside the xmacro.
    # option "i" of sed: swap "in-place" instead of outputting to terminal.
    # The empty quotes after sed -i are for MacOS compatibility:
    # https://stackoverflow.com/questions/12272065/sed-undefined-label-on-macos
    sp.run(f'sed -i "" "s|fileDirectory|{working_dir}|" {outmacro}', shell=True)


## XF SECTION

    os.chmod(xmacros_dir, 0o775)
    # load xfdtd on OSC
    p0 = sp.run(f'module load xfdtd/7.9.2.2', shell=True, capture_output=True)
    if p0.returncode: # print out the error message
        print('\nError at part_b_job2.py: XF SECTION while loading module\n'+
              p0.stderr.decode())

    # load output.xmacro
    p1 = sp.run(f'xfdtd {xf_proj} --execute-macro-script={outmacro}', 
                shell=True, capture_output=True) 
    if p1.returncode: # print out the error message
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

            desti  = (f'{working_dir}/Run_Outputs/'
                      f'{run_name}/uan_files/{gen}_uan_files/{pop_ind_num}/')
            nation = (f'{gen}_{pop_ind_num}_{freq}.uan')

            p2 = sp.run(f'mkdir -p {desti};',
                        shell=True, capture_output=True)
            if p2.returncode:
                print('\nError at the end of part_b_job2.py when mkdir\n'+
                      p2.stderr.decode())

            p3 = sp.run(f'mv {working_dir}/Antenna_Performance_Metric/'
                        f'{gen}_{pop_ind_num}_{freq}.uan {desti}{nation}',
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
