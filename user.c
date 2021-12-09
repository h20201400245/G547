#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/ioctl.h>

#include "config.h"

int file_desc;

int led1_status(int file_desc, int16_t *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, LED1_STATUS,msg);
 return ret_val;
}

int led2_input(int file_desc, int16_t *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, LED2_INPUT,msg);
 return ret_val;
}



int main(void)
{
 int ret_val;
 int16_t led1_value,toggle_input,user_input;

 file_desc = open(DEVICE_FILE_NAME,0);

 if(file_desc<0)
 {
  printf("Device Open Failed for %s\n",DEVICE_FILE_NAME);
  exit(-1);
 }

 while(1)
 {

  led2_input(file_desc,&toggle_input);
  if(toggle_input)
  {
  printf("********************************************** \n");
   printf("touch input detected \n");
  led1_status(file_desc,&led1_value);
  printf("WHITE LED status(toggling):%d \n",led1_value);
  printf("ENTER THE RED LED status:");
  scanf("%d",&user_input);
  ioctl(file_desc, LED2_USER_INPUT,(int16_t*)&user_input);
  }
}
 
}
