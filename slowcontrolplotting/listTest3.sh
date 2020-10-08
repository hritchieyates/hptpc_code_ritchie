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

#for i in 1 2 3 4 5
#do
   # tailNo=`expr $i + 1`
  #  echo $tailNo
 #   echo $i
    #awk NR==1 $1 > somefile6.sh
    #awk 'NR=="$i"' > somefile6.sh
    #head -n $i $1 | tail $tailNo > somefile6.sh
#    sed -n "${tailNo}, ${i}p" < $1 > somefile6.sh
    #awk -v colno=$i 'NR==colno' > somefile6.sh
#    awk NR==1 somefile6.sh > somefile7.sh 
    startYear=$(cut -d " "  -f 1 $1) 
    startTime=$(cut -d " "  -f 2 $1)
    endYear=$(cut -d " "  -f 3 $1)
    endTime=$(cut -d " "  -f 4 $1)
    runNo=$(cut -d " "  -f 5 $1)
    
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
#done
#for i in `cat`; do echo $i; done < somefile2.sh


#lname=$(cut -d \  -f 1 $1)
#fname=$(cut -d \  -f 2 $oneLine)
#mname=$(cut -d \  -f 3 $oneLine)




#echo $lname
#echo $fname
#echo $mname