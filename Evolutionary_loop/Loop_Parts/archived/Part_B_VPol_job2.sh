#!/bin/bash
#*******************************************************************************
#  Original file name: Part_B_GPU_job2_asym_array.sh
#      This is Part B2 of the loop, which prepares and runs output.xmacro with 
#      the relevant parameter (antenna type, population number, grid size, etc)
#      Output.xmacro writes the XFDtd simulation data to an output file (.uan)
#
#  Programmer: OSU GENETIS Team
#
#  Revision history:
#     07/13/23  Jason Yao, third pass over the loop for training purposes
#
#  Notes:
#     * vertical ruler at column 80
#
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
NSECTIONS=${10}

# Lines for output.xmacro files 
# I've commented these out because we needed to put them inside of a loop in the
# macroskeleton 
# Currently these are hardcoded outputmacroskeleton_GPU.xmacro 
# line1='var query = new ResultQuery();'
# line2='///////////////////////Get Theta and Phi Gain///////////////'
# line3='query.projectId = App.getActiveProject().getProjectDirectory();'

# frequencies are scaled up by 100 to avoid float operation errors in bash
freqlist=\
"8333 10000 11667 13333 15000 16667 18334 20000 21667 23334 25000 26667 28334 
30000 31667 33334 35000 36667 38334 40001 41667 43334 45001 46667 48334 50001 
51668 53334 55001 56668 58334 60001 61668 63334 65001 66668 68335 70001 71668 
73335 75001 76668 78335 80001 81668 83335 85002 86668 88335 90002 91668 93335 
95002 96668 98335 100000 101670 103340 105000 106670"

# We need the Loop to pause until all the XF jobs are done
# To do this, we'll just count the flag files
cd $WorkingDir/Run_Outputs/$RunName/GPUFlags/
flag_files=$(ls | wc -l) 
while [[ $flag_files -lt $NPOP ]] 
do
  sleep 1m
  echo $flag_files
  flag_files=$(ls | wc -l) #$(ls -l --file-type | grep -v '/$' | wc -l)
done

rm -f $WorkingDir/Run_Outputs/$RunName/GPUFlags/*
echo $flag_files
echo "Done!"

# First, empty the file without chaning permission
cd $XmacrosDir
> output.xmacro

# echo "var m = $i;" >> output.xmacro
echo "var NPOP = $NPOP;" >> output.xmacro
echo "for (var k = $(($gen*$NPOP + 1)); k <= $(($gen*$NPOP+$NPOP)); k++){" \
      >> output.xmacro

if [ $NSECTIONS -eq 1 ] # if 1, then the cone is symmetric
then
  cat shortened_outputmacroskeleton.txt >> output.xmacro
else
  cat shortened_outputmacroskeleton_Asym.txt >> output.xmacro
fi

# use | as delimiter for sed sice WorkingDir contains /
# (any symbol following "s" can be used as the delimiter)
sed -i "" "s|fileDirectory|${WorkingDir}|" output.xmacro

module load xfdtd/7.9.2.2
xfdtd $XFProj \
  --execute-macro-script=$XmacrosDir/output.xmacro || true --splash=false
#Xvnc :5 &  DISPLAY=:5 xfdtd $XFProj --execute-macro-script=$XmacrosDir/simulation_PEC.xmacro || true

cd $WorkingDir/Antenna_Performance_Metric
for i in `seq $(($gen*$NPOP + $indiv)) $(($gen*$NPOP + $NPOP))`; do
  pop_ind_num=$(($i - $gen*$NPOP))
  for freq in `seq 1 60`;do

    desti="$WorkingDir/Run_Outputs/$RunName/uan_files/${gen}_uan_files/"
    nation="${pop_ind_num}/${gen}_${pop_ind_num}_${freq}.uan"

    mkdir -p $desti${pop_ind_num}
    mv ${gen}_${pop_ind_num}_${freq}.uan $desti$nation
      
  done
done
