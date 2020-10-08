#$1: First Run
#$2: Last Run

#cleaning up
rm somefile.sh

#Query the database and make a file with the result
mysqlCommand=$(mysql -udmatter -pseedark -hdmdbserv -A onlHP -e "select cathodeVoltage, timeStop, runId from run_end where runId>=${1} and runId<=${2} and timeStart<='2019-01-01 00:00:00'" >> somefile.sh)
echo "$mysqlCommand"
eval "$mysqlCommand"
