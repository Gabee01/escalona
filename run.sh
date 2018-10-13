#!/bin/sh
make clean
make
./escalona < t2.in > t2.out
cat t2.out
