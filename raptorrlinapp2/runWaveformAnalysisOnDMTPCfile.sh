#!/bin/bash
# $1: first run
# $2: last run
# $3: month
# $4: beam or gain

# make sure "previous run" is set ok and the month and beam / gain as well as the output dir are set correctly!
previousRun=2081070
previousMask='beam'
previousMonth="\/09\/"
previousYear='\/2018\/'

# further parameters
fileFlag="_Smooth_BLOsc"
#fileFlag="WaveformTest75to77_RMS015Inversec"
#cfgFileName="Waveform_testApp_dmtpc_input"
cfgFileName="Waveform_testApp_printWaveforms_dmtpc_input_old2_NoCuts"
#cfgFileName="WaveformAnalysis_printWaveforms_dmtpc_input2"
#testApp=Waveform_testApp
testApp=Waveform_testApp_printWaveforms
#testApp=WaveformAnalysis_printWaveforms
MONTH=$3
MASK=$4
YEAR=2020

# parameters related to the place the input file can be found at
inputFilePathDmtpcOld=/nfs/dbhome/jwalding/Dropbox/HPTPC/databackup/$YEAR/${MONTH}/raw/
#inputFilePathDmtpcNew=/scratch0/hrichie/hptpc_ritchie/HPTPC/data/preampData/raw/
#inputFilePathDmtpcNew=/scratch0/hrichie/hptpc_ritchie/HPTPC/${YEAR}/${MONTH}/raw/
inputFilePathDmtpcNew=/hadoop/group/hptpc/data/${YEAR}/${MONTH}/raw/
#outputFilePathRaptorr=/scratch0/hrichie/hptpc_ritchie/HPTPC/data/preampData/raptorr_waveform-Analysis/
#outputFilePathRaptorr=/hadoop/group/hptpc/data/${YEAR}/${MONTH}/raptorr_waveform-Analysis/
outputFilePathRaptorr=/scratch0/hrichie/hptpc_ritchie/HPTPC/data/${YEAR}/${MONTH}/raptorr_waveform-Analysis/R${1}-R${2}/
#outputFilePathRaptorr=/scratch0/adeistin/code/HPTPC/raptorr/reduced_waveform_test/

mkdir -p $inputFilePathDmtpcNew
mkdir -p /scratch0/hrichie/hptpc_ritchie/HPTPC/data/${YEAR}
mkdir -p /scratch0/hrichie/hptpc_ritchie/HPTPC/data/${YEAR}/${MONTH}
mkdir -p /scratch0/hrichie/hptpc_ritchie/HPTPC/data/${YEAR}/${MONTH}/raptorr_waveform-Analysis
mkdir -p $outputFilePathRaptorr

# external run list
#runListFile="CERN_beam_test_runs.txt"
#runListFile="CERN_beam_test_runs_with_spark_flag_ignored.txt"
#runListFile="RHUL_gain_runs_june_2019.txt"
#runListFile="RHUL_gain_runs_aug_2019_04gig.txt"
#runListFile="RHUL_gain_runs_aug_2019_08gig.txt"
#runListFile="RHUL_gain_runs_aug_2019_12gig.txt"
#runListFile="failed_runs.txt"

# some more definitions for the job submittion to the farm
# i.e. the directory for temproary files
source /scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/setup_raptorr_LP2.sh
#source /scratch0/adeistin/code/HPTPC/raptorr/setup_raptorr.sh
tmpDir=/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/tmp/
logDir=/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/tmp/
errDir=/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/tmp/
stdout="-o ${logDir}"
stderr="-e ${errDir}"

# create directories
mkdir -p $tmpDir
mkdir -p $logDir
mkdir -p $errDir

# run loop
#for run in 1319087 1319088 ...;
#for run in $(cat $runListFile);
for run in $( seq -f %1.0f $1 $2 )
do
        cfgFileNameSubmit="${cfgFileName}_${MONTH}_${run}_${MASK}${fileFlag}.cfg"
	if [ ! -f "${inputFilePathDmtpcNew}/hptpc_${MASK}_R${run}.raw.root" ];
	then
		echo " file ${inputFilePathDmtpcNew}/hptpc_${MASK}_R${run}.raw.root not found, checking the dropbox "
		if [ ! -f "${inputFilePathDmtpcOld}/hptpc_${MASK}_R${run}.raw.root" ]; 
		then 
			echo "File ${inputFilePathDmtpcOld}/hptpc_${MASK}_R${run}.raw.root not found!"; 
			continue;
		else
			cp ${inputFilePathDmtpcOld}/hptpc_${MASK}_R${run}.raw.root ${inputFilePathDmtpcNew}/.
			echo " copied file, going on ";
		fi
	fi
	# check if the file exists after the above iteration, in case not: 
	if [ ! -f "${inputFilePathDmtpcNew}/hptpc_${MASK}_R${run}.raw.root" ];
	then
		continue;
	else
		# To prevent that bad things happen, we delet the config file in case it already exists
		if [ -f ${tmpDir}/${cfgFileNameSubmit} ];
		then
			echo " delete config file and create a new one"
			rm ${tmpDir}/${cfgFileNameSubmit}
		fi
		# create and adjust the config file
		sed "s/${previousRun}/${run}/g" /scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/src/exe/${cfgFileName}.cfg > ${tmpDir}/${cfgFileNameSubmit}
		sed -i "s/_${previousMask}_/_${MASK}_/g" ${tmpDir}/${cfgFileNameSubmit}
		sed -i "s/${previousMonth}/\/${MONTH}\//g" ${tmpDir}/${cfgFileNameSubmit}
		sed -i "s/${previousYear}/\/${YEAR}\//g" ${tmpDir}/${cfgFileNameSubmit}


		# create run bash script
		runFileName="${tmpDir}/wa_${MONTH}_${run}_${MASK}${fileFlag}.sh"
		# delete the run file in case it already exists
		if [ -f $runFileName ];
		then
			echo " delete run file and create a new one"
			rm $runFileName
		fi
		echo "#!/bin/bash" >> $runFileName
		#echo "set umask 002" >> $runFileName
		echo "source /scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/setup_raptorr_LP2.sh" >> $runFileName
		# ... add a directory on the working node and enter it ...
		echo "export WORKDIR=/data/hrichie/\$PBS_JOBID" >> $runFileName
		echo "mkdir -p \$WORKDIR" >> $runFileName
		echo "cd \$WORKDIR" >> $runFileName
		# create the raptorr command to be run
		anaCommand="/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/build/bin/${testApp} -c ${tmpDir}/${cfgFileNameSubmit}  -n 100 -o \$WORKDIR/raptorr_tpcdata_${MASK}_R${run}${fileFlag}.root"
		# ... run the command on the working node
		echo "$anaCommand" >> $runFileName
		# ... copy the output file to its destination
		#echo "cp raptorr_tpcdata_${MASK}_R${run}${fileFlag}.root ${outputFilePathRaptorr}/." >> $runFileName
		echo "cp *.root ${outputFilePathRaptorr}/." >> $runFileName
		# ... leave and delete the working directory ...
		echo "cd .. " >> $runFileName
		echo "echo \"j\'ai fini\"" >> $runFileName
		echo "rm -rf \$WORKDIR" >> $runFileName
		chmod 744 $runFileName
		qsubmit="qsub"
		qsubopt="-q medium -l pmem=4gb,pvmem=4gb"
		qsubmit="${qsubmit} ${qsubopt} ${stdout} ${stderr} ${runFileName}"
		#eval $anaCommand
		eval $qsubmit
		echo "Job for run $run submitted"
		#$runFileName
	fi
done
echo "done!"
