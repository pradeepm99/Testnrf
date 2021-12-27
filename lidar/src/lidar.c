#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/sensor.h>
#include <sys/__assert.h>

extern int lidarInit(struct device *dev);
extern int getLidarIntensity(struct device *dev);
extern void setLidar(int lidar, int intensity, int *sensor_signal);

int setBit(int val, int pos);
int getIntensity(float dist);

#define LIDAR_1 1
#define LIDAR_2 2
#define BIT_0 0 // LIDAR 1
#define BIT_1 1 // LIDAR 1 intensity bit 2
#define BIT_2 2 // LIDAR 1 intensity bit 1
#define BIT_3 3 // LIDAR 1 intensity bit 0
#define BIT_4 4 // LIDAR 2 
#define BIT_5 5 // LIDAR 2 intensity bit 2
#define BIT_6 6 // LIDAR 2 intensity bit 1
#define BIT_7 7 // LIDAR 2 intensity bit 0
#define LIDAR_BAUD_115200 2 


int lidarInit(struct device *dev) {

    struct sensor_value offset_value;
    dev = device_get_binding(DT_LABEL(DT_INST(LIDAR_BAUD_115200, null)));
	__ASSERT(dev != NULL, "Failed to get binding");

	printk("Device %s - %p is ready\n", dev->name, dev);

	offset_value.val1 = 0;
	offset_value.val2 = 0;

	int ret = sensor_attr_set(dev, SENSOR_CHAN_DISTANCE, SENSOR_ATTR_OFFSET, &offset_value);

	if (ret) {
		printk("sensor_attr_set failed ret = %d\n", ret);
        return -1;
	}
    return 0;
}

int getLidarIntensity(struct device *dev) {

    struct sensor_value dist;
    int ret =  sensor_channel_get(dev, SENSOR_CHAN_DISTANCE, &dist);
	if (ret) {
		printk("Failed to get measurements (%d)\n", ret);
		return;
	}

	int intensity = getIntensity(dist.val1 + dist.val2*10^-6);
	printk("Distance is %d.%d meters\n", dist.val1, dist.val2);
    return intensity;
}

int getIntensity(float dist) {
	int intensity;
	if(dist >=0 || dist <1) {
		intensity = 0b111;
	}
	else if(dist >=1 || dist <3) {
		intensity = 0b110;
	}
	else if(dist >=3 || dist <4) {
		intensity = 0b101;
	}
	else if(dist >=4 || dist <5) {
		intensity = 0b100;
	}
	else if(dist >=5 || dist <6) {
		intensity = 0b011;
	}
	else if(dist >=6 || dist <7) {
		intensity = 0b010;
	}
	else if(dist >=7 || dist <8) {
		intensity = 0b001;
	}
	else {
		intensity = 0b000;
	}
	return intensity;
}

void setLidar(int lidar, int intensity, int *sensor_signal) {

    switch(lidar) {
        case LIDAR_1:
            setBit(*sensor_signal, BIT_0);
            getBit(intensity, BIT_0);
            setBit(*sensor_signal, BIT_3);        
            getBit(intensity, BIT_1);
            setBit(*sensor_signal, BIT_2);
            getBit(intensity, BIT_2);
            setBit(*sensor_signal, BIT_1); 
            break;

        case LIDAR_2:
            setBit(*sensor_signal, BIT_4);
            getBit(intensity, BIT_0);
            setBit(*sensor_signal, BIT_7);
            getBit(intensity, BIT_1);
            setBit(*sensor_signal, BIT_6);
            getBit(intensity, BIT_2);
            setBit(*sensor_signal, BIT_5);
            break;

        default:
            printk("Invalid lidar number");
    }

}

