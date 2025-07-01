#! /bin/bash

if ! [[ -x pzip ]]; then
    echo "pzip executable does not exist"
    exit 1
fi

cp pzip tests/pzip


cd tests
./tester/run-tests.sh $*
cd ..

rm tests/pzip