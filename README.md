# G547
DEVICE DRIVERS REPOSITORY

1.	Download the main.c and Makefile
2.	Go to the directory in which code is downloaded and enter the command make
3.	Insert the module using sudo insmod main.ko and check if module is loaded using the command lsmod 
4.	Check the partition information using cat /proc/partitions and also check using ls -l /dev/main*
5.	Take root access using sudo -s command
6.	Write into disk use cat > /dev/main , type something & press enter to read back from the disk on command line use command xxd /dev/main | less
7.	Remove the module using sudo rmmod main.ko

