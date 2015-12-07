#!/bin/sh

PWD = `pwd`
cd depends/net/
echo "Enter directory $PWD/depends/net/"
./make -a make && ./make install && ./make -a clean
echo "Leaving directory $PWD/depends/net/"

cd test/
echo "Enter directory $PWD/depends/net/test/"
./make -a make #&& ./make install && ./make -a clean
echo "Leaving directory $PWD/depends/net/test/"

cd ../../fs
echo "Enter directory $PWD/depends/fs/"
./make -a make && ./make install && ./make -a clean
echo "Leaving directory $PWD/depends/fs/"

cd ../../utils 
echo "Enter directory $PWD/depends/utils/"
./make -a make && ./make install && ./make -a clean
echo "Leaving directory $PWD/depends/utils/"

cd ../../app
echo "Enter directory $PWD/app"
./make -a make
echo "Leaving directory $PWD/app"

cd ../simple_rsync
echo "Enter directory $PWD/simple_rsync"
./make -a make
echo "Leaving directory $PWD/simple_rsync"

