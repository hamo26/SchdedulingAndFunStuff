#! /bin/sh
cd edfTest/
make clean all
make
cd ../avrora/bin/
sh runTest.sh
