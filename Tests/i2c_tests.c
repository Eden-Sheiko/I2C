#include "../Inc/i2c_interface.h"
#include "../Inc/i2c_log.h"

/* todo: add test framework */

int main(void){
    LOG_INFO("TEST HAS STARTED");
    uint8_t data = 0xff;
    i2c_module_config_t cfg = {
        .addr =  0x10,
        .file_path = "/dev/i2c-2",
        .debug = 1,
        .locking = 0
    };
    i2c_module_t* i2c = i2c_device_init(&cfg);
    if (i2c == NULL) {
        LOG_ERROR("INIT TEST FAILED");
    }else {
        LOG_INFO("INIT TEST PASSED");
    }
    if (i2c_device_toggle_logger(false, i2c) != I2C_OK) {
        LOG_ERROR("LOGGER FALSE TEST FAILED");
    }else {
        LOG_INFO("LOGGER FALSE TEST PASSED");
    }
    if (i2c_device_toggle_logger(true, i2c) != I2C_OK) {
        LOG_ERROR("LOGGER TRUE TEST FAILED");
    }else {
        LOG_INFO("LOGGER TRUE TEST PASSED");
    }

    if (i2c_device_write(i2c, &data, 1) != I2C_OK) {
        LOG_ERROR("write 1b TEST FAILED");
    }else {
        LOG_INFO("write 1b TEST PASSED");
    }





    i2c_device_destroy(i2c);
    return 0;

}