#!/bin/sh
cp ../thread/* ./
make clean
make
make install
cp ../epoll/* ./
make
make install
