/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author         Notes
 * 2021-08-08     XiaojieFan     Make the first version of mcp23008's package
 */

#include <rtthread.h>
#include "mcp23008.h"


#define TEST_IO    0 //(0-7)
#define I2C_BUS    "i2c1"

int mcp23008_sample(void)
{
    rt_uint8_t value;
    mcp23008_device_t dev = RT_NULL;
    
    dev = mcp23008_init(I2C_BUS, RT_NULL);

    if (dev == RT_NULL)
        return -1;
    mcp23008_set_pin_mode(dev, TEST_IO, OUTPUT_MODE);
    mcp23008_pin_write(dev, TEST_IO, 0);

    value = mcp23008_pin_read(dev, TEST_IO);
    rt_kprintf("The value of mcp23008.P%d is %d\n", TEST_IO, value);

    rt_thread_mdelay(1000);

    mcp23008_pin_write(dev, TEST_IO, 1);

    value = mcp23008_pin_read(dev, TEST_IO);
    rt_kprintf("The value of mcp23008.P%d is %d\n", TEST_IO, value);

    mcp23008_deinit(dev);

    return 0;
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(mcp23008_sample, a mcp23008 sample);
#endif
