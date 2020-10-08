#!/bin/bash
# $1: file to process (- .root)
# $2: clean? (true/false)
# $3: global cut


root -l <<-EOF
.L plottingAndCleaning.C+
plottingAndCleaning("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/2018/11/raptorr_waveform-Analysis/${1}.root", "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/cleanData/${1}_${2}.root", "${3}", ${2} , "", "", "")

EOF
















