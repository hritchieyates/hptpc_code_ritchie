#!/bin/bash
# $1: first run
# $2: last run

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
