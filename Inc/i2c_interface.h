/**
 * \file           i2c_interface.h
 * \brief          I2C device interface abstraction layer
 * \author         Eden Sheiko
 * \var            0.9.0
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
#include <pthread.h>



#ifdef __cplusplus
extern "C" {
#endif

#define             MAX_ADDR 0x7F
#define             MAX_BUFF 4096           /* 4096 Byte */

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
    bool               debug;          /*!< Debug logger */
    bool               locking;        /*!< locking option */
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
    INVALID_ARG,            /*!< Invalid argument */
    I2C_OVERFLOW,           /*!< Buffer overflow */
    I2C_LOCK_FAILED,        /*!< Fail to Lock */
    I2C_UNLOCK_FAILED       /*!< Fail to UnLock */
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

/**
 * \brief           Set the filesystem path for the I2C device
 * \note            This function handles memory allocation. The previous path in `dev`
 * will be freed automatically.
 * \param[in]       file_path: String containing the path (e.g., "/dev/i2c-1")
 * \param[in,out]   dev: Pointer to I2C device handle to modify
 * \return          \ref I2C_OK on success, member of \ref i2c_error_t otherwise
 */
i2c_error_t i2c_device_set_file_path(char* file_path, i2c_module_t* dev);

/**
 * \brief           Set the I2C slave address for the device
 * \note            This function calls `ioctl` immediately to set the address on the
 * open file descriptor.
 * \param[in]       addr: The 7-bit I2C slave address (must be < \ref MAX_ADDR)
 * \param[in,out]   dev: Pointer to I2C device handle
 * \return          \ref I2C_OK on success, member of \ref i2c_error_t otherwise
 */
i2c_error_t i2c_device_set_addr(uint8_t addr, i2c_module_t* dev);

/**
 * \brief           Toggle the safety lock/mutex context (Future Feature)
 * \note            This function is currently a placeholder.
 * \param[in]       ctx: Set to `true` to lock, `false` to unlock
 * \param[in,out]   dev: Pointer to I2C device handle
 * \return          \ref I2C_OK
 */
i2c_error_t i2c_device_toggle_lock(bool ctx, i2c_module_t* dev);

/**
 * \brief           Enable or disable the internal logger context for the I2C device
 * \param[in]       ctx: Set to `true` to enable logging, `false` to disable
 * \param[in,out]   dev: Pointer to I2C device handle
 * \return          \ref I2C_OK on success, member of \ref i2c_error_t otherwise
 */
i2c_error_t i2c_device_toggle_logger(bool ctx, i2c_module_t* dev);

/**
* \brief           Write data to a specific register of an I2C device
* \param[in,out]   dev: Pointer to I2C device handle
* \param[in]       reg: The internal register address to write to
* \param[in]       pdata: Pointer to the data buffer to write
* \param[in]       len: Number of bytes to write
* \return          \ref I2C_OK on success, member of \ref i2c_error_t otherwise
*/
i2c_error_t i2c_device_reg_write(i2c_module_t* dev, uint8_t reg, const uint8_t* pdata, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* I2C_INTERFACE_H */
