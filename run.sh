#!/bin/sh
make clean
make
./escalona < t2.in > t2.out
diff t2.sol t2.out
