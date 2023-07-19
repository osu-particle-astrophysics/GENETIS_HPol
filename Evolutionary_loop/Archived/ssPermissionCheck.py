#*******************************************************************************
#  file: ssPermissionCheck.py
#     This script is based on ssPermissionCheck.sh
#     This is an attempt to slowly migrate to Python.
#
#  Programmer: Jason Yao (yao.966@osu.edu)
#
#  Revision history:
#     07/18/23  Original version
#
#  Notes:
#     * vertical ruler at column 80
#
#  TODO: 
#     * ssPermissionCheck.sh seems capable of waiting (sleep 1m);
#       add this in the future.
#     * ssPermissionCheck.sh sends email with error messages; add this.
#
#*******************************************************************************
import argparse as ap

### ARGUMENTS ###
# Use the following command to checkout the arguments in the terminal
# >> Python3 ssPermissionCheck.py -h
parser = ap.ArgumentParser()
parser.add_argument("RunName", help="Run Name")
parser.add_argument("WorkingDir", help="Working Directory (Evolutionary_Loop)")
parser.add_argument("usr_spec", help="User-specified checkpoint (aka state)")
g = parser.parse_args()


### MESSAGES ###
congrats = \
"Hurray! Checkpoint (line 2) in SaveState.txt matches what you specified"
err = \
"SaveState did not update after jobs finished.\
 Permissions were likely not opened after the last run."


### READING ###
# The "f" below stands for "f-strings".
# This is cleaner than the old way of using %'s for string interpolation.
file_path = f'{g.WorkingDir}/SaveStates/{g.RunName}_SaveState.txt'

file = open(file_path,"r")    # Open the file (read mode)
file.readline()               # reading the first line (generation); not needed
checkpoint = file.readline()  # reading the second line (state)

### MAIN ###
if (int(checkpoint)==int(g.usr_spec)):
    print()
else:
    print(err);
