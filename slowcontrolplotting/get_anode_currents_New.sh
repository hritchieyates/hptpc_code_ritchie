#$1: Input file

#cat somefile.sh
#$names=$(cat somefile.sh)
#head -n 5 $1 | tail -1
#sed '6q;d' $1
#oneLine="awk 'NR==5' $1"
#eval $oneLine


#x=0
#want=5
#cat somefile.sh | while read line; do
#  x=$(( x+1 ))
#  if [ $x -eq "$want" ]; then
#    echo $line
#    break
#  fi
#done


#arguments=`awk '{a = $1 " " a} END {print a}' somefile.sh`


#while IFS= read -r LINE; do
 #   "$LINE" >> somefile3.sh
#done < somefile2.sh


#xargs -l echo >> somefile3.sh < somefile2.sh

for i in 1
do
    tailNo=`expr $i + 1`
    echo $tailNo
    echo $i
    awk NR==1 $1 > somefile7.sh
    #awk 'NR=="$i"' > somefile6.sh
    #head -n $i $1 | tail $tailNo > somefile6.sh
   # sed -n "${tailNo}, ${i}p" < $1 > somefile6.sh
    #awk -v colno=$i 'NR==colno' > somefile6.sh
#    awk NR==1 somefile6.sh > somefile7.sh 
    startYear=$(cut -d " "  -f 1 somefile7.sh) 
    startTime=$(cut -d " "  -f 2 somefile7.sh)
    endYear=$(cut -d " "  -f 7 somefile7.sh)
    endTime=$(cut -d " "  -f 8 somefile7.sh)
    runNo=$(cut -d " "  -f 13 somefile7.sh)
    
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
    #rm somefile6.sh
    #rm somefile7.sh




    dir="slow_control_plots"
    slowCommand="./Make_slow_control_plots.exe '$startYear $startTime' '$endYear $endTime' '$runNo' './slow_control_plots/Run_${runNo}_slow_control_pl\
ots'"
#    echo "./Make_slow_control_plots.exe '$1' '$2' '$3' './slow_control_plots/Run_${runNo}_slow_control_plots'"
#echo "./slow_control_plots/Run_${3}_slow_control_plots"                                                 

    eval $slowCommand
    echo "$slowCommand"




done
#for i in `cat`; do echo $i; done < somefile2.sh


#lname=$(cut -d \  -f 1 $1)
#fname=$(cut -d \  -f 2 $oneLine)
#mname=$(cut -d \  -f 3 $oneLine)




#echo $lname
#echo $fname
#echo $mname