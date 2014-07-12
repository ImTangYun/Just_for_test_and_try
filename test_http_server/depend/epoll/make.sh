#!/bin/sh
cd ../makedir
./link.sh
cd ../../src
make clean
make
