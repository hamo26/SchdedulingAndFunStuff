#! /bin/sh
cd cbsTest/
make clean all
make
cd ../avrora/bin/
sh runTest.sh
