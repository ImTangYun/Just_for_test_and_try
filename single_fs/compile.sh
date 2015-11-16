#!/bin/sh

cd depends/net/
./make -a make && ./make install && ./make -a clean

cd ../fs
./make -a make && ./make install && ./make -a clean

cd ../../app

./make -a make
