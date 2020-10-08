#!/bin/bash
# $1: first run
# $2: last run

haddCommand="hadd -f ./raptorr_ccd-Analysis/R${1}-to-R${2}_raptorrccdAnalysis_gain.root"
for i in $( seq -f %1.0f $1 $2 )
do 
        if [ ! -f "./raptorr_ccd-Analysis/raptorr_tpcdata_gain_R${i}.root" ]; 
        then 
                echo "File raptorr_tpcdata_gain_R${i}.root not found!"; 
        else
                haddCommand="$haddCommand ./raptorr_ccd-Analysis/raptorr_tpcdata_gain_R${i}.root"  
        fi
done
eval $haddCommand
echo "$haddCommand"
echo "./raptorr_ccd-Analysis/R${1}-to-R${2}_raptorrccdAnalysis_gain.root"
