#! /bin/sh
cd srpTest/
make clean all
make
cd ../avrora/bin/
sh runTest.sh
