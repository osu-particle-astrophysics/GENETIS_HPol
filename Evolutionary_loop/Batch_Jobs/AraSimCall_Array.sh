#!/bin/bash
#SBATCH -A PAS1960
#SBATCH -t 18:00:00
#SBATCH -N 1
#SBATCH -n 8
#SBATCH --output=/fs/ess/PAS1960/GENETIS_HPol/Evolutionary_Loop/Run_Outputs/%x/AraSim_Outputs/AraSim_%a.output
#SBATCH --error=/fs/ess/PAS1960/GENETIS_HPol/Evolutionary_Loop/Run_Outputs/%x/AraSim_Errors/AraSim_%a.error

source $AraSimDir/new_root_setup.sh
source $AraSimDir/setup.sh
source $AraSimDir/araenv.sh # for AraSim libraries

cd $AraSimDir
num=$(($((${SLURM_ARRAY_TASK_ID}-1))/${Seeds}+1)) # num denotes antenna number
seed=$(($((${SLURM_ARRAY_TASK_ID}-1))%${Seeds}+1))
echo a_${num}_${seed}.txt

chmod -R 777 $AraSimDir/outputs/
./AraSim setup.txt ${SLURM_ARRAY_TASK_ID} $TMPDIR a_${num}.txt > \
  $TMPDIR/AraOut_${gen}_${num}_${seed}.txt 

cd $TMPDIR
echo "Let's see what's in TMPDIR:"
ls -alrt 

echo $gen > $TMPDIR/${num}_${seed}.txt
echo $num >> $TMPDIR/${num}_${seed}.txt
echo $seed >> $TMPDIR/${num}_${seed}.txt

# Moving stuff from scratch space back to GE60; Jason 061523
mv AraOut.setup.txt.run${SLURM_ARRAY_TASK_ID}.root\
 $WorkingDir/Antenna_Performance_Metric/AraOut_${gen}_${num}_${seed}.root
mv AraOut_${gen}_${num}_${seed}.txt $WorkingDir/Antenna_Performance_Metric/
mv ${num}_${seed}.txt $WorkingDir/Run_Outputs/$RunName/AraSimFlags

## This part appears unnecessary now
: << 'END'
# we need to go fix the file names from the jobs
# first, the AraOut Files
cd $WorkingDir/Antenna_Performance_Metric

## The below commands are only relevant when using pbsdcp
## They'll print an error to the error file, but they shouldn't really cause a problem
cp AraOut_${gen}_${num}_${seed}.txt.* AraOut_${gen}_${num}_${seed}.txt
rm AraOut_${gen}_${num}_${seed}.txt.*

# now do the flag files
cd $WorkingDir/Run_Outputs/$RunName/AraSimFlags

cp ${num}_${seed}.txt.* ${num}_${seed}.txt
rm ${num}_${seed}.txt.*
END