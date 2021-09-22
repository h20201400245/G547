#include "chardev.h"

#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#define DEVICE_FILE_NAME "/dev/imu_char"
/////////////////////////////////////////////////////////////////

/* ioctl call 1*/

int gy_xaxis(int file_desc)
{
int ret_val=0;
int message=0;

ret_val=ioctl(file_desc, GY_XAXIS,message);

if(ret_val==0)
{
printf("gyro failed: %i\n",ret_val);
exit(-1);
}
printf("Reading in X axis in gyro: %d\n",ret_val);
return 0;
}
/* ioctl call 2*/
int gy_yaxis(int file_desc)
{
int ret_val;
char message[100];

ret_val=ioctl(file_desc, GY_YAXIS,message);

if(ret_val==0)
{
printf("gyro failed: %i\n",ret_val);
exit(-1);
}

printf("Reading in y axis in gyro: %d\n",ret_val);
return 0;
}

/*ioctl call 3*/
int gy_zaxis(int file_desc)
{
int ret_val;
char message[100];

ret_val=ioctl(file_desc, GY_ZAXIS,message);

if(ret_val<0)
{
printf("gyro failed: %d\n",ret_val);
exit(-1);
}

printf("Reading in z axis in gyro %d\n",ret_val);
return 0;
}


/*ioctl call 4*/
int acc_xaxis(int file_desc)
{
int ret_val;
char message[100];

ret_val=ioctl(file_desc, ACC_XAXIS,message);

if(ret_val<0)
{
printf("ACC failed: %d\n",ret_val);
exit(-1);
}

printf("Reading in X axis in acc %d\n",ret_val);
return 0;
}

/*ioctl call 5*/
int acc_yaxis(int file_desc)
{
int ret_val;
char message[100];
ret_val=ioctl(file_desc, ACC_YAXIS,message);

if(ret_val<0)
{
printf("ACC failed: %d\n",ret_val);
exit(-1);
}
printf("Reading in y axis in acc %d\n",ret_val);
return 0;
}


/*ioctl call 6*/
int acc_zaxis(int file_desc)
{
int ret_val;
char message[100];
ret_val=ioctl(file_desc,ACC_ZAXIS,message);
if(ret_val<0)
{
printf("ACC failed: %d\n",ret_val);
exit(-1);
}
printf("Reading in Z axis in acc %d\n",ret_val);
return 0;
}

/*ioctl call 7*/
int mag_xaxis(int file_desc)
{
int ret_val;
char message[100];
ret_val=ioctl(file_desc, MAG_XAXIS,message);
if(ret_val<0)
{
printf("MAG failed: %d\n",ret_val);
exit(-1);
}
printf("Reading in X axis in mag %d\n",ret_val);
return 0;
}

/*ioctl call 8*/

int mag_yaxis(int file_desc)
{
 int ret_val;
char message[100];
ret_val=ioctl(file_desc, MAG_YAXIS,message);
if(ret_val<0)
{
printf("MAG failed: %d\n",ret_val);
exit(-1);
}
printf("Reading in Y axis in mag %d\n",ret_val);
return 0;
}

/*ioctl call 9*/
int mag_zaxis(int file_desc)
{
int ret_val;
char message[100];
ret_val=ioctl(file_desc, MAG_ZAXIS,message);
if(ret_val<0)
{
printf("MAG failed: %d\n",ret_val);
exit(-1);
}
printf("Reading in Z axis in mag %d\n",ret_val);
return 0;
}

/*ioctl call 10*/
int bar_pressure(int file_desc,int req_val)
{
int ret_val;
char msg[100];
if (req_val==9)
ret_val=ioctl(file_desc, BAR_PRESSURE, msg);
if(ret_val<0)
{
printf("bar failed with req val: %d %d\n",ret_val,req_val);
exit(-1);
}
//printf("call 10: %d\n",ret_val);
printf("Reading in bar %d\n",ret_val);
return 0;
}
/////////////////////////////////////////////////////////////////
int main()
{
int file_desc, ret_val;
char *msg="This is user space main program\n";

file_desc=open(DEVICE_FILE_NAME,O_RDONLY);
printf("%s returns %d\n",DEVICE_FILE_NAME,file_desc);
if (file_desc<0)
{
printf("Can't open device file %s\n", DEVICE_FILE_NAME);
exit(-1);
}

gy_xaxis(file_desc);
gy_yaxis(file_desc);
gy_zaxis(file_desc);

acc_xaxis(file_desc);
acc_yaxis(file_desc);
acc_zaxis(file_desc);

mag_xaxis(file_desc);
mag_yaxis(file_desc);
mag_zaxis(file_desc);

bar_pressure(file_desc,9);

close(file_desc);
return 0;
}
////////////////////////////////////////////////////////////////
