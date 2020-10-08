#$1: First Run
#$2: Last Run

#cleaning up
rm runInfo.sh

#Query the database and make a file with the result
mysqlCommand=$(mysql -udmatter -pseedark -hdmdbserv -A onlHP -e "select timeStart, timeStop, runId from run_end where runId>=${1} and runId<=${2} and timeStart>='2019-01-01 00:00:00'" >> runInfo.sh)
echo "$mysqlCommand"
eval "$mysqlCommand"

#get number of lines from file
numolines=$(< runInfo.sh wc -l)
echo $numolines
numolines2=`expr $numolines - 1`

for i in `seq 1 $numolines2`
do
    #Remove the first line
    awk 'NR == 1 {next} {print}' runInfo.sh > runInfoTemp.sh
    mv runInfoTemp.sh runInfo.sh
    #Separate rows and columns
    awk NR==1 runInfo.sh > runInfoLine.sh
    startYear=$(awk '{print $1}' runInfoLine.sh)
    startTime=$(awk '{print $2}' runInfoLine.sh)
    endYear=$(awk '{print $3}' runInfoLine.sh)
    endTime=$(awk '{print $4}' runInfoLine.sh)
    runNo=$(awk '{print $5}' runInfoLine.sh)
    
    #Print separate values (this is just for debugging)
    echo "start year = "
    echo $startYear
    echo "start time = "
    echo $startTime
    echo "end year = "
    echo $endYear
    echo "end time = "
    echo $endTime
    echo "run Number = "
    echo $runNo
 
    #clean up
    rm runInfoLine.sh
 
    #run make slow control plots exe
    mkdir ./slow_control_plots/R${1}_${2}
    dir="slow_control_plots/R${1}_${2}"
    slowCommand="./Make_slow_control_plots.exe '$startYear $startTime' '$endYear $endTime' '$runNo' './slow_control_plots/R${1}_${2}/Run_${runNo}_slow_control_plots'"
    eval $slowCommand
    echo "$slowCommand"

    #Set up values for plot over many runs
    if [ ${i} == 1 ];
    then
	firstStartYear=$startYear
	firstStartTime=$startTime
	firstRunNo=$runNo
    fi
    if [ ${i} == $numolines2 ];
    then
	lastEndYear=$endYear
	lastEndTime=$endTime
	lastRunNo=$runNo
    fi

done

    #run make slow control plots exe for plot over many runs
    slowCommandMulti="./Make_slow_control_plots.exe '$firstStartYear $firstStartTime' '$lastEndYear $lastEndTime' '$firstRunNo to $lastRunNo' './slow_control_plots/R${1}_${2}/Run_${firstRunNo}_to_${lastRunNo}_slow_control_plots'"
    echo "$slowCommandMulti"
    eval $slowCommandMulti
    echo "$slowCommandMulti"



#clean up
#rm runInfo.sh