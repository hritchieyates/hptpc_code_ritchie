#!/bin/bash
# $1: file to process (- .root)
# $2: clean? (true/false)
# $3: global cut


loadCommand=".L plottingAndCleaning.C+"
cleanCommand="plottingAndCleaning("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/2018/11/raptorr_waveform-Analysis/${1}.root", "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/cleanData/${1}_${2}.root", "${3}", ${2} , "", "", "")"

eval $loadCommand
echo "$loadCommand"
eval $cleanCommand
echo "$cleanCommand"





haddCommand="hadd -f ./raptorr_waveform-Analysis/R${1}-to-R${2}_raptorrwaveformAnalysis_gain.root"
for i in $( seq -f %1.0f $1 $2 )
do 
        if [ ! -f "./raptorr_waveform-Analysis/raptorr_tpcdata_gain_R${i}.root" ]; 
        then 
                echo "File raptorr_tpcdata_gain_R${i}.root not found!"; 
        else
                haddCommand="$haddCommand ./raptorr_waveform-Analysis/raptorr_tpcdata_gain_R${i}.root"  
        fi
done
eval $haddCommand
echo "$haddCommand"
echo "./raptorr_waveform-Analysis/R${1}-to-R${2}_raptorrwaveformAnalysis_gain.root"
