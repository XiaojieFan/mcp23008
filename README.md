# mcp23008 package

[中文页](README_ZH.md) | English

## Introduction

The `mcp23008` software package is a software package developed by XiaojieFan for the I2C parallel port expansion circuit MCP23008. Using this software package, you can use the device on RT-Thread very conveniently .

This article mainly introduces the use of the package, API, and `MSH` test commands.

### Directory Structure

```
mcp23008
│ README.md // package description
│ mcp23008.c // source file
│ mcp23008.h // header file
│ mcp23008_sample.c // Sample code for software package
│ SConscript // RT-Thread default build script
│ LICENSE // License file
```

### License

mcp23008 complies with the Apache-2.0 license, see the `LICENSE` file for details.

### Dependence

- RT_Thread 3.0+
- i2c device driver

## method of obtaining

To use `mcp23008 package`, you need to select it in the package management of RT-Thread. The specific path is as follows:

```
RT-Thread online packages
    peripheral libraries and drivers --->
        mcp23008: Remote 8-bit I/O expander for I2C-bus --->
```

Enter the configuration menu of the mcp23008 software package for specific configuration according to your needs

```
    --- mcp23008: Remote 8-bit I/O expander for I2C-bus
        [*] Enable mcp23008 sample
           Version (latest) --->
```

**Enable mcp23008 sample**: Enable mcp23008 sample

After the configuration is complete, let the RT-Thread package manager automatically update, or use the pkgs --update command to update the package to the BSP.

## Instructions

The use process of mcp23008 software package is generally as follows:

1. Initialize mcp23008 device `mcp23008_init`
2. Perform IO operations
   - Use API `mcp23008_port_read/mcp23008_port_write` to operate 8 channels of IO at the same time
   - Use API `mcp23008_pin_read/mcp23008_pin_write` to operate one of the IOs separately

For detailed usage, please refer to [mcp23008 sample program](mcp23008_sample.c).

## MSH Test Command

If the sample program of the mcp23008 software package is enabled, the command `mcp23008_sample` will be exported to the console. After the call, it will detect the MCP23008 device with the address of `0x20` on the `i2c1` bus by default, and will operate the port 0 of the expansion port for testing. The results are as follows:

```
msh >mcp23008_sample
[D/mcp23008] mcp23008 init done
The value of mcp23008.P0 is 0
The value of mcp23008.P0 is 1
msh>
```

## Precautions

Nothing.

## Contact information

- Maintenance: [XiaojieFan](https://github.com/XiaojieFan)
- Homepage: <https://github.com/XiaojieFan/mcp23008>
