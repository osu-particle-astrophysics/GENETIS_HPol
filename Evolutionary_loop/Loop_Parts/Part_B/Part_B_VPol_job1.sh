#*******************************************************************************
#  Original file name: Part_B_GPU_job1.sh
#     This is Part B1 of the loop, which prepares and runs simulation_PEC.xmacro
#     with information such as the parameters of the antennas.
#
#  Programmer: OSU GENETIS Team
#
#  Revision history:
#     07/11/23  Jason Yao, third pass over the loop for training purposes
#     07/14/23  Jason Yao, merging Part_B_Curved_Constant_Quadratic_1.sh into
#               this file as a comment (see "CAT SECTION", line 147)
#
#  Notes:
#     * vertical ruler at column 80
#
#  TODO:
#
#*******************************************************************************

indiv=$1
gen=$2
NPOP=$3
WorkingDir=$4
RunName=$5
XmacrosDir=$6
XFProj=$7
GeoFactor=$8
num_keys=$9
curved=${10}
nsections=${11}

# If we're in the 0th generation, we need to make the directory for the XF jobs
if [ ${gen} -eq 0 ]
then
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/XF_Outputs
  mkdir -m775 $WorkingDir/Run_Outputs/$RunName/XF_Errors
fi

# We need to check if directories we're going to write to already exist
# (would occur if we go back to rerun the same generation)
# The directories are the simulation directories from gen*NPOP+1 to gen*NPOP+10

for i in `seq 1 $NPOP`
do
  # first, declare the number of the individual we are checking
  individual_number=$(($gen*$NPOP + $i))

  # next, write the potential directories corresponding to that individual
  if [ $individual_number -lt 10 ]
  then
    indiv_dir_parent=$XFProj/Simulations/00000$individual_number/
  elif [[ $individual_number -ge 10 && $individual_number -lt 100 ]]
  then
    indiv_dir_parent=$XFProj/Simulations/0000$individual_number/
  elif [[ $individual_number -ge 100 && $individual_number -lt 1000 ]]
  then
    indiv_dir_parent=$XFProj/Simulations/000$individual_number/
  elif [ $individual_number -ge 1000 ]
  then
    indiv_dir_parent=$XFProj/Simulations/00$individual_number/
  fi

  # now delete the directory if it exists
  if [ -d $indiv_dir_parent ]
  then
    rm -rf $indiv_dir_parent
  fi
done

# The number of the next simulation directory is held in a hidden file in the
# Simulations directory. The file is named .nextSimulationNumber
if [[ $gen -ne 0 ]]
then
  echo $(($gen*$NPOP + 1)) > $XFProj/Simulations/.nextSimulationNumber
fi

chmod -R 777 $XmacrosDir
cd $XmacrosDir

# frequencies are scaled up by 100 to avoid float operation errors in bash
freqlist=\
"8333 10000 11667 13333 15000 16667 18334 20000 21667 23334 25000 26667 28334 
30000 31667 33334 35000 36667 38334 40001 41667 43334 45001 46667 48334 50001 
51668 53334 55001 56668 58334 60001 61668 63334 65001 66668 68335 70001 71668 
73335 75001 76668 78335 80001 81668 83335 85002 86668 88335 90002 91668 93335 
95002 96668 98335 100000 101670 103340 105000 106670"

# empty the file without changing the permission
> simulation_PEC.xmacro

echo "var NPOP = $NPOP;" > simulation_PEC.xmacro
echo "var indiv = $indiv;" >> simulation_PEC.xmacro

# Now we can write the frequencies to simulation_PEC.xmacro
# first we need to declare the variable for the frequency lists
# the below commands write the frequency scale factor and "var freq =" to
# simulation_PEC.xmacro
echo "//Factor of $GeoFactor frequency" >> simulation_PEC.xmacro
echo "var freq " | tr "\n" "=" >> simulation_PEC.xmacro

# here's how we change our frequencies and put them in simulation_PEC.xmacro
for i in $freqlist; # iterating through all values in our list
do
  if [ $i -eq 8333 ] # we need to start with a bracket
  then
    echo " " | tr "\n" "[" >> simulation_PEC.xmacro
    # Whenever we append to a file, it adds a newline character at the end.
    # The tr command replaces the newline (\n) with a bracket.
    # (There's a space at the start; that will separate the = from the list by a
    # space.)
  fi

  # Now we're ready to start appending our new frequencies
  # We start by changing our frequencies by the scale factor;
  # we'll call this variable k
  k=$(($GeoFactor*$i))
  # Now we'll append our frequencies.The frequencies we're appending are divided
  # by 100, since the original list was scaled up by 100. IT'S IMPORTANT TO DO
  # IT THIS WAY. we can't just set k=$((scale*$i/100)) because of how bash
  # handles float operations. Instead, we need to echo it with the | bc command
  # to allow float quotients

  if [ $i -ne 106670 ]
  then
    echo "scale=2 ; $k/100 " | bc | tr "\n" "," >> simulation_PEC.xmacro
    echo "" | tr "\n" " " >> simulation_PEC.xmacro
    # The above gives spaces between commas and numbers.
    # We have to be careful! We want commas between numbers,
    # but not after our last number, so we replace \n with comma above,
    # but with "]" below
  else
    echo "scale=2 ; $k/100 " | bc | tr "\n" "]" >> simulation_PEC.xmacro
    echo " " >> simulation_PEC.xmacro
  fi
done


if [[ $gen -eq 0 && $indiv -eq 1 ]]
then
  echo "if(indiv==1){" >> simulation_PEC.xmacro
  echo \
    "App.saveCurrentProjectAs(\"$WorkingDir/Run_Outputs/$RunName/$RunName\");"\
    >> simulation_PEC.xmacro
  echo "}" >> simulation_PEC.xmacro
fi

## CAT SECTION ##
# If we want to run Part_B_Curved_Constant_Quadratic_1.sh (now deleted),
# uncomment the following two cat commands instead.
# cat simulationPECmacroskeleton_curved_constant_quadratic.txt \
#     >> simulation_PEC.xmacro
# cat simulationPECmacroskeleton2_curved_constant_quadratic.txt \
#     >> simulation_PEC.xmacro
if [ $curved -eq 0 ]; then
  if [ $nsections -eq 1 ]; then # straight side symmetric bicone
    cat simulationPECmacroskeleton_GPU.txt >> simulation_PEC.xmacro
    cat simulationPECmacroskeleton2_GPU.txt >> simulation_PEC.xmacro 
  else                          # straight side asymmetric bicone
    cat simulationPECmacroskeleton_Sep.txt >> simulation_PEC.xmacro
    cat simulationPECmacroskeleton2_Sep.txt >> simulation_PEC.xmacro
  fi
else                            # curved bicone
  cat simulationPECmacroskeleton_curved.txt >> simulation_PEC.xmacro
  cat simulationPECmacroskeleton2_curved.txt >> simulation_PEC.xmacro
fi
## END OF CAT SECTION ##

## We need to change the gridsize by the same factor as the antenna size
## The gridsize in the macro skeleton is currently set to 0.1
## We want to make it scale in line with our scalefactor
initial_gridsize=0.1
new_gridsize=$(bc <<< "scale=6; $initial_gridsize/$GeoFactor")
## I'm going to test smaller grid sizes
# gen_grid_factor=$((($gen*5+1)))
# new_gridsize=$(bc <<< "scale=6; $initial_gridsize/$gen_grid_factor")
sed -i "" "s/var gridSize = 0.1;/var gridSize = $new_gridsize;/" simulation_PEC.xmacro
# echo "New grid size is ${new_gridsize}"

sed -i "" "s+fileDirectory+${WorkingDir}/Generation_Data+" simulation_PEC.xmacro
# sed swaps out the hardcoded words without using dummy files (option i)
# The first empty quote after sed -i is there for this to work on MacOS
# (Jason 07/11/23;
# https://stackoverflow.com/questions/12272065/sed-undefined-label-on-macos)

# if Ice = 1, use sed to replace //CreateIce(); with CreateIce();
# Else just leave it


if [[ $gen -ne 0 && $i -eq 1 ]]
then
  cd $XFProj
  rm -rf Simulations
fi

echo
echo
echo 'Opening XF user interface...'
echo '*** Please remember to save the project with the same name as RunName! ***'
echo
echo '1. Import and run simulation_PEC.xmacro'
echo '2. Import and run output.xmacro'
echo '3. Close XF'

module load xfdtd/7.9.2.2
#if [ $gen -ne 0] 
#then
# Xvnc :5 &  DISPLAY=:5 xfdtd $XFProj \
#   --execute-macro-script=$XmacrosDir/simulation_PEC.xmacro || true
#else
#mkdir -m775 ${gen}_Antenna_Images
xfdtd $XFProj --execute-macro-script=$XmacrosDir/simulation_PEC.xmacro || true
#fi
chmod -R 775 $XmacrosDir


## Here is where we submit the GPU job

# determine how many jobs in the job array will run simultaeously
if [ $NPOP -lt $num_keys ]
then
  batch_size=$NPOP
else
  batch_size=$num_keys
fi

# We'll make the run name the job name so as to use it with SBATCH commands.
cd $WorkingDir
sbatch --array=1-${NPOP}%${batch_size} --job-name=${RunName} --export=ALL,\
WorkingDir=$WorkingDir,RunName=$RunName,XFProj=$XFProj,NPOP=$NPOP,gen=${gen}\
 Batch_Jobs/GPU_XF_Job.sh
