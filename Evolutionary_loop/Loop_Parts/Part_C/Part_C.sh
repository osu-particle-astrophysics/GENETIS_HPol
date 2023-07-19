#*******************************************************************************
#  file: Part_C.sh
#     This is Part C of the loop, which is responsible for converting the .uan
#     files (made in Part B by XF) into AraSim-readable .dat files.
#
#  Programmer: OSU GENETIS Team
#
#  Revision history:
#     07/19/23  Jason Yao, removed the part that's contained in Part B2. 
#
#  Notes:
#     * vertical ruler at column 80
#
#  TODO:
#
#*******************************************************************************

NPOP=$1
WorkingDir=$2
RunName=$3
gen=$4
indiv=$5

# move the .uan files to the run directory
cd $WorkingDir/Antenna_Performance_Metric
mv *.uan $WorkingDir/Run_Outputs/$RunName/uan_files/

# XFintoARA.py will read them and then in part D we will move them into dedicated directories
python XFintoARA.py $NPOP $WorkingDir $RunName $gen $indiv
# Run AraSim -- feeds the plots into AraSim 
# First we convert the plots from XF into AraSim readable files,
# then we move them to AraSim directory and execute AraSim