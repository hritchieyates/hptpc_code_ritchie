#!/bin/bash
#$1: global cut
#$2: output name
#$3: Scheme
#$4: Clean "_anode3" or ""

listToLookAt=runsToPlotIndivLists$3Mod.sh
filePrefix="raptorr_tpcdata_gain_"
fileSuffix="_Am241"
filePath="/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal"
#filePath="/scratch4/adeistin/2020/03/raptorr_waveform-Analysis/"

#dir=$3
#filePath="${filePath}/${dir}"

mv EntryCheckCommandList.C EntryCheckCommandListOld.C

cp ${listToLookAt} IndivLists.sh

#get number of lines from file                                                                                      
numolines=$(< IndivLists.sh wc -l)
echo $numolines
numolines2=`expr $numolines - 1`

cp IndivLists.sh IndivListsToUse.sh
echo "{" >> EntryCheckCommandList.C
echo "gROOT->ProcessLine(\".X setUp_bkgVsSignal_libs.C\");" >> EntryCheckCommandList.C

for a in `seq 1 $numolines`
do
    #Remove the first line                           
    awk 'NR == 1 {next} {print}' IndivListsToUse.sh > IndivListsTemp.sh
    mv IndivListsTemp.sh IndivListsToUse.sh
    

    #Separate rows and columns                                      
    awk NR==1 IndivListsToUse.sh > IndivListsLine.sh
    firstRun=$(awk '{print $1}' IndivListsLine.sh)
    lastRun=$(awk '{print $2}' IndivListsLine.sh)
    echo "first run = "
    echo $firstRun
    echo "last run = "
    echo $lastRun

    rm IndivListsLine.sh


    #Make the bkgVsSignal command and print it to file
    EntryCheckCommand="gROOT->ProcessLine(\"compareDifferentRuns(\\\"${1}\\\",\\\"${2}\\\", ${firstRun}, ${lastRun}, \\\"${4}\\\")\");"

    echo $EntryCheckCommand
    echo $EntryCheckCommand >> EntryCheckCommandList.C
done


echo "gROOT->ProcessLine(\".L plotNoEntries.C\");" >> EntryCheckCommandList.C
echo "gROOT->ProcessLine(\".L plotNoEntries.C\");"
echo "gROOT->ProcessLine(\"plotNoEntries(\\\"${2}\\\", \\\"${3}\\\")\");"
echo "gROOT->ProcessLine(\"plotNoEntries(\\\"${2}\\\", \\\"${3}\\\")\");" >> EntryCheckCommandList.C
echo "gROOT->ProcessLine(\".q\");"
echo "gROOT->ProcessLine(\".q\");" >> EntryCheckCommandList.C
echo "}" >> EntryCheckCommandList.C

root -l EntryCheckCommandList.C
#clean up
rm IndivListsToUse.sh

















