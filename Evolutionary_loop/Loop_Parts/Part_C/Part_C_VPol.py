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

Usage: python3 Part_C_VPol.py <num_pop> <working_dir> <run_name> <gen> <indiv>

For more information on the arguments: 
  >> python3 part_c_vpol.py -h
'''

import argparse     # for parsing command-line arguments
import os           # for changing file permissions

# ARGUMENTS
parser = argparse.ArgumentParser();
parser.add_argument("num_pop", type=int,
                    help="number of individual per generation (NPOP)");
parser.add_argument("working_dir",
                    help="working directory (WorkingDir)");
parser.add_argument("run_name",
                    help="RunName");
parser.add_argument("gen", type=int,
                    help="index of generation");
parser.add_argument("indiv", type=int,
                    help="index of individual");
args = parser.parse_args();


# GLOBALS

# rows and columns for matrix "mat" used in read_file() and main()
num_rows = 73 # grouping rows inside the .uan file by the 1st-column numbers
num_cols = 37 # (73 rows per group, 37 groups in total)

# 1st column of the .uan file is the polar (zenith) angle: 0 to 180 deg.
# 2nd column is the azimuthal angle: 0 to 360 deg
# Format of the .uan files (as compared to that of AraSim, check out line 77):
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
def read_file(indiv:int, freq_idx:int):

    # uan_files have subdirectories labeled by generations, eg. 0_uan_files
    # Each generation subdirectories then have subsubdirectories labeled by
    # the index of individuals (1 to num_pop where num_pop is indiv. per gen.)
    # inside these subsubdirectories, there are 60 .uan files:
    #   one .uan file for each frequency in the list freq_list above.
    uan_name = (f'{args.working_dir}/Run_Outputs/'
                f'{args.run_name}/uan_files/{args.gen}_uan_files/'
                f'{indiv}/{args.gen}_{indiv}_{freq_idx}.uan')
    with open(uan_name) as f:
        for foo in range(18):
            f.readline()  # discarding header lines in the .uan file

        # initialize a (num_rows X num_cols) matrix to zeros
        mat = [["0" for x in range(num_cols)] for y in range(num_rows)]

        # looping through all lines in the .uan file (37 * 73 = 2701 lines)
        for i in range(num_cols):
            for j in range(num_rows):

                line = f.readline()
                line_list = line.split(" ")
                linear_gain = "%.2f" % 10**(float(line_list[2])/10)

                line_final = (line_list[0]                 + "\t" + "  " + 
                              line_list[1]                 + "\t" + "  " +
                              "%.2f" % float(line_list[2]) + "\t" +
                              str(linear_gain)             + "\t" +
                              "%.2f" % float(line_list[5]) + "\n")
            
                mat[j][i] = line_final
        f.close()        
    return mat


def main():
  for antenna in range(1, args.num_pop+1):

    # For each individual (antenna), open a new file: evol_antenna_model_?.dat
    # where ? is a number from 1 to num_pop (number of individuals per gen.)
    with open(f'{args.working_dir}/Antenna_Performance_Metric/'
              f'evol_antenna_model_{antenna}.dat',"w+") as dat_file:
      
        # change file permission (same as chmod a+rwx in bash)
        os.chmod(f'{args.working_dir}/Antenna_Performance_Metric/'
                 f'evol_antenna_model_{antenna}.dat',0o777)

# For each individual, combine all 60 frequency responses in a single .dat file.
# There are 60 frequencies in freq_list and correspondingly 60 .uan files.
# (insde .../uan_files/0_uan_files/1/, as an example, there are 60 .uan files;
# each of these contain the frequency response of generation-zero-antenna-one.
# by frequency response we refer to the gain of the individual at some
# combination of (polar,azimuthal) angle pair.
        for frq_idx,freq in enumerate(freq_list):
            dat_file.write(f'freq: {freq} MHz\n')
            dat_file.write( 'SWR: 1.965000\n')
            dat_file.write( 'Polar     Azimuthal     Gain(dB)     '
                            'Gain     Phase(deg)\n')

            mat = read_file(antenna, frq_idx+1)

# For each polar angle group, discard the 360-degree azimuthal angle (360 == 0)
# That is, for each of the 37 groups, discard the last row, as done below
            for azi in range(num_rows-1):
                for pol in range(num_cols):
                    dat_file.write(mat[azi][pol])
# format for AraSim (for each of the 60 frequencies)
# for the same azimuthal angle (2nd column), increase the polar angle by 5 deg.
# then move on to the next azimuthal angle, rinse and repeat.

if __name__ == '__main__':
    main()