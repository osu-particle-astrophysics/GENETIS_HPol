########  XF output conversion code (C)  ######################################################
#
#         1. Converts .uan file from XF into a readable .dat file that Arasim can take in.
#
###############################################################################################

# variables
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