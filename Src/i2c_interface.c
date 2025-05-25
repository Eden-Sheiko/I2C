#include "../Inc/i2c_interface.h"
#include "../Inc/log.h"

struct i2c_module {
    size_t             speed;
    uint8_t            addr;
    char*              file_path;
    int                fd;
};

i2c_module_t* i2c_device_init(i2c_module_config_t* config){
    i2c_module_t* i2c_instance = NULL;
    if (config == NULL) {
        return NULL;
    }
    i2c_instance = calloc(1,sizeof(i2c_module_t));
    if (i2c_instance == NULL) {
        return NULL;
    }
    i2c_instance->speed = config->speed;
    i2c_instance->addr = config->addr;
    i2c_instance->file_path = config->file_path;

    i2c_instance->fd = open(i2c_instance->file_path, O_RDWR);
    if (i2c_instance->fd < 0) {
        LOG_ERROR("I2C open failed %d \n",i2c_instance->fd);
        close(i2c_instance->fd);
        free(i2c_instance);
        return NULL;
    }
    if (ioctl(i2c_instance->fd, I2C_SLAVE, i2c_instance->addr ) < 0) {
        LOG_ERROR("I2C ioctl failed %d \n",i2c_instance->fd);
        close(i2c_instance->fd);
        free(i2c_instance);
        return NULL;
    }
    LOG_INFO("I2C setup was ok \n");
    return i2c_instance;
}

i2c_error_t i2c_device_write(i2c_module_t* dev, const uint8_t* pdata, size_t len) {
    ssize_t bytes_rd = 0;
    if (dev == NULL || pdata == NULL) {
        return I2C_NULL_ERROR;
    }
    bytes_rd = write(dev->fd, pdata, len);
    if (bytes_rd < 0) {
        return I2C_ERROR;
    }
    if (bytes_rd != len) {
        return I2C_ERROR;
    }
    return I2C_OK;
}

i2c_error_t i2c_device_read(i2c_module_t* dev, uint8_t* pdata, size_t len) {
    ssize_t bytes_wr = 0;
    if (dev == NULL || pdata == NULL) {
        return I2C_NULL_ERROR;
    }
    bytes_wr = read(dev->fd, pdata, len);
    if (bytes_wr < 0) {
        return I2C_ERROR;
    }
    if (bytes_wr != len) {
        return I2C_ERROR;
    }
    return I2C_OK;
}

i2c_error_t i2c_device_destroy(i2c_module_t* dev) {
    if (dev == NULL) {
        return I2C_NULL_ERROR;
    }
    free(dev);
    return I2C_OK;
}
