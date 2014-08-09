#!/bin/sh
cp ../thread/* ./
make clean
make
cp ../epoll/* ./
make
make install
