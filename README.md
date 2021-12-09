# G547
DEVICE DRIVERS REPOSITORY

KERNEL SPACE DRIVER BUILD PROCESS :

•	Build the driver by using Makefile (sudo make)

•	Load the driver using sudo insmod driver.ko

•	Check whether module is inserter in kernel space with lsmod.

•	Unload the driver using sudo rmmod driver, after checking LEDs.

USER SPACE APPLICATION BUILD PROCESS :

•	Compile user application code with gcc -o output user.c

•	Run the application (sudo ./output) after inserting kernel driver module.
