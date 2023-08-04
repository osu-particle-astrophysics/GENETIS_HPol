#*******************************************************************************
#  This is the HPol main loop, based on Asym_XF_Loop.h from the VPol main loop.
#
#  Programmer: Alex Machtay (machtay.1@osu.edu)
#              Julie Rolla (Julie.a.rolla@jpl.nasa.gov)
#              Jason Yao (yao.966@osu.edu)
#
#  Revision history:
#     06/30/23  Started modifying from VPol to HPol
#
#  Notes:
#     * vertical ruler at column 80
#
#  TODO:
#
#
#*******************************************************************************
#!/bin/bash
#SBATCH -e /fs/ess/PAS1960/GENETIS_HPol/Evolutionary_loop/scriptEOFiles
#SBATCH -o /fs/ess/PAS1960/GENETIS_HPol/Evolutionary_Loop/scriptEOFiles
module load python/3.6-conda5.2 


# VARIABLES
# general variables
RunName='2023_02_20_Symmetric_Run'
Design="Symmetric Dipole" # antenna type
TotalGens=100   # number of generations (after initial) to run through
NPOP=50         # number of individuals per generation; keep below 99
Seeds=10        # number of AraSim jobs per individual; see Julie's dissertation
NNT=30000       # Number of Neutrinos Thrown in AraSim   
exp=18          # exponent of the energy for the neutrinos in AraSim
ScaleFactor=1.0 # factor used to punish antennas larger than the drilling holes
GeoFactor=1     # factor used to scale DOWN antennas. 

# GA variables
REPRODUCTION=5  # number (not fraction!) of individuals formed by reproduction
CROSSOVER=0     # number (not fraction!) of individuals formed by crossover
MUTATION=0      # probability of mutation (percent)
SIGMA=6         # standard deviation for the mutation operation (percent)
ROULETTE=5      # percent of individuals selected by roulette (divided by 10)
TOURNAMENT=0    # percent of individuals selected by tournament (divided by 10)
RANK=0          # percent of individuals selected by rank (divided by 10)
ELITE=0         # Elite function on/off (1/0)

# PUEO variables
SYMMETRY=1
if [ "${Design}" = "PUEO" ]; then
  if [ $SYMMETRY -eq 0 ]; then
    XFCOUNT=$((NPOP*2))
  else
    XFCOUNT=$NPOP
  fi
fi

# bicone variables
RADIUS=0        # 0 for symmetric radius
LENGTH=0        # 0 for symmetric length
ANGLE=0         # 0 for symmetric angle
CURVED=0        # 0 for straight sides; 1 for curved
A=0             # If 1, A is asymmetric (A x^2 + B describes the curve of sides)
B=0             # If 1, B is asymmetric
SEPARATION=0    # 0 for constant separation. If 1, separation evolves.
NSECTIONS=1     # number of sections (1 for symmetric bicone; 2 for asymmetric.)

# flags
DEBUG_MODE=0    # 0 for real runs; 1 for testing (ex: send specific seeds)
# XF variables
num_keys=4      # number of XF keys
FREQ=60         # number of frequencies being iterated over in XF 
                # (Currectly only affects the output.xmacro loop)


# DIRECTORY INITIALIZATION
# HPol location on OSC: 
# WorkingDir=/fs/ess/PAS1960/GENETIS_HPol/Evolutionary_loop
# XmacrosDir=/fs/ess/PAS1960/GENETIS_HPol/Xmacros
# AraSimExec=/fs/ess/PAS1960/BiconeEvolutionOSC/Original_GENETIS_AraSim/AraSim 
# XFProj=$WorkingDir/Run_Outputs/${RunName}/${RunName}.xf
WorkingDir=/Users/Jason/Documents/OSU/GENETIS/GENETIS_HPol/Evolutionary_loop
XmacrosDir=/Users/Jason/Documents/OSU/GENETIS/GENETIS_HPol/Xmacros
XFProj=$WorkingDir/Run_Outputs/${RunName}/${RunName}.xf


# SAVE STATE: in case the loop is interrupted
# Check if SaveState exists; if not, initialized one.
SaveStateFile=${RunName}_SaveState.txt
cd ${WorkingDir}/SaveStates
if ! [ -f "${SaveStateFile}" ]; then
  echo "SaveState does not exist. Making one and starting new run"
  echo 0 >  ${RunName}_SaveState.txt # generation
  echo 0 >> ${RunName}_SaveState.txt # checkpoint
  echo 1 >> ${RunName}_SaveState.txt # individual
fi
cd ..

# Read the current state
line=1
InititalGen=0
state=0
indiv=0
while read p; do
  if [ $line -eq 1 ]
  then
    InitialGen=$p 
  fi
  
  if [ $line -eq 2 ]
  then
    state=$p
  fi
  
  if [ $line -eq 3 ]
  then
    indiv=$p
  fi
  
  if [ $line -eq 2 ]
  then
    line=3
  fi

  if [ $line -eq 1 ]
  then
    line=2
  fi
done < SaveStates/$SaveStateFile # reads in SaveStateFile into the while loop
echo "${InitialGen}"
echo "${state}"
echo "${indiv}"


# The LOOP
for gen in `seq $InitialGen $TotalGens`; do

# Make the RunName directories at the start of a new run
if [[ $gen -eq 0 && $state -eq 0 ]]; then
  # read -p \
  # "To start generation ${gen} at checkpoint ${state}, press any key..." -n1 -s
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/AraSimFlags
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/AraSimConfirmed
	mkdir -m775 $WorkingDir/Run_Outputs/$RunName/AraSim_Outputs
	mkdir -m775 $WorkingDir/Run_Outputs/$RunName/AraSim_Errors
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/GPUFlags
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/XFGPUOutputs
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/XF_Outputs
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/XF_Errors
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/uan_files
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/Gain_Plots
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/Antenna_Images
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/AraOut
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/Generation_Data
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/Evolution_Plots
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/Root_Files

  # Recording run date and run detail
  python Data_Generators/dateMaker.py
  mv -f runDate.txt \
     $WorkingDir/Run_Outputs/$RunName/run_details.txt
  echo "\n" >> $WorkingDir/Run_Outputs/$RunName/run_details.txt
  head -n 62 Loop_Scripts/main.sh | tail -n 37 >> \
    $WorkingDir/Run_Outputs/$RunName/run_details.txt
  state=1
  ./SaveState_Prototype.sh $gen $state $RunName $indiv
fi


# PART A: running GA (output: generationDNA.csv and parents.csv)
if [ $state -eq 1 ]; then
  ./Loop_Parts/Part_A/Part_A.sh \
    $gen $NPOP $WorkingDir $RunName $GeoFactor $RANK $ROULETTE $TOURNAMENT \
    $REPRODUCTION $CROSSOVER $MUTATION $SIGMA "$Design"
    # Note that the double quote around $Design is required to call the GA
  state=2
  ./SaveState_Prototype.sh $gen $state $RunName $indiv
fi


# PART B (output: .uan files)
# Part b1
if [ $state -eq 2 ]; then
  if [ "${Design}" = "PUEO" ]; then       # PUEO
    ./Loop_Parts/Part_B/Part_B_PUEO.sh \
      $indiv $gen $NPOP $WorkingDir $RunName $XmacrosDir \
      $XFProj $GeoFactor $num_keys $SYMMETRY $XFCOUNT

  elif [ "${Design}" = "HPol" ]; then     # HPol
    echo "Put Hpol Job1 Skeleton Here!"

  else                                    # Bicones (symm, asymm, curved, etc.)
    python3 $WorkingDir/Loop_Parts/Part_B/part_b_job1.py \
      $indiv $gen $NPOP $WorkingDir $RunName $XmacrosDir \
      $XFProj $GeoFactor $num_keys $CURVED $NSECTIONS
  fi

  state=3
  ./SaveState_Prototype.sh $gen $state $RunName $indiv
fi
  
# Part b2
if [ $state -eq 3 ]; then
  if [ "${Design}" = "PUEO" ]; then       # PUEO
    echo "pueo part b2 stuff here!"
  
  elif [ "${Design}" = "HPol" ]; then     # HPol
    echo "Put Hpol Job2 Skeleton Here!"
  
  else                                    # Bicones (symm, asymm, curved, etc.)
  python3 $WorkingDir/Loop_Parts/Part_B/part_b_job2.py \
      $indiv $gen $NPOP $WorkingDir $RunName $XmacrosDir \
      $XFProj $GeoFactor $num_keys $CURVED $NSECTIONS
  fi

  state=4
  ./SaveState_Prototype.sh $gen $state $RunName $indiv
fi


# PART C
if [ $state -eq 4 ]; then
  indiv=1
  if [ "${Design}" = "PUEO" ]; then
    echo "pueo part c stuff here!"

  elif [ "${Design}" = "HPol" ]; then
    echo "hpol part c stuff here!"

  else
    python3 Loop_Parts/Part_C/part_c_vpol.py \
      $NPOP $WorkingDir $RunName $gen $AraSimExec
  fi

  state=5
  ./SaveState_Prototype.sh $gen $state $RunName $indiv
fi

# PART D
# Part d1
if [ $state -eq 5 ]
then
  python3 ./Loop_Parts/Part_D/part_d_job1.py\
    $gen $NPOP $WorkingDir $AraSimExec $exp $NNT $RunName $Seeds $DEBUG_MODE

  state=6
  ./SaveState_Prototype.sh $gen $state $RunName $indiv
fi

# ## Part D2 ##
# if [ $state -eq 6 ]
# then
#   # ./Part_D2_AraSeed.sh 
#   ./Loop_Parts/Part_D/Part_D2_Array.sh $gen $NPOP $WorkingDir $RunName $Seeds $AraSimExec
#   # ./Loop_Parts/Part_D/Part_D2_AraSeed_Notif.sh \
#   #  $gen $NPOP $WorkingDir $RunName $Seeds $AraSimExec
#   state=7
#   ./SaveState_Prototype.sh $gen $state $RunName $indiv
# fi

# ## Part E ##
# ## Concatenates the AraSim data files into a string so that it's usable for getting scores
# ## Gets important information on the fitness scores and generation DNA
# ## moves the .uan files from Antenna Performance Metric to RunOutputs/$RunName folder
# if [ $state -eq 7 ]
# then
#   if [ $CURVED -eq 0 ]  # Evolve straight sides
#   then
#     ./Loop_Parts/Part_E/Part_E_Asym.sh $gen $NPOP $WorkingDir $RunName $ScaleFactor \ 
#       $indiv $Seeds $GeoFactor $AraSimExec $XFProj $NSECTIONS $SEPARATION
#   else      # Evolv curved sides
#     ./Loop_Parts/Part_E/Part_E_Curved.sh $gen $NPOP $WorkingDir $RunName $ScaleFactor \ 
#       $indiv $Seeds $GeoFactor $AraSimExec $XFProj $NSECTIONS $SEPARATION $CURVED
#   fi
#   state=8
#   ./SaveState_Prototype.sh $gen $state $RunName $indiv 
# fi

# ## Part F ##
# if [ $state -eq 8 ]
# then
#   if [ $CURVED -eq 0 ]
#   then
#     ./Loop_Parts/Part_F/Part_F_asym.sh $NPOP $WorkingDir $RunName $gen $Seeds $NSECTIONS
#   else
#     ./Loop_Parts/Part_F/Part_F_Curved.sh $NPOP $WorkingDir $RunName $gen $Seeds $NSECTIONS
#   fi
#   state=1
#   ./SaveState_Prototype.sh $gen $state $RunName $indiv

# fi
done

# cp generationDNA.csv "$WorkingDir"/Run_Outputs/$RunName/FinalGenerationParameters.csv
# mv runData.csv Antenna_Performance_Metric

# #########################################################################################################################
# ###Moving the Veff AraSim output for the actual ARA bicone into the $RunName directory so this data isn't lost in     ###
# ###the next time we start a run. Note that we don't move it earlier since (1) our plotting software and fitness score ###
# ###calculator expect it where it is created in "$WorkingDir"/Antenna_Performance_Metric, and (2) we are only creating ###
# ###it once on gen 0 so it's not written over in the looping process.                                                  ###
# ########################################################################################################################
# cd "$WorkingDir"
# mv AraOut_ActualBicone.txt "$WorkingDir"/Run_Outputs/$RunName/AraOut_ActualBicone.txt