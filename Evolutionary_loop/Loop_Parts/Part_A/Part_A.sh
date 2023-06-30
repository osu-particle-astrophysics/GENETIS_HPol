#*******************************************************************************
#  This is HPol Part A, based on Part_A_PUEO.sh from the GENETIS_PUEO project.
#	 This part of the loop:
#			1. Runs the genetic algorithm (GA)
#			2. Moves the GA outputs (.csv files) to the proper location so they are
#				 not overwritten.
#
#  Programmer: OSU GENETIS Team
#
#  Revision history:
#     06/30/23  Started modifying from Part_A.sh to Part_A_HPol.sh
#
#  Notes:
# 		* vertical ruler at column 80
#
#  TODO:
#
#*******************************************************************************

# VARIABLES
gen=$1
NPOP=$2
WorkingDir=$3
RunName=$4
GeoFactor=$5
rank_no=$6
roulette_no=$7
tournament_no=$8
reproduction_no=$9
crossover_no=${10}
mutationRate=${11}
sigma=${12}

cd $WorkingDir

g++ -std=c++11 GA/Shared-Code/GA/SourceFiles/New_GA.cpp -o GA/New_GA.exe
./GA/New_GA.exe PUEO $gen $NPOP $rank_no $roulette_no $tournament_no $reproduction_no $crossover_no $mutationRate $sigma

cp Generation_Data/generationDNA.csv Run_Outputs/$RunName/Generation_Data/${gen}_generationDNA.csv
mv Generation_Data/generators.csv Run_Outputs/$RunName/Generation_Data/${gen}_generators.csv

if [ $gen -gt 0 ]
then
	mv Generation_Data/parents.csv Run_Outputs/$RunName/Generation_Data/${gen}_parents.csv
fi
chmod -R 775 Generation_Data/