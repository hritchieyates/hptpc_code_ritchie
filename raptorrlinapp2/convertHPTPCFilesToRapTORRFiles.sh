#!/bin/bash
# $1: first run
# $2: last run
# $3: month
# $4: beam or gain

# make sure "previous run" is set ok and the month and beam / gain as well as the output dir are set correctly!
previousRun=1354002
previousMask='beam'
previousMonth='\/11\/'
previousYear='\/2018\/'

# further parameters
cfgFileName="testapp_tpcreadout_dmtpcfile"
fileFlag=""
testApp="testAppTPCReadout"
MONTH=$3
MASK=$4
YEAR=2019

# parameters related to the place the input file can be found at
inputFilePathDmtpcOld=/nfs/dbhome/jwalding/Dropbox/HPTPC/databackup/$YEAR/${MONTH}/raw/
#inputFilePathDmtpcNew=/hadoop/group/hptpc/data/tpcdatabackup/${YEAR}/${MONTH}/raw/
inputFilePathDmtpcNew=/hadoop/group/hptpc/data/$YEAR/${MONTH}/raw
#outputFilePathRaptorr=/hadoop/group/hptpc/data/${YEAR}/${MONTH}/raptorr_raw/
#outputFilePathRaptorr=/scratch0/adeistin/code/HPTPC/raptorr/reduced_waveform_test
outputFilePathRaptorr=/hadoop/group/hptpc/data/$YEAR/${MONTH}/raptorr_raw

# external run list
#runListFile="CERN_beam_test_runs.txt"
#runListFile="CERN_beam_test_runs_with_spark_flag_ignored.txt"
#runListFile="RHUL_gain_runs_june_2019.txt"
#runListFile="RHUL_gain_runs_aug_2019_04gig.txt"
#runListFile="RHUL_gain_runs_aug_2019_08gig.txt"
#runListFile="RHUL_gain_runs_aug_2019_12gig.txt"
#runListFile="RHUL_gain_runs_aug_2019_missing_runs.txt"
runListFile="RHUL_gain_runs_aug_2019_all.txt"
#runListFile="failed_runs.txt"

# some more definitions for the job submittion to the farm
# i.e. the directory for temproary files
source /scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2new/setup_raptorr_LP2.sh
tmpDir=/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2new/raptorr/tmp_co/
logDir=/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2new/raptorr/tmp_co/
errDir=/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2new/raptorr/tmp_co/
stdout="-o ${logDir}"
stderr="-e ${errDir}"

mkdir -p $tmpDir
mkdir -p $logDir
mkdir -p $errDir

# run loop
#for run in 1319087 1319088 ...;
for run in $( seq -f %1.0f $1 $2 );
#for run in $(cat $runListFile);
do 
	cfgFileNameSubmit="${cfgFileName}_${MONTH}_${run}_${MASK}${fileFlag}.cfg"
	dmtpcInputFileName="hptpc_${MASK}_R${run}.raw.root"
	raptorrOutputFileName="raptorr_tpcdata_${MASK}_R${run}${fileFlag}.root"
	if [ ! -f "${inputFilePathDmtpcNew}/${dmtpcInputFileName}" ];
	then
		echo " file ${inputFilePathDmtpcNew}/${dmtpcInputFileName} not found, checking the dropbox "
		if [ ! -f "${inputFilePathDmtpcOld}/${dmtpcInputFileName}" ]; 
		then 
			echo "File ${inputFilePathDmtpcOld}/${dmtpcInputFileName} not found!"; 
			continue;
		else
			cp ${inputFilePathDmtpcOld}/${dmtpcInputFileName} ${inputFilePathDmtpcNew}/.
			echo " copied file, going on ";
		fi
	fi
	# check if the file exists after the above iteration, in case not: 
	if [ ! -f "${inputFilePathDmtpcNew}/${dmtpcInputFileName}" ];
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
                # this is not really used anymore, except for us needing the config file in a different place than its original one
                sed "s/${previousRun}/${run}/g" /scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2new/raptorr/src/exe/${cfgFileName}.cfg > ${tmpDir}/${cfgFileNameSubmit}
                sed -i "s/_${previousMask}_/_${MASK}_/g" ${tmpDir}/${cfgFileNameSubmit}
                sed -i "s/${previousMonth}/\/${MONTH}\//g" ${tmpDir}/${cfgFileNameSubmit}
                sed -i "s/${previousYear}/\/${YEAR}\//g" ${tmpDir}/${cfgFileNameSubmit}
		# create run bash script
		runFileName="${tmpDir}/co_${MONTH}_${run}_${MASK}${fileFlag}.sh"
		# delete the run file in case it already exists
		if [ -f $runFileName ];
		then
			echo " delete run file and create a new one"
			rm $runFileName
		fi
		echo "#!/bin/bash" >> $runFileName
		#echo "set umask 002" >> $runFileName
		echo "source /scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2new/setup_raptorr_LP2.sh" >> $runFileName
		# ... add a directory on the working node and enter it ...
		echo "export WORKDIR=/data/hrichie/\$PBS_JOBID" >> $runFileName
		echo "mkdir -p \$WORKDIR" >> $runFileName
		echo "cd \$WORKDIR" >> $runFileName
		# create the raptorr command to be run
		anaCommand="/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2new/raptorr/build/bin/${testApp} -c ${tmpDir}/${cfgFileNameSubmit} -n 1000 -o \$WORKDIR/${raptorrOutputFileName} -t ${inputFilePathDmtpcNew}/${dmtpcInputFileName}"
		# ... run the command on the working node
		echo "$anaCommand" >> $runFileName
		# ... copy the output file to its destination
		#echo "cp ${raptorrOutputFileName} ${outputFilePathRaptorr}/." >> $runFileName
		echo "cp *.root ${outputFilePathRaptorr}/." >> $runFileName
		# ... leave and delete the working directory ...
		echo "cd .. " >> $runFileName
		echo "echo \"j\'ai fini\"" >> $runFileName
		echo "rm -rf \$WORKDIR" >> $runFileName
		chmod 744 $runFileName
		qsubmit="qsub"
		qsubopt="-q medium -l  pmem=8gb,pvmem=8gb"
		qsubmit="${qsubmit} ${qsubopt} ${stdout} ${stderr} ${runFileName}"
		#eval $anaCommand
		eval $qsubmit
		#echo $qsubmit
		echo "Job for run $run submitted"
	fi
done
echo "done!"
