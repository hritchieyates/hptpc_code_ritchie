#1 mysql data set, e.g. `chamber_pressure_bpg`
#2 start time to be included in the query, e.g. "2018-11-25 23:59:59"
#mysqlDataSet=$1
startTime=$1
endTime=$2
startTime="${startTime/ /-}"
startTime="${startTime//:/-}"
endTime="${startTime/ /-}"
endTime="${startTime//:/-}"
mysqlCommand="mysql -u dmatter -pseedark -h dmdbserv.pp.rhul.ac.uk -D onlHP -e 'use onlHP; select * from run_end'"
echo "$mysqlCommand"
eval "$mysqlCommand"
