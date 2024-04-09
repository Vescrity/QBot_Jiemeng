startTime=`date +%Y%m%d-%H:%M:%S`
startTime_s=`date +%s`

cd test_runspace
cmake ..
make

endTime=`date +%Y%m%d-%H:%M:%S`
endTime_s=`date +%s`

sumTime=$[ $endTime_s - $startTime_s ]

echo "$startTime ---> $endTime" 
echo "本次编译耗时: $sumTime seconds"