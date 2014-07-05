#!/bin/sh
cd ../depend/epoll
make clean
make
make install
cd ../config
make clean
make
make install

cd ../thread
make clean
make
make install

cd ../util
make clean
make
make install

cd ../../src
make clean
make
