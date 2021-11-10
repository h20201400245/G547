# G547
DEVICE DRIVERS REPOSITORY

Download the main.c and Makefile
Go to the directory in which code is downloaded and enter the command make
Insert the module using sudo insmod main.ko and check if module is loaded using the command lsmod 
Check the partition information using cat /proc/partitions and also check using ls -l /dev/main*
Take root access using sudo -s command
Write into disk use cat > /dev/main , type something & press enter to read back from the disk on command line use command xxd /dev/main | less
Remove the module using sudo rmmod main.ko
