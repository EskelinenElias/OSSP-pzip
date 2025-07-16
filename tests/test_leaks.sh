export MallocStackLogging=1

./pzip leaks/big_data.in leaks/large_data.in leaks/big_data.in > leaks/leaks.out &
PID=$!
sleep 0.1
leaks $PID
wait $PID
