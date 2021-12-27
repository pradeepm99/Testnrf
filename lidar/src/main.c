#include <device.h>


void main(void){

    const struct device *dev1, *dev2;

    int sensor_signal = 0b00000000;

    int retVal = lidarInit(dev1);
    int intensity1 = 0;
    int intensity2 = 0;

    while(1){
        intensity1 = getLidarIntensity(dev1);
        if(intensity1 > 0 || intensity2 > 0) {
            if(intensity1 > 0) {
                setLidar(1, intensity1, &sensor_signal);   
            }

            if(intensity2 > 0) {
                setLidar(2, intensity2, &sensor_signal);
            }

            // send to bluetooth;
        }
    }
}