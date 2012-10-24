make clean
make
avr-objdump -zhD main.elf > main.od
java -jar ~/workplace/494/hw2/avrora-beta-1.6.0.jar -platform=mica2 ./main.od
