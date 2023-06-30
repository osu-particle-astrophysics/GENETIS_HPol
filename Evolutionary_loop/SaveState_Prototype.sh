gen=$1
state=$2
runName=$3
indiv=$4
echo ${gen}   >  SaveStates/${runName}_SaveState.txt
echo ${state} >> SaveStates/${runName}_SaveState.txt
echo ${indiv} >> SaveStates/${runName}_SaveState.txt
 
