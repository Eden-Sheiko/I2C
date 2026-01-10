/**
 * \file           i2c_interface.c
 * \brief          I2C device interface implementation
 */

#include "../Inc/i2c_interface.h"
#include "../Inc/log.h"
#include <string.h>     /* For strerror */
#include <poll.h>       /* For poll */
#include <fcntl.h>      /* For open */
#include <unistd.h>     /* For close, read, write */
#include <errno.h>      /* For errno */

/**
 * \brief           I2C module structure definition
 */
struct i2c_module {
    size_t             speed;          /*!< Bus speed in Hz */
    uint8_t            addr;           /*!< I2C slave address */
    char*              file_path;      /*!< I2C device file path */
    int                fd;             /*!< File descriptor */
};

/**
 * \brief           Initialize I2C module
 * \param[in]       config: Pointer to I2C configuration
 * \return          Pointer to I2C module on success, NULL on failure
 */
i2c_module_t* i2c_device_init(i2c_module_config_t* config) {
    i2c_module_t* i2c_instance = NULL;

    if (config == NULL) {
        return NULL;
    }

    i2c_instance = calloc(1, sizeof(i2c_module_t));
    if (i2c_instance == NULL) {
        return NULL;
    }

    i2c_instance->speed = config->speed;
    i2c_instance->addr = config->addr;
    i2c_instance->file_path = config->file_path;

    

    i2c_instance->fd = open(i2c_instance->file_path, O_RDWR);
    if (i2c_instance->fd < 0) {
        if (config->verbose) {
           LOG_ERROR("I2C open failed %d", i2c_instance->fd);
        }
        free(i2c_instance);
        return NULL;
    }

    if (ioctl(i2c_instance->fd, I2C_SLAVE, i2c_instance->addr) < 0) {
        if (config->verbose ) {
            LOG_ERROR("I2C ioctl failed %d", i2c_instance->fd);
        }
        close(i2c_instance->fd);
        free(i2c_instance);
        return NULL;
    }

     if (config->verbose) {
            LOG_INFO("I2C setup successful");
        }
    return i2c_instance;
}

/**
 * \brief           Write data to I2C device
 * \param[in]       dev: I2C module instance
 * \param[in]       pdata: Pointer to data to write
 * \param[in]       len: Number of bytes to write
 * \return          I2C error code
 */
i2c_error_t i2c_device_write(i2c_module_t* dev, const uint8_t* pdata, size_t len) {
    ssize_t bytes_wr = 0;

    if (dev == NULL || pdata == NULL) {
        return I2C_NULL_ERROR;
    }

    bytes_wr = write(dev->fd, pdata, len);
    if (bytes_wr < 0) {
        LOG_ERROR("Failed to write to I2C");
        return I2C_ERROR;
    }

    if ((size_t)bytes_wr != len) {
        LOG_WARN("Partial I2C write: expected %zu, wrote %zd", len, bytes_wr);
        return I2C_ERROR;
    }

    return I2C_OK;
}

/**
 * \brief           Read data from I2C device (blocking)
 * \param[in]       dev: I2C module instance
 * \param[out]      pdata: Pointer to buffer to store read data
 * \param[in]       len: Number of bytes to read
 * \return          I2C error code
 */
i2c_error_t i2c_device_read(i2c_module_t* dev, uint8_t* pdata, size_t len) {
    ssize_t bytes_rd = 0;

    if (dev == NULL || pdata == NULL) {
        return I2C_NULL_ERROR;
    }

    bytes_rd = read(dev->fd, pdata, len);
    if (bytes_rd < 0) {
        LOG_ERROR("Failed to read from I2C");
        return I2C_ERROR;
    }

    if ((size_t)bytes_rd != len) {
        LOG_WARN("Partial I2C read: expected %zu, got %zd", len, bytes_rd);
        return I2C_ERROR;
    }

    return I2C_OK;
}


/**
 * \brief           Destroy I2C module and free resources
 * \param[in]       dev: I2C module instance
 * \return          I2C error code
 */
i2c_error_t i2c_device_destroy(i2c_module_t* dev) {
    if (dev == NULL) {
        return I2C_NULL_ERROR;
    }

    close(dev->fd);
    free(dev);

    return I2C_OK;
}
