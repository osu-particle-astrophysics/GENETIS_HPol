#*******************************************************************************
# File: part_d_job1.py
#    Based on Part_D1_Array.sh.
#
#  Programmer: Jason Yao (yao.966@osu.edu)
#
#  Revision history:
#     07/28/23 Jason Yao, Original version
#
#  Notes:
#       * vertical ruler at column 80
#  TODO:
#       * remove .root files
#
#*******************************************************************************
'''
This is Part D1 of the Loop.
This script moves each .dat file (from Part C) into a folder AraSim can access,
while changing it to a .txt, which is what AraSim reads. For each individual,
the script then runs AraSim and moves the output into Antenna_Performance_Metric.
Finally, this script makes a directory for all errors and output files from
AraSim to be dumped.

Usage: python4 part_d_job1.py <indiv> <gen> <npop> <working_dir> ... <nsections>

For more information on the arguments: 
  >> python3 part_d_job1.py -h
'''


import argparse
import subprocess as sp

from pathlib import Path


def main(gen, npop, working_dir, arasim_exec,
         exp, nnt, run_name, seeds, debug_mode):
    
## AraSim SETUP
    specific_seed = 32000
    
    if not debug_mode:
        # replace num_nnu (number of neutrinots thrown) in setup_dummy.txt
        p2 = sp.run(f'sed -e "s/num_nnu/{nnt}/" -e "s/n_exp/{exp}/" '
                    f'-e "s/current_seed/{specific_seed}/" '
                    f'{arasim_exec}/setup_dummy_araseed.txt '
                    f'>> {arasim_exec}/setup.txt',
                    capture_output=True, shell=True, text=True)
        if p2.stderr:
            print(f'Error at part d job1:\n {p2.stderr}')
    

## RUN AraSim
        num_jobs = npop * seeds
        max_jobs = 252 # maximum number of simultaneous jobs

        # run AraSim from AraSim/setup.txt for each individual using a job array
        try:
            sp.run(['sbatch',
                   f'--job-name={run_name}',
                   f'--export=ALL,gen={gen},WorkingDir={working_dir},'
                   f'RunName={run_name},Seeds={seeds},'
                   f'AraSimDir={arasim_exec}',
                   f'--array=1-{num_jobs}%{max_jobs}',
                   f'{working_dir}/Batch_Jobs/AraSimCall_Array.sh'],
                   capture_output=True)
                    
        except:
            print('Error at part d1 submission')
        
    else:
        print("debug mode under construction")


### END OF MAIN
        


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("gen", type=int,
                        help="index of generation (starts from 0)")
    parser.add_argument("npop", type=int,
                        help="number of individuals per generation (NPOP)")
    parser.add_argument("working_dir", type=Path,
                        help="working directory")
    parser.add_argument("arasim_exec", type=Path,
                        help="path to AraSim executable")
    parser.add_argument("exp", type=int,
                        help="exponent of the energy for neutrinos in AraSim")
    parser.add_argument("nnt", type=int,
                        help="number of nuetrinos thrown in AraSim")
    parser.add_argument("run_name", type=str,
                        help="run name")
    parser.add_argument("seeds", type=int,
                        help="number of AraSim jobs per individual")
    parser.add_argument("debug_mode", type=int,
                        help="debug mode switch (0 for real runs)")
    args = parser.parse_args()

    main(args.gen, args.npop, args.working_dir, 
         args.arasim_exec, args.exp, args.nnt, 
         args.run_name, args.seeds, args.debug_mode)
