#$1: First Run
#$2: Last Run

#cleaning up
rm somefile9.sh
rm somefile6.sh

#Query the database and make a file with the result
mysqlCommand=$(mysql -udmatter -pseedark -hdmdbserv -A onlHP -e "select timeStart, timeStop, runId from run_end where runId>=${1} and runId<=${2} and timeStart>='2019-01-01 00:00:00'" >> somefile9.sh)
echo "$mysqlCommand"
eval "$mysqlCommand"

cp somefile9.sh somefile6.sh

#get number of lines from file
numolines=$(< somefile6.sh wc -l)
echo $numolines
numolines2=`expr $numolines - 1`

for i in `seq 1 $numolines2`
do
    #Remove the first line
    awk 'NR == 1 {next} {print}' somefile6.sh > somefile8.sh
    mv somefile8.sh somefile6.sh
    #Separate rows and columns
    awk NR==1 somefile6.sh > somefile7.sh
    startYear=$(awk '{print $1}' somefile7.sh)
    startTime=$(awk '{print $2}' somefile7.sh)
    endYear=$(awk '{print $3}' somefile7.sh)
    endTime=$(awk '{print $4}' somefile7.sh)
    runNo=$(awk '{print $5}' somefile7.sh)
    
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
    rm somefile7.sh
 
    #run make slow control plots exe
    dir="slow_control_plots"
    slowCommand="./Make_slow_control_plots.exe '$startYear $startTime' '$endYear $endTime' '$runNo' './slow_control_plots/Run_${runNo}_slow_control_plots'"
    eval $slowCommand
    echo "$slowCommand"

done
#clean up
rm somefile6.sh
rm somefile9.sh