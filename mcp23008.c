/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-08     XiaojieFan   first version
 */

#include "mcp23008.h"

#define DBG_ENABLE
#define DBG_SECTION_NAME     "mcp23008"
#define DBG_LEVEL            DBG_INFO
#define DBG_COLOR
#include <rtdbg.h>

#define	CLEAR_SPEC_BIT(x, bit)	(x &= ~(1 << bit))	/* 清零第bit位 */

#define	SET_SPEC_BIT(x, bit)	(x |= (1 << bit))	/* 置位第bit位 */


static uint8_t mcp23008_read_one_byte(mcp23008_device_t dev, uint8_t  reg)
{
    struct rt_i2c_msg msg[2] = {0};
    rt_uint8_t temp = 0;
    RT_ASSERT(dev != RT_NULL);
    msg[0].addr 	= MCP23008_ADDR;
    msg[0].flags 	= RT_I2C_WR;
    msg[0].len 		= 1;
    msg[0].buf 		= &reg;
    msg[1].addr 	= MCP23008_ADDR;
    msg[1].flags 	= RT_I2C_RD;// will restart
    msg[1].len 		= 1;
    msg[1].buf		= &temp;
    rt_mutex_take(dev->lock, RT_WAITING_FOREVER);

    if (rt_i2c_transfer(dev->i2c, msg, 2) != 2)
    {
        LOG_E("i2c write reg failed.\n");
    }

    rt_mutex_release(dev->lock);
    return temp;
}

static rt_err_t mcp23008_write_one_byte(mcp23008_device_t dev, uint8_t reg, uint8_t data)
{
    struct rt_i2c_msg msg[2] = {0};

    RT_ASSERT(dev != RT_NULL);
    msg[0].addr 	= MCP23008_ADDR;
    msg[0].flags 	= RT_I2C_WR;
    msg[0].len 		= 1;
    msg[0].buf 		= &reg;
    msg[1].addr  	= MCP23008_ADDR;
    msg[1].flags 	= RT_I2C_WR | RT_I2C_NO_START;
    msg[1].len 		= 1;
    msg[1].buf		= &data;
    rt_mutex_take(dev->lock, RT_WAITING_FOREVER);
    if (rt_i2c_transfer(dev->i2c, msg, 2) == 2)
    {
        rt_mutex_release(dev->lock);
        return RT_EOK;
    }
    else
    {
        rt_mutex_release(dev->lock);
        LOG_E("i2c bus write failed!\n");
        return -RT_ERROR;
    }
}

rt_err_t mcp23008_set_pin_mode(mcp23008_device_t dev, rt_uint8_t pin, uint8_t mode)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t value = 0;
    value = mcp23008_read_one_byte(dev,MCP23008_IODIR);
    if (mode == OUTPUT_MODE)//OUTPUT
    {
        CLEAR_SPEC_BIT(value,pin);
    }
    else if(mode == INPUT_MODE) //INPUT
    {
        SET_SPEC_BIT(value,pin);
    }
    ret = mcp23008_write_one_byte(dev,MCP23008_IODIR,value);	   		

    return ret;
}

rt_err_t mcp23008_set_pin_pullup_mode(mcp23008_device_t dev, rt_uint8_t pin, uint8_t mode)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t value = 0;
    value = mcp23008_read_one_byte(dev,MCP23008_GPPU);
    if (mode == NO_INPUT_PULL_UP_MODE)//OUTPUT
    {
        CLEAR_SPEC_BIT(value,pin);
    }
    else if(mode == INPUT_PULL_UP_MODE) //INPUT
    {
        SET_SPEC_BIT(value,pin);
    }
    ret = mcp23008_write_one_byte(dev,MCP23008_GPPU,value);	    		

    return ret;
}

rt_uint8_t mcp_read_gpio(mcp23008_device_t dev)
{
    return mcp23008_read_one_byte(dev, MCP23008_GPIO);
}

rt_err_t mcp_write_gpio(mcp23008_device_t dev, rt_uint8_t value)
{
    return mcp23008_write_one_byte(dev, MCP23008_GPIO, value);

}

rt_err_t mcp_write_pin(mcp23008_device_t dev, rt_uint8_t pin, rt_uint8_t value)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t temp = 0;
    temp = mcp_read_gpio(dev);
    if (value == 0x01)
    {
        SET_SPEC_BIT(temp,pin);
    }
    else if (value == 0x00)
    {
        CLEAR_SPEC_BIT(temp,pin);
    }
    return mcp_write_gpio(dev,temp);
}
//polarity 0 active-low 
rt_err_t mcp_config_IOCON_(mcp23008_device_t dev, rt_uint8_t polarity, rt_bool_t openDrain)
{
    rt_err_t ret = RT_EOK;
	rt_uint8_t value = 0;
    rt_uint8_t temp = 0xFF;
    value = mcp23008_read_one_byte(dev, MCP23008_IOCON);
    value |= (polarity << 1);
    value |= (openDrain << 2);
    return mcp23008_write_one_byte(dev,MCP23008_IOCON, value);		
}

//一次性将需要的配置信息 设置OK,不单独设置一个引脚 mode 暂不使用
rt_err_t mcp_config_interrupt(mcp23008_device_t dev,  rt_uint8_t pin, rt_uint8_t mode)
{
    rt_err_t ret = RT_EOK;
    ret = mcp23008_write_one_byte(dev, MCP23008_GPINTEN, pin);
    if (ret != RT_EOK)
    {
        LOG_E("write GPINTEN failed .\n");
        return ret;
    }
    ret = mcp23008_write_one_byte(dev, MCP23008_INTCON, pin);
    if (ret != RT_EOK)
    {
        LOG_E("write INCCON failed .\n");
        return ret;
    }
    return ret;
}

uint8_t mcp23008_port_read(mcp23008_device_t dev)
{
    return mcp_read_gpio(dev);
}

void mcp23008_port_write(mcp23008_device_t dev, uint8_t value)
{
    mcp_write_gpio(dev, value);
}

uint8_t mcp23008_pin_read(mcp23008_device_t dev, uint8_t bit)
{
    uint8_t data;
    data = mcp23008_port_read(dev);

    if (data & (1 << bit))
        return 1;
    else
        return 0;
}

void mcp23008_pin_write(mcp23008_device_t dev, uint8_t bit, uint8_t value)
{
    uint8_t data;
    data = mcp23008_port_read(dev);

    if (value == 0)
        data &= ~(1 << bit);
    else 
        data |= 1 << bit;

    mcp23008_port_write(dev, data);
}


mcp23008_device_t mcp23008_init(const char *i2c_bus_name, uint8_t AddrInput)
{
    mcp23008_device_t dev;
    RT_ASSERT(i2c_bus_name);
    dev = rt_calloc(1, sizeof(struct mcp23008_device));
    if (dev == RT_NULL)
    {	
        LOG_E("can't allocate memory for mcp23008 device on '%s' ", i2c_bus_name);
        return RT_NULL;
    }
    dev->i2c = rt_i2c_bus_device_find(i2c_bus_name);
    if (dev->i2c == RT_NULL)
    {
        LOG_E("Can't find mcp23008 device on '%s' ", i2c_bus_name);
        return RT_NULL;
    }
    dev->lock = rt_mutex_create("mutex_mcp23008", RT_IPC_FLAG_FIFO);
    if (dev->lock == RT_NULL)
    {
        LOG_E("Can't create mutex for mcp23008 device on ' %s ' ", i2c_bus_name);
        return RT_NULL;
    }
    dev->AddrInput = AddrInput;
    return dev;		
}

void mcp23008_deinit(struct mcp23008_device *dev)
{
 	RT_ASSERT(dev);
	rt_mutex_delete(dev->lock);
	rt_free(dev);
}
