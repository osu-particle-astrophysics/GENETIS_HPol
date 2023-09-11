#*******************************************************************************
# File: part_b_job1.py
#    Based on Part_B_VPol_job1.sh, this is an attempt to migrate to Python.
#
#  Programmer: Jason Yao (yao.966@osu.edu)
#
#  Revision history:
#     07/26/23  Jason Yao, Original version
#
#  Notes:
#       * vertical ruler at column 80
#  TODO:
#       1. check if subprocess runs xfdtd and submits the job-script properly.
#          (XF SECTION)
#
#*******************************************************************************
'''
This is Part B1 of the loop, which prepares and runs simulation_PEC.xmacro with
information such as the parameters of the antennas. The xmacro is mainly made of
two xmacro-skeleton text files (see CAT SECTION)

Usage: python3 part_b_job1.py <indiv> <gen> <npop> <working_dir> ... <nsections>

Outout: XmacrosDir/simulation_PEC.xmacro

For more information on the arguments: 
  >> python3 part_b_job1.py -h
'''

import argparse
import os
import subprocess as sp

from pathlib import Path


def dummy(kind=None):
    """
    This is a dummy function

    :param kind: Optional "kind" of ingredients.
    :type kind: list[str] or None
    :return: none
    """
    print("Hi I am a dummy function")



def main(indiv, gen, npop, working_dir, run_name, xmacros_dir, xf_proj, 
         geo_factor, num_keys, curved, nsections):

## DIRECTORY CHECK    

    # In case we re-run the same generation, check the existence of
    # subdirectories under Simulations located inside $XFProj.

# DELETE ME DELETE ME DELETE ME DELETE ME DELETE ME DELETE ME 
    # Note: XFdtd makes these; I am faking
    (xf_proj / 'Simulations').mkdir(parents=True)
# DELETE ME DELETE ME DELETE ME DELETE ME DELETE ME DELETE ME 

    for i in range(1, npop+1):
        # For the 1st generation, this loop checks antenna 1 to antenna npop;
        # for the 2nd generation, it checks antenna npop+1 to antenna npop+npop
        # (ie. 1*npop+1 to 1*npop+npop), and so on and so forth until we check a
        # total number of [(gen+1) * npop] antennas.
        # (recall that gen is the INDEX of generation, which starts from 0)
        index = gen*npop + i

        # target directories have six digits, eg. 000912, in which case 
        # "index" above would be 912.
        target = xf_proj / 'Simulations' / f'{index:06d}'
    
# DELETE ME DELETE ME DELETE ME DELETE ME DELETE ME DELETE ME 
        os.mkdir(target) # XFdtd akes these; I am faking
# DELETE ME DELETE ME DELETE ME DELETE ME DELETE ME DELETE ME 

        if os.path.exists(target):
            os.rmdir(target)    # remove target directory if it exists

# NECESSARY ? NECESSARY ? NECESSARY ? NECESSARY ? NECESSARY ? NECESSARY ? 
    if gen != 0:
        with open (xf_proj / 'Simulations' / '.nextSimulationNumber', "w") as f:
            f.write( str(gen*npop + 1) )
# NECESSARY ? NECESSARY ? NECESSARY ? NECESSARY ? NECESSARY ? NECESSARY ? 


## INITIALIZE XMACRO

    # list of 60 frequencies used in XFdtd VPol simulations.
    freq_list = [83.33, 100.00, 116.67, 133.33, 150.00, 166.67, 183.34, 200.00,
        216.67, 233.34, 250.00, 266.67, 283.34, 300.00, 316.67, 333.34, 350.00,
        366.67, 383.34, 400.01, 416.67, 433.34, 450.01, 466.67, 483.34, 500.01,
        516.68, 533.34, 550.01, 566.68, 583.34, 600.01, 616.68, 633.34, 650.01,
        666.68, 683.35, 700.01, 716.68, 733.35, 750.01, 766.68, 783.35, 800.01,
        816.68, 833.35, 850.02, 866.68, 883.35, 900.02, 916.68, 933.35, 950.02,
        966.68, 983.35, 1000.00, 1016.70, 1033.40, 1050.00, 1066.70] 

    sim_path = xmacros_dir / 'simulation_PEC.xmacro'
    
    # Begin writing some run-specific variables to the xmacro
    with open (sim_path, "w") as f:
        f.write(f'var NPOP = {npop};\n'
                f'var indiv = {indiv};\n'
                f'//Factor of {geo_factor} frequency\n'
                'var freq = [')
        f.write(', '.join( f'{freq*geo_factor:.2f}' for freq in freq_list) )
        f.write('] \n')    # using f-string padding: 2 digits after decimal pt.
    
    if gen == 0 and indiv == 1:
        with open (sim_path, "a") as f:
            f.write('if(indiv==1){\n'
                    f'App.saveCurrentProjectAs(\"{working_dir}/Run_Outputs/'
                    f'{run_name}/{run_name}\");\n'
                    '}\n')

    sim_path.chmod(0o775)      # This is the same as Bash chmod 775 ${sim_path}.


## CAT SECTION

    # The rest of simulation_PEC.xmacro is built from two skeleton text files.
    if curved == 0:                            
        if nsections == 1:                     # straight-side symmetric bicone
            fst = 'simulationPECmacroskeleton_GPU.txt'  # first skeleton
            snd = 'simulationPECmacroskeleton2_GPU.txt' # second skeleton

        else:                                  # straight-side asymmetric bicone
            fst = 'simulationPECmacroskeleton_Sep.txt'
            snd = 'simulationPECmacroskeleton2_Sep.txt'
    else:                                      # curved-side bicone
            fst = 'simulationPECmacroskeleton_curved.txt'
            snd = 'simulationPECmacroskeleton2_curved.txt'

    with open(sim_path, 'a') as outfile:
        outfile.write((xmacros_dir / fst).read_text())
        outfile.write((xmacros_dir / snd).read_text())


## SED-TION (Ha! Get it? Like...section?)

    # sed swaps out the hardcoded word "fileDirectory" inside the xmacro.
    # option "i" of sed: swap "in-place" instead of outputting to terminal.
    # The empty quotes after sed -i are for MacOS compatibility:
    # https://stackoverflow.com/questions/12272065/sed-undefined-label-on-macos
    sp.run(f'sed -i "" "s|fileDirectory|{working_dir}/Generation_Data|" '
           f'{sim_path}', shell=True)

    # Change gridsize by the same factor used for changing antenna size.
    xmacro_skeleton_default_gridsize = 0.1 # padding: 6 digits after decimal pt.
    grid_size = f'{xmacro_skeleton_default_gridsize/geo_factor:.6f}'

    sp.run(f'sed -i "" "s/var gridSize = 0.1;/var gridSize = {grid_size};/" '
           f'{sim_path}', shell=True)


## XF SECTION

    xmacros_dir.chmod(0o775)

    # load the XFdtd module on OSC and load simulation_PEC.xmacro into XFdtd
    p1 = sp.run(f'module load xfdtd/7.9.2.2\n'
                f'xfdtd {xf_proj} --execute-macro-script={sim_path}', 
                shell=True, capture_output=True) 

    if p1.stderr: # print out the error message
        print('\nError at part_b_job1.py XF SECTION while loading xmacro\n'+
              p1.stderr.decode())

    # Determine the number of simultaneously running jobs in the SLURM job-array
    if npop <= num_keys:
        batch_size = npop       # quite unlikely, since we only have 5 keys
    else:
        batch_size = num_keys


## SIMULATION JOB SUBMISSION
    # Submit a job-array to OSC; each job in the array is responsible for
    # simulating one antenna in the current generation. Make the SLURM job-name
    # the same as run_name so that we can use SLURM variables.
    try:
        sp.call(['sbatch', f'--array=1{npop}%{batch_size}',
                f'--job-name={run_name}',
                f'--export=ALL,WorkingDir={working_dir},RunName={run_name},'
                f'XFProj={xf_proj},NPOP={npop},gen={gen}',
                f'{working_dir}/Batch_Jobs/GPU_XF_Job.sh'],
                capture_output=True)
    except:
        print('\nError at part_b_job1.py during job submission\n')

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
