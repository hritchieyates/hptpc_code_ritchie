#!/bin/bash
# $1: clean? (true/false)
# $2: global cut

listToLookAt=runsToPlotSgl.sh
fileSuffix="_raptorr_tpcdata_gain_PeakTime2"


mv cleaningCommandList.sh cleaningCommandListOld.sh

cp ${listToLookAt} uncleanedFiles.sh

#get number of lines from file                                                                                      
numolines=$(< uncleanedFiles.sh wc -l)
echo $numolines
numolines2=`expr $numolines - 1`

cp uncleanedFiles.sh uncleanedFilesToUse.sh

for a in `seq 1 $numolines2`
do
    #Remove the first line                           
    awk 'NR == 1 {next} {print}' uncleanedFilesToUse.sh > uncleanedFilesTemp.sh
    mv uncleanedFilesTemp.sh uncleanedFilesToUse.sh
    
    #Separate rows                                      
    awk NR==1 uncleanedFilesToUse.sh > uncleanedFilesLine.sh
    i=$(awk '{print$1}' uncleanedFilesLine.sh)
    echo $i
    #Make the cleaning command and print it to file
  cleaningCommand="plottingAndCleaning(\"/scratch0/hrichie/hptpc_ritchie/HPTPC/data/2019/12/raptorr_waveform-Analysis/R1339001-R1341077/${i}${fileSuffix}.root\", \"/scratch0/hrichie/hptpc_ritchie/HPTPC/data/cleanData/${i}${fileSuffix}_${1}.root\", \"${2}\", ${1} , \"\", \"\", \"\")"
  echo $cleaningCommand
  echo $cleaningCommand >> cleaningCommandList.sh
done

#clean up
rm uncleanedFilesToUse.sh

















