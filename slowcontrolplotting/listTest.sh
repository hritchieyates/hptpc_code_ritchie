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

sed -n '1,2p' somefile2.sh > somefile4.sh
#for i in `cat`; do echo $i; done < somefile2.sh


#lname=$(cut -d \  -f 1 $1)
#fname=$(cut -d \  -f 2 $oneLine)
#mname=$(cut -d \  -f 3 $oneLine)




#echo $lname
#echo $fname
#echo $mname