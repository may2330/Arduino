#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define buf_size 10

int main(int argc, char **argv)
{
    int fd;
    double T;
    int flag;
    int count;
    char buf[buf_size];
    int i;
    double temp;
    double cpu_freq;

    while(1)
    {
	memset(buf,0,buf_size);
	// RaspberryPi CPU Temp
        fd = open("/sys/class/thermal/thermal_zone0/temp",O_RDONLY);
        if(fd<2)
            printf("Can not open File\n");
 
        count = read(fd, buf, 5);	   
        //fscanf(fd,"%3f", &T);
        temp = atoi(buf);
        temp /= 1000;
        printf("The temperature is %6.3f C.\n", temp);
        close(fd);


/* 라즈베리파이

	// RaspberryPi CPU Current Clock freq
	fd = open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq",O_RDONLY);
	if(fd<2)
	    printf("Can not open File : scaling_cur_freq\n");
        count = read(fd, buf, 8);
        cpu_freq = atoi(buf);
	cpu_freq /= 1000; 
        printf("cpu_cur_freq = %6.3f MHz\n", cpu_freq);
        close(fd);

	// RaspberryPi CPU Min Clock freq
	fd = open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq",O_RDONLY);
	if(fd<2)
	    printf("Can not open File : scaling_cur_freq\n");
        count = read(fd, buf, 8);
        cpu_freq = atoi(buf);
	cpu_freq /= 1000; 
        printf("cpu_min_freq = %6.3f MHz\n", cpu_freq);
        close(fd);

	// RaspberryPi CPU Max Clock freq
	fd = open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq",O_RDONLY);
	if(fd<2)
	    printf("Can not open File : scaling_cur_freq\n");
        count = read(fd, buf, 8);
        cpu_freq = atoi(buf);
	cpu_freq /= 1000; 
        printf("cpu_max_freq = %6.3f MHz\n", cpu_freq);
        close(fd);
*/

        sleep(1);
    }
    return 0;
}
