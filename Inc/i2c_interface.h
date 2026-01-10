/**
 * \file           i2c_interface.h
 * \brief          I2C device interface abstraction layer
 * \author         Eden Sheiko
 */

#if !defined(__linux__)
    #error "Linux is NOT detected. Run the module with a Linux system."
#endif

#ifndef I2C_INTERFACE_H
#define I2C_INTERFACE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Forward declaration of I2C module structure
 */
typedef struct i2c_module i2c_module_t;

/**
 * \brief           I2C module configuration structure
 */
typedef struct i2c_module_config {
    uint8_t            addr;           /*!< 7-bit I2C device address */
    char*              file_path;      /*!< Path to I2C device file (e.g. "/dev/i2c-1") */
    bool               thread_safe;    /*!< Enable locking   */
    bool               verbose;        /*!< Enable Logging   */
} i2c_module_config_t;

/**
 * \brief           I2C error codes
 */
typedef enum i2c_error {
    I2C_OK = 0,             /*!< Operation successful */
    I2C_BUSY,               /*!< I2C device busy */
    I2C_ERROR,              /*!< General error */
    I2C_TIMEOUT_ERROR,      /*!< Timeout occurred */
    I2C_NULL_ERROR,         /*!< Null pointer provided */
    INVALID_ARG             /*!< Invalid argument */
} i2c_error_t;

/**
 * \brief           Initialize an I2C device with given configuration
 * \param[in]       config: Pointer to configuration structure
 * \return          Pointer to I2C module instance or NULL on failure
 */
i2c_module_t* i2c_device_init(i2c_module_config_t* config);

/**
 * \brief           Read data from I2C device (blocking)
 * \param[in]       dev: Pointer to initialized I2C module
 * \param[out]      pdata: Pointer to buffer to store data
 * \param[in]       len: Number of bytes to read
 * \return          I2C error code
 */
i2c_error_t i2c_device_read(i2c_module_t* dev, uint8_t* pdata, size_t len);

/**
 * \brief           Write data to I2C device
 * \param[in]       dev: Pointer to initialized I2C module
 * \param[in]       pdata: Pointer to data to write
 * \param[in]       len: Number of bytes to write
 * \return          I2C error code
 */
i2c_error_t i2c_device_write(i2c_module_t* dev, const uint8_t* pdata, size_t len);

/**
 * \brief           Destroy and clean up I2C module instance
 * \param[in]       dev: Pointer to I2C module
 * \return          I2C error code
 */
i2c_error_t i2c_device_destroy(i2c_module_t* dev);

i2c_error_t i2c_device_set_file_path(char* file_path, i2c_module_config_t* config);

i2c_error_t i2c_device_set_addr(i2c_module_config_t* config);

i2c_error_t i2c_device_toggle_log(i2c_module_config_t* config);

i2c_error_t i2c_device_toggle_thread_safe(i2c_module_config_t* config);





#ifdef __cplusplus
}
#endif

#endif /* I2C_INTERFACE_H */
