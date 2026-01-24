/**
 * \file           i2c_interface.c
 * \brief          I2C device interface implementation
 * \author         Eden Sheiko
 * \var            0.9.0
 */

#include "../Inc/i2c_interface.h"
#include "../Inc/i2c_log.h"
#include <string.h>     /* For strerror */
#include <fcntl.h>      /* For open */
#include <unistd.h>     /* For close, read, write */
#include <errno.h>      /* For errno */

/**
 * \brief           I2C module structure definition
 */
struct i2c_module {
    uint8_t            addr;           /*!< I2C slave address */
    char*              file_path;      /*!< I2C device file path */
    int                fd;             /*!< File descriptor */ 
    bool               ctx_log;        /*!< logger */  
    bool               ctx_safe;       /*!< mutex */  
    pthread_mutex_t    mutex;          /*!< Lock */
};

/**
 * \brief           Initialize I2C module
 * \param[in]       config: Pointer to I2C configuration
 * \return          Pointer to I2C module on success, NULL on failure
 */
i2c_module_t* i2c_device_init(i2c_module_config_t* config) {
    i2c_module_t* i2c_instance = NULL;

    if (config == NULL) {
        goto err;
    }

    i2c_instance = calloc(1, sizeof(i2c_module_t));
    if (i2c_instance == NULL) {
        goto err;
    }

    i2c_instance->addr = config->addr;
    i2c_instance->file_path = config->file_path;
    i2c_instance->ctx_log = config->debug;
    i2c_instance->ctx_safe = config->locking;

    if (i2c_instance->ctx_safe) {
        if (pthread_mutex_init(&i2c_instance->mutex, NULL) != 0) {
            goto err_clean;
        }
    }
    
    i2c_instance->fd = open(i2c_instance->file_path, O_RDWR);
    if (i2c_instance->fd < 0) {
        if (i2c_instance->ctx_log) {
            LOG_ERROR("I2C open failed %d", i2c_instance->fd);
        }
        goto err_clean;
    }

    if (ioctl(i2c_instance->fd, I2C_SLAVE, i2c_instance->addr) < 0) {
        if (i2c_instance->ctx_log) {
            LOG_ERROR("I2C ioctl failed %d", i2c_instance->fd);
        }
        goto err_close_fd;
    }

    if (i2c_instance->ctx_log) {
        LOG_INFO("I2C setup successful");
    }
    return i2c_instance;

    err_clean:
    free(i2c_instance);
    return NULL;
    
    err_close_fd:
    close(i2c_instance->fd);
    if (i2c_instance->ctx_safe) {
        if (pthread_mutex_destroy(&i2c_instance->mutex) != 0) {
            return NULL;
        }
    }
    free(i2c_instance);
    return NULL;
    
    err:
    return NULL;
    
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
        if (dev->ctx_log) {
            LOG_ERROR("Failed to write to I2C");
        }
        return I2C_ERROR;
    }

    if ((size_t)bytes_wr != len) {
        if (dev->ctx_log) {
            LOG_WARN("Partial I2C write: expected %zu, wrote %zd", len, bytes_wr);
        }
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
        if (dev->ctx_log) {
            LOG_ERROR("Failed to read from I2C");
        }
        return I2C_ERROR;
    }

    if ((size_t)bytes_rd != len) {
        if (dev->ctx_log) {
            LOG_WARN("Partial I2C read: expected %zu, got %zd", len, bytes_rd);
        }
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
    if (dev->ctx_safe) {
        if (pthread_mutex_destroy(&dev->mutex) != 0){
            free(dev);
            return I2C_ERROR;
        }
    }
    free(dev);
    return I2C_OK;
}

/* todo fix the error */
i2c_error_t i2c_device_set_file_path(char* file_path, i2c_module_t* dev){
    if (file_path == NULL || dev == NULL){
        return I2C_NULL_ERROR;
    }
    strncpy(dev->file_path, file_path, sizeof(file_path));
    if (dev->ctx_log) {
        LOG_INFO("I2C file path changed successful to %s \n", file_path);
    }
    return I2C_OK;
}

/* todo need to update the kenel driver*/
i2c_error_t i2c_device_set_addr(uint8_t addr, i2c_module_t* dev){
    if (addr > 0x77 || dev == NULL){
        return I2C_NULL_ERROR;
    }
    dev->addr = addr;
    if (dev->ctx_log) {
        LOG_INFO("I2C addr changed successful to %u \n", dev->addr);
    }
    return I2C_OK;
}

i2c_error_t i2c_device_toggle_lock(bool ctx, i2c_module_t* dev) {
    if (dev == NULL){
        return I2C_NULL_ERROR;
    }
    dev->ctx_safe = ctx;
    if (dev->ctx_log) {
        LOG_INFO("lock set to : %d \n", dev->ctx_safe);
    }
    return I2C_OK;
}

i2c_error_t i2c_device_toggle_logger(bool ctx, i2c_module_t* dev) {
    if (dev == NULL){
        return I2C_NULL_ERROR;
    }
    dev->ctx_log = ctx;
    LOG_INFO("log set to : %d \n",  dev->ctx_log);
    return I2C_OK;
}

