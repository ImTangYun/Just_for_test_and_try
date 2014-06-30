#!/bin/sh
make clean
make
make install
cd ../../src/
make clean
make
