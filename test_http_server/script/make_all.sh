#!/bin/sh
cd ../depend/epoll
cd ../makedir
./link.sh
cd ../../src
make clean
make
cd ../depend/makedir
rm Makefile
rm *.o
rm *.a
rm *.so
rm *.h
rm *.cpp
rm *.md
