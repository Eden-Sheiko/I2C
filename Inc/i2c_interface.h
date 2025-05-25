#ifndef __I2C_INTERFACE_H__
#define __I2C_INTERFACE_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* i2c and io includes */
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

typedef struct i2c_module i2c_module_t;

/* todo: add master slave? */
typedef struct i2c_module_config {
    size_t             speed;
    uint8_t            addr;
    char*              file_path;
} i2c_module_config_t;

typedef enum i2c_error {
    I2C_OK = 0,
    I2C_BUSY,
    I2C_ERROR,
    I2C_NULL_ERROR
} i2c_error_t;

i2c_module_t* i2c_device_init(i2c_module_config_t* config);

//i2c_error_t i2c_device_write_reg(i2c_module_t* dev, uint8_t reg, const uint8_t* pdata, size_t len);

/* todo - test if there is non blocking methods 
 * add also mutex to protect the device */
/* Future improvements: */
// i2c_error_t i2c_device_write_non_blk(i2c_module_t* dev, uint8_t reg, uint8_t* pdata, size_t len);
// i2c_error_t i2c_device_read_non_blk(i2c_module_t* dev, uint8_t reg, uint8_t* pdata, size_t len);


i2c_error_t i2c_device_read(i2c_module_t* dev, uint8_t* pdata, size_t len);

i2c_error_t i2c_device_write(i2c_module_t* dev, const uint8_t* pdata, size_t len);

i2c_error_t i2c_device_destroy(i2c_module_t *);



#endif /* __I2C_INTERFACE_H__ */