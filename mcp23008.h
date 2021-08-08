/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-08-08     XiaojieFan    first version
 */

#ifndef __MCP23008_H
#define __MCP23008_H

#include <rtthread.h>
#include <rtdevice.h>

/*mcp23008 register */

#define MCP23008_IODIR 			0x00
#define MCP23008_IPOL  			0x01
#define MCP23008_GPINTEN 		0x02
#define MCP23008_DEFVAL   		0x03
#define MCP23008_INTCON 			0x04
#define MCP23008_IOCON				0x05
#define MCP23008_GPPU  			0x06
#define MCP23008_INTF				0x07
#define MCP23008_INTCAP			0x08
#define MCP23008_GPIO				0x09
#define MCP23008_OLAT				0x0A

/* mcp23008 register value */

#define INPUT_MODE  1
#define OUTPUT_MODE  0
#define INPUT_PULL_UP_MODE 2
#define NO_INPUT_PULL_UP_MODE 3


#define MCP23008_ADDR (0x40 >> 1)//0x20 0100000

struct mcp23008_device
{
	struct rt_i2c_bus_device *i2c;
	rt_mutex_t lock;
	uint8_t AddrInput;	
	
};
typedef struct mcp23008_device *mcp23008_device_t;

/**
 * This function initialize the mcp23008 device.
 *
 * @param dev_name the name of i2c bus device
 * @param i2c_addr the i2c device address for i2c communication,RT_NULL use default address
 *
 * @return the pointer of device structure, RT_NULL reprensents  initialization failed.
 */
mcp23008_device_t mcp23008_init(const char *dev_name, rt_uint8_t i2c_addr);

/**
 * This function releases memory
 *
 * @param dev the pointer of device structure
 */
void mcp23008_deinit(struct mcp23008_device *dev);

/**
 * This function read the data port of mcp23008.
 *
 * @param dev the pointer of device structure
 *
 * @return the state of data port. 0xFF meas all pin is high.
 */
uint8_t mcp23008_port_read(mcp23008_device_t dev);

/**
 * This function sets the status of the data port.
 *
 * @param dev the pointer of device structure
 * @param port_val the port value you want to set, 0xFF meas all pin output high.
 */
void mcp23008_port_write(mcp23008_device_t dev, uint8_t port_val);

/**
 * This function read the specified port pin of the mcp23008.
 *
 * @param dev the pointer of device structure
 * @param pin the specified pin of the data port
 *
 * @return the status of the specified data port pin, 0 is low, 1 is high.
 */
uint8_t mcp23008_pin_read(mcp23008_device_t dev, uint8_t pin);

/**
 * This function sets the status of the specified port pin.
 *
 * @param dev the pointer of device structure
 * @param pin_val the specified pin value you want to set, 0 is low, 1 is high.
 */
void mcp23008_pin_write(mcp23008_device_t dev, uint8_t pin, uint8_t pin_val);

/**
 * This function sets the mode of the specified port pin.
 *
 * @param dev the pointer of device structure
 * @param pin the specified pin of the data port
 * @param mode the specified pin mode you want to set, INPUT_MODE OR OUTPUT_MODE.
 */
rt_err_t mcp23008_set_pin_mode(mcp23008_device_t dev, rt_uint8_t pin, uint8_t mode);
#endif

