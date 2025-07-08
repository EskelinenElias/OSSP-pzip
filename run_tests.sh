#! /bin/bash
make 

if ! [[ -x pzip ]]; then
    echo "pzip executable does not exist"
    exit 1
fi

echo "running tests..."

cp pzip tests/pzip


cd tests
./tester/run-tests.sh $*
cd ..