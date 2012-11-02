#! /bin/sh
cd cashTest/
make clean all
make
cd ../avrora/bin/
sh runTest.sh
