#!/bin/sh
cd ../depend/epoll
cd ../makedir
#./link.sh
cp ../thread/* ./                                                                                                 
make clean
make
cp ../epoll/* ./
make
make install

cd ../util
make clean
make
make install

cd ../../src
make clean
make
rm *.o
cd ../depend/makedir
rm Makefile
rm *.o
rm *.a
rm *.so
rm *.h
rm *.cpp
rm *.md
