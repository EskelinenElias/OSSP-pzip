#! /bin/bash
make 

if ! [[ -x pzip_asan ]]; then
    echo "pzip_asan executable does not exist"
    exit 1
fi

echo "running tests..."

cp pzip_asan tests/pzip


cd tests
./tester/run-tests.sh $*
cd ..