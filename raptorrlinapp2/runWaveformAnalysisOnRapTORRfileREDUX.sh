#!/bin/bash
# $1: first run
# $2: last run
# $3: month
# $4: beam or gain

# make sure "previous run" is set ok and the month and beam / gain as well as the output dir are set correctly!
previousRun=1256073
previousMask='beam'
previousMonth="\/09\/"
previousYear='\/2018\/'

# further parameters
cfgFileName=Waveform_testApp
testApp=Waveform_testApp
MONTH=$3
MASK=$4
YEAR=2019

# parameters related to the place the input file can be found at
inputFilePathRaptorr=/hadoop/group/hptpc/data/${YEAR}/${MONTH}/raptorr_raw/
outputFilePathRaptorr=/hadoop/group/hptpc/data/${YEAR}/${MONTH}/raptorr_waveform-Analysis/

# external run list
#runListFile="CERN_beam_test_runs.txt"
#runListFile="CERN_beam_test_runs_with_spark_flag_ignored.txt"
#runListFile="RHUL_beam_runs_nov_2018.txt"
#runListFile="RHUL_gain_runs_dec_2018.txt"
#runListFile="RHUL_gain_runs_nov_2018.txt"
#runListFile="RHUL_neutron_runs_dec_2018.txt"
#runListFile="failed_runs.txt"
runListFile="RHUL_gain_runs_june_2019.txt"

# some more definitions for the job submittion to the farm
# i.e. the directory for temproary files
source /scratch0/adeistin/code/HPTPC/raptorr/setup_raptorr_LP2.sh
tmpDir=/scratch0/adeistin/code/HPTPC/raptorr/tmp/
logDir=/scratch0/adeistin/code/HPTPC/raptorr/tmp/
errDir=/scratch0/adeistin/code/HPTPC/raptorr/tmp/
stdout="-o ${logDir}"
stderr="-e ${errDir}"

# create directories
mkdir -p $tmpDir
mkdir -p $logDir
mkdir -p $errDir

# run loop
#for run in 1319087 1319088 ...;
#for run in $( seq -f %1.0f $1 $2 )
for run in $(cat $runListFile);
do
	if [ ! -f "${inputFilePathRaptorr}/raptorr_tpcdata_${MASK}_R${run}.root" ];
	then
		echo " file ${inputFilePathRaptorr}/raptorr_tpcdata_${MASK}_R${run}.root not found, call the week early! "
		continue;
	else
		# To prevent that bad things happen, we delet the config file in case it already exists
		if [ -f ${tmpDir}/${cfgFileName}_${MONTH}_${run}_${MASK}.cfg ];
		then
			echo " delete config file and create a new one"
			rm ${tmpDir}/${cfgFileName}_${MONTH}_${run}_${MASK}.cfg
		fi
		# create and adjust the config file
		sed "s/${previousRun}/${run}/g" /scratch0/adeistin/code/HPTPC/raptorr/src/exe/${cfgFileName}.cfg > ${tmpDir}/${cfgFileName}_${MONTH}_${run}_${MASK}.cfg
		sed -i "s/_${previousMask}_/_${MASK}_/g" ${tmpDir}/${cfgFileName}_${MONTH}_${run}_${MASK}.cfg
		sed -i "s/${previousMonth}/\/${MONTH}\//g" ${tmpDir}/${cfgFileName}_${MONTH}_${run}_${MASK}.cfg
		sed -i "s/${previousYear}/\/${YEAR}\//g" ${tmpDir}/${cfgFileName}_${MONTH}_${run}_${MASK}.cfg


		# create run bash script
		runFileName=${tmpDir}/wa_${MONTH}_${run}_${MASK}.sh
		# delete the run file in case it already exists
		if [ -f $runFileName ];
		then
			echo " delete run file and create a new one"
			rm $runFileName
		fi
		echo "#!/bin/bash" >> $runFileName
		#echo "set umask 002" >> $runFileName
		echo "source /scratch0/adeistin/code/HPTPC/raptorr/setup_raptorr_LP2.sh" >> $runFileName
		# ... add a directory on the working node and enter it ...
		echo "export WORKDIR=/data/adeistin/\$PBS_JOBID" >> $runFileName
		echo "mkdir -p \$WORKDIR" >> $runFileName
		echo "cd \$WORKDIR" >> $runFileName
		# create the raptorr command to be run
		anaCommand="/scratch0/adeistin/code/HPTPC/raptorr/build/bin/${testApp} -c ${tmpDir}/${cfgFileName}_${MONTH}_${run}_${MASK}.cfg -n 1000 -o \$WORKDIR/raptorr_tpcdata_${MASK}_R${run}_expSmooth.root"
		# ... run the command on the working node
		echo "$anaCommand" >> $runFileName
		# ... copy the output file to its destination
		echo "cp raptorr_tpcdata_${MASK}_R${run}_expSmooth.root ${outputFilePathRaptorr}/." >> $runFileName
		# ... leave and delete the working directory ...
		echo "cd .. " >> $runFileName
		echo "echo \"j\'ai fini\"" >> $runFileName
		echo "rm -rf \$WORKDIR" >> $runFileName
		chmod 744 $runFileName
		qsubmit="qsub"
		qsubopt="-q medium -l  pmem=4gb,pvmem=4gb"
		qsubmit="${qsubmit} ${qsubopt} ${stdout} ${stderr} ${runFileName}"
		#eval $anaCommand
		eval $qsubmit
		echo "Job for run $run submitted"
		#$runFileName
	fi
done
echo "done!"
