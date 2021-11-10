# G547
DEVICE DRIVERS REPOSITORY

1. Download the main.c and Makefile
2. Go to the directory in which code is downloaded and enter the command make
3. Enter the command sudo insmod main.ko to insert the object file into kernel
4. Check the partition information using cat /proc/partitions and also check using ls -l /dev/dof*
5. Take root access using sudo -s command
6. Write into disk use cat > /dev/dof1 and type something and press enter
7. Enter the command xxd /dev/dof1 | less to display the input which is stored in the partition we created
8. Remove the module from the kernel using sudo rmmod main.ko


