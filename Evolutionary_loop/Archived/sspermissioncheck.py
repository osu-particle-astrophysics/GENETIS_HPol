#*******************************************************************************
#  file: sspermissioncheck.py
#     Based on ssPermissionCheck.sh, this is an attempt to migrate to Python.
#     This is currently left inside Archived as a python template
#     but will be deleted eventually, as we no longer use ssPermissionCheck.sh
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
#     * ssPermissionCheck.sh sends email with error messages
#
#*******************************************************************************
'''
This script checks if the user-specified checkpoint (usr_spec) matches what
is actually stored inside the SaveState.txt files.

Usage: python3 sspermissioncheck.py <run_name> <working_dir> <usr_spec>

For more information on the arguments: 
  >> python3 sspermissioncheck.py -h
'''

import argparse
import time

from pathlib import Path


def main(file_path):
    with open(file_path) as f:          # note: default open mode is read
        f.readline()                    # discard the first line (generation)
        checkpoint = int(f.readline())  # read the second line (state)


    if checkpoint == args.usr_spec:
        print('Hurray! Checkpoint (line 2) in SaveState.txt matches '
              'what you specified')

    else:
        # The original Bash script has the following; usefulness TBD
        print("2nd attemp...")
        time.sleep(15)

        if checkpoint == args.usr_spec:
            print('Hurray! Checkpoint (line 2) in SaveState.txt matches '
                  'what you specified')
        else:
            print('SaveState did not update after jobs finished. '
                  'Permissions were likely not opened after the last run.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    # nargs='?' makes arg optional
    parser.add_argument("run_name", help="Run Name", type=str,
                        default="test_run", nargs='?')
    parser.add_argument("working_dir", type=Path,
                        help="Working Directory (Evolutionary_Loop)",
                        default="/Users/Jason/Documents/OSU/GENETIS/GENETIS_HPol/"
                        "Evolutionary_loop", nargs='?')
    parser.add_argument("usr_spec", help="User-specified checkpoint (aka state)",
                        type=int, default="4", nargs='?')
# For more info on nargs and Path, see Ezio's comment at
# https://github.com/osu-particle-astrophysics/GENETIS_HPol/pull/3#discussion_r1268767075
    args = parser.parse_args()


    # The "f" below stands for "f-strings".
    # This is cleaner than the old way of using %'s for string interpolation.
    file_path = args.working_dir / 'SaveStates' / f'{args.run_name}_SaveState.txt'
    # Because working_dir's type is set to pathlib.Path, 
    # there is no need to put it inside fstring interpolation braces.
    # (Github PR #3 conversation, link above)

    main(file_path)

