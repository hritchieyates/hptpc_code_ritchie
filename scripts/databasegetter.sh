#1 start time to be included in the query, e.g. "2018-11-25 23:59:59"
#2 end time
#mysqlDataSet=$1
startTime=$1
endTime=$2
startTime="${startTime/ /-}"
startTime="${startTime//:/-}"
endTime="${startTime/ /-}"
endTime="${startTime//:/-}"
mysqlCommand="mysql -u dmatter -pseedark -h dmdbserv.pp.rhul.ac.uk -D GOOD_RUNS -e 'use GOOD_RUNS; select * from run_info WHERE start_time > \"$1\" AND start_time < \"$2\" ORDER BY run_info.run_number;'"
echo "$mysqlCommand"
eval "$mysqlCommand"
