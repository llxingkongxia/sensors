/* 
 * Copyright (C) 2015 ASUSTek Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
 
 /******************************/
/* Asus Sensor Hub Attribute */
/*****************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/input/ASH.h>

static struct device * g_sensordevice = NULL;
static struct class *g_property_class = NULL;

/**************************/
/* Debug and Log System */
/************************/
#define MODULE_NAME	"ASH_ATTR"

#undef dbg
#ifdef ASH_ATTR_DEBUG
	#define dbg(fmt, args...) printk(KERN_DEBUG "[%s]"fmt,MODULE_NAME,##args)
#else
	#define dbg(fmt, args...)
#endif
#define log(fmt, args...) printk(KERN_INFO "[%s]"fmt,MODULE_NAME,##args)
#define err(fmt, args...) printk(KERN_ERR "[%s]"fmt,MODULE_NAME,##args)

/*static int ASH_class_open(struct inode * inode, struct file * file)
{
    return 0;
}

static const struct file_operations ASH_class_fops = {
    .owner      = THIS_MODULE,
    .open       = ASH_class_open,
};*/

/*static int create_ASH_chrdev(void)
{
	
	g_devMajor = register_chrdev(0, "sensors", &ASH_class_fops);
	if (g_devMajor < 0) {
		err("%s: could not get major number\n", __FUNCTION__);
		return g_devMajor;
	}
	
	return 0;
}*/

static int create_ASH_class(void)
{
	/* create sys/class file node*/
	g_property_class = class_create(THIS_MODULE, "sensors");
	if (IS_ERR(g_property_class)) {
		err("%s: class_create ERROR.\n", __FUNCTION__);
		return PTR_ERR(g_property_class);
	}
	
	return 0;
}

/*
 Return NULL for error handling
*/
struct device *ASH_ATTR_device_create(ASH_type type)
{	
	int ret = 0;
	//dev_t dev;
	struct device *sensor_dev=NULL;

	/*prepare character device and return for error handling*/
	/*if(g_devMajor < 0){
		ret=create_ASH_chrdev();
		if(ret < 0)
			return NULL;
	}*/	

	/*prepare sys/class file node and return for error handling*/
	if(g_property_class == NULL || IS_ERR(g_property_class) ){
		ret=create_ASH_class();
		if(ret < 0)
			return NULL;
	}

	switch(type){
		case psensor:
			//dev = MKDEV(g_devMajor, psensor);
			sensor_dev = device_create(g_property_class, NULL, 0, NULL, "%s", "psensor");
			break;
		case lsensor:
			//dev = MKDEV(g_devMajor, lsensor);
			sensor_dev = device_create(g_property_class, NULL, 0, NULL, "%s", "lsensor");
			break;
		case frgbsensor:
			//dev = MKDEV(g_devMajor, frgbsensor);
			sensor_dev = device_create(g_property_class, NULL, 0, NULL, "%s", "frgbsensor");
			break;
		case hallsensor:
			//dev = MKDEV(g_devMajor, hallsensor);
			sensor_dev = device_create(g_property_class, NULL, 0, NULL, "%s", "hallsensor");
			break;
		case SAR_sensor:
			//dev = MKDEV(g_devMajor, SAR_sensor);
			sensor_dev = device_create(g_property_class, NULL, 0, NULL, "%s", "SAR_sensor");
			break;
		default:
			err("%s: Type ERROR.(%d)\n", __FUNCTION__, type);

	}

	if (IS_ERR(sensor_dev)) {
		ret = PTR_ERR(sensor_dev);
		err("%s: sensor_dev pointer is ERROR(%d). \n", __FUNCTION__, ret);		
		return NULL;
	}	

	g_sensordevice = sensor_dev;
	return sensor_dev;
}
EXPORT_SYMBOL(ASH_ATTR_device_create);

void ASH_ATTR_device_remove(ASH_type type)
{
	//dev_t dev;
	if(!g_sensordevice){
		err("sensor device is NULL, so can't remove it");
		return;
	}
	
	switch(type){
		case psensor:
			//dev = MKDEV(g_devMajor, psensor);
			device_unregister(g_sensordevice);
			break;
		case lsensor:
			//dev = MKDEV(g_devMajor, lsensor);
			device_unregister(g_sensordevice);
			break;
		case frgbsensor:
			//dev = MKDEV(g_devMajor, frgbsensor);
			device_unregister(g_sensordevice);
			break;
		case hallsensor:
			//dev = MKDEV(g_devMajor, hallsensor);
			device_unregister(g_sensordevice);
			break;
		case SAR_sensor:
			//dev = MKDEV(g_devMajor, SAR_sensor);
			device_unregister(g_sensordevice);
			break;
		default:
			err("%s: ASH_ATTR_device_remove Type ERROR.(%d)\n", __FUNCTION__, type);

	}

}
EXPORT_SYMBOL(ASH_ATTR_device_remove);
