#! /bin/sh
cd srp/
make clean all
make
cd ../avrora/bin/
sh runTest.sh
