#include "../Inc/i2c_interface.h"
#include "../Inc/i2c_log.h"

int main(void){
    LOG_INFO("--- I2C Example App ---");
    uint8_t data = 0xff;
    i2c_module_config_t cfg = {
        .addr =  0x10,
        .file_path = "/dev/i2c-2",
        .debug = 1,
        .locking = 0
    };
    i2c_module_t* i2c = i2c_device_init(&cfg);
    if (i2c == NULL) {
        LOG_ERROR("module didnt load up\n");
    }
    if (i2c_device_write(i2c, &data, 1) != I2C_OK) {
        LOG_ERROR("write failed\n");
    }
    if (i2c_device_set_addr(0x55, i2c) != I2C_OK) {
        LOG_ERROR("Set addr failed\n");
    }
    if (i2c_device_write(i2c, &data, 1) != I2C_OK) {
        LOG_ERROR("write failed\n");
    }
    i2c_device_toggle_logger(false ,i2c);
    i2c_device_toggle_logger(true ,i2c);

    if (i2c_device_reg_write(i2c ,0x12 ,&data, 1) != I2C_OK) {
        LOG_ERROR("write failed\n");
    }


    i2c_device_destroy(i2c);
    return 0;

}