#include "../Inc/i2c_interface.h"
#include "../Inc/i2c_log.h"

int main(void){
    printf("start program testing \n");
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
        printf("we have an error\n");
    }





    i2c_device_destroy(i2c);
    return 0;

}