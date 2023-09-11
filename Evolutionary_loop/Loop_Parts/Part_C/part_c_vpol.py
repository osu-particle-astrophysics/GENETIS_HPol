#*******************************************************************************
#  File: part_c_vpol.py
#
#  Programmer: Jason Yao
#
#  Revision history:
#     07/19/23  Original version based on Part_C.sh and XFintoARA.py
#
#  Notes:
#     * vertical ruler at column 80
#     * for reference of the structure of the uan files, see sample.uan;
#       more explanation below inside main() as comments
#
#  TODO:
#     * freq_list hardcoded (see freqlist in Part_B_VPol_job1.sh, line 80); 
#       it is probably better if freqlist can somehow be passed instead.
#     * use something to automatically format output files instead of manually
#       inserting tabs and spaces.
#
#*******************************************************************************
'''
This is Part C of the loop, which is responsible for converting the .uan files 
(made in Part B by XF) into AraSim-readable .dat files.

Usage: python3 Part_C_VPol.py <num_pop> <working_dir> <run_name> <gen> <arasim>

For more information on the arguments: 
  >> python3 part_c_vpol.py -h
'''

import argparse     # for parsing command-line arguments
import os           # for changing file permissions

from pathlib import Path

# ARGUMENTS


# GLOBALS

# rows and columns for matrix "mat" used in read_file() and main()
num_rows = 73 # grouping rows inside the .uan file by the 1st-column numbers
num_cols = 37 # (73 rows per group, 37 groups in total)

# 1st column of the .uan file is the polar (zenith) angle: 0 to 180 deg.
# 2nd column is the azimuthal angle: 0 to 360 deg
# Format of the .uan files:
#   keep the polar angle (first column) the same, increase the azimuthal (2nd
#   column) angles; then move on to the next polar angle, rinse and repeat.

# vpol frequencies (MHz)
freq_list = [ 83.33, 100.00, 116.67, 133.33, 150.00, 166.67, 183.34, 200.00,
             216.67, 233.34, 250.00, 266.67, 283.34, 300.00, 316.67, 333.34, 
             350.00, 366.67, 383.34, 400.01, 416.67, 433.34, 450.01, 466.67, 
             483.34, 500.01, 516.68, 533.34, 550.01, 566.68, 583.34, 600.01, 
             616.68, 633.34, 650.01, 666.68, 683.35, 700.01, 716.68, 733.35, 
             750.01, 766.68, 783.35, 800.01, 816.68, 833.35, 850.02, 866.68,
             883.35, 900.02, 916.68, 933.35, 950.02, 966.68, 983.35, 1000.00,
             1016.70, 1033.40, 1050.00, 1066.70]


# FUNCTIONS
''' Reads the .uan file and converts to a matrix "mat" '''
def read_file(antenna_idx:int, freq_idx:int,
              working_dir:Path, run_name:str, gen:int):

    # uan_files/ have subdirectories labeled by generations, eg. 0_uan_files.
    # Each generation-subdirectories then have subsubdirectories labeled by
    # the index of individuals (1 to num_pop where num_pop is indiv. per gen.).
    # Inside these subsubdirectories, there are 60 .uan files:
    #   one .uan file for each frequency in the list freq_list above.
    uan_name = (working_dir / f'Run_Outputs' / f'{run_name}' /
                'uan_files' / f'{args.gen}_uan_files' / f'{antenna_idx}' /
                f'{gen}_{antenna_idx}_{freq_idx}.uan')
    with open(uan_name) as f:
        for foo in range(18):
            f.readline()  # discarding header lines in the .uan file

        # initialize a (num_rows X num_cols) matrix to zeros
        mat = [["0" for x in range(num_cols)] for y in range(num_rows)]

        # looping through all lines in the .uan file (37 * 73 = 2701 lines)
        for i in range(num_cols):
            for j in range(num_rows):

                line = f.readline()      # .split(" ") splits by ONE whitespace
                line_list = line.split() # .split() splits all (including tabs)
                linear_gain = f'{10**(float(line_list[2])/10):.2f}'

                line_final = (line_list[0]                 + "\t\t\t" +
                              line_list[1]                 + "\t\t\t" +
                              f'{float(line_list[2]):.2f}' + "\t\t "  +
                              linear_gain                  + "\t\t"   +
                              f'{float(line_list[5]):.2f}' + "\n")
            
                mat[j][i] = line_final
    return mat


def main(num_pop, working_dir, run_name, gen, arasim_exec):
# For each individual (antenna), open a new file: evol_antenna_model_?.dat
# where ? is a number from 1 to num_pop (number of individuals per gen.)
    for antenna in range(1, num_pop+1):

        # output file name
        of_name=(arasim_exec / f'a_{antenna}.txt')
        with open(of_name, "w") as ofstream:
            os.chmod(of_name,0o777) # change file permission (ie. chmod a+rwx)

# In a .uan file, each row starts with a pair of (polar, azimuthal) angles
# and records the gain of the antenna at that angle at some specified frequency.
# eg. "0_uan_files/27/0_27_60.uan" would (with 37*73 rows) document the gains
# of the 0th-generation-antenna-27 at 1066.70 Mhz (the 60th freq. in freq_list)

# There are 60 frequencies in freq_list and correspondingly 60 .uan files for
# each individual. Combine all 60 .uan into a single .dat file for AraSim.
# (eg. inside uan_files/0_uan_files/1/, there are 60 .uan files for antenna 1)
            for frq_idx,freq in enumerate(freq_list):
                ofstream.write(f'freq: {freq} MHz\n')
                ofstream.write( 'SWR: 1.965000\n')
                ofstream.write( 'Polar     Azimuthal     Gain(dB)     '
                                'Gain     Phase(deg)\n')

                mat = read_file(antenna, frq_idx+1, working_dir,
                                run_name, gen)

# For each polar angle group, discard the 360-degree azimuthal angle (360 == 0)
# That is, for each of the 37 groups, discard the last row, as done below
                for azi in range(num_rows-1):
                    for pol in range(num_cols):
                        ofstream.write(mat[azi][pol])
# format for AraSim (for each of the 60 frequencies)
# for the same azimuthal angle (2nd column), increase the polar angle by 5 deg.
# then move on to the next azimuthal angle, rinse and repeat.

if __name__ == '__main__':
    parser = argparse.ArgumentParser();
    parser.add_argument("num_pop", type=int,
                        help="number of individual per generation (NPOP)");
    parser.add_argument("working_dir", type=Path,
                        help="working directory (WorkingDir)");
    parser.add_argument("run_name",
                        help="RunName");
    parser.add_argument("gen", type=int,
                        help="index of generation");
    parser.add_argument("arasim_exec", type=Path,
                        help="Path to AraSim directory");
    args = parser.parse_args();

    main(args.num_pop, args.working_dir, args.run_name,
         args.gen, args.arasim_exec)
