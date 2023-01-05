#include <driver/gpio.h>
#include <esp_log.h>

#include "esp_t9062.h"

#ifdef __cplusplus
extern "C" {
#endif


#define EXAMPLE_READ_REGISTER
#define EXAMPLE_WRITE_REGISTER
/* #define EXAMPLE_CHANGE_ADDRESS */

#define CONFIG_I2C_SDA_PIN 21
#define CONFIG_I2C_SCL_PIN 22
#define CONFIG_SENSOR_POWER_PIN 23
#define CONFIG_SENSOR_ADDRESS 0x28
#define CONFIG_SENSOR_NEW_ADDRESS 0x28

#define MAIN_TASK_LOOP_DELAY 5000

static const char *TAG = "example-main.c";

void app_main(void) {
    int8_t ret;
    uint16_t i;

    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = CONFIG_I2C_SDA_PIN,
        .scl_io_num = CONFIG_I2C_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    t9062_sensor_t t9062_sensor = {
        .i2c_port = 0,
        .address = CONFIG_SENSOR_ADDRESS,
        .time_of_measurement = 0,
        .sensor_power_pin = CONFIG_SENSOR_POWER_PIN,
        .i2c_config = &i2c_config,
    };

    ESP_LOGI(TAG, "[app_main] app started");

    // power sensor
    ESP_LOGI(TAG, "##### POWER SENSOR #####");
    ret = t9062_set_sensor_power(&t9062_sensor, true);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "[app_main] error t9062_set_sensor_power");
    }
    vTaskDelay(pdMS_TO_TICKS(70));

    // set up i2c port
    ESP_LOGI(TAG, "##### SETUP I2C #####");
    ret = i2c_param_config(t9062_sensor.i2c_port, &i2c_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "[app_main] error i2c_param_config");
    }
    ret = i2c_driver_install(t9062_sensor.i2c_port, i2c_config.mode, 0, 0, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "[app_main] error i2c_driver_install");
    }

    // t9062_read()
    ESP_LOGI(TAG, "##### READ SENSOR #####");
    ret = t9062_read(&t9062_sensor);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "t9062_read() returned error %i", ret);
    } else {
        ESP_LOGI(TAG, "t9062_read() t: %.1f°C rh: %.1f%%", t9062_sensor.temperature, t9062_sensor.humidity);
    }

#ifdef EXAMPLE_CHANGE_ADDRESS
    // t9062_change_address()
    ESP_LOGI(TAG, "##### CHANGE ADDRESS #####");
    ret = t9062_change_address(&t9062_sensor, CONFIG_SENSOR_NEW_ADDRESS);
    if (ret != 0) {
        ESP_LOGE(TAG, "t9062_change_address() returned error %i", ret);
    }
#endif

#ifdef EXAMPLE_READ_REGISTER
    // read registers
    ESP_LOGI(TAG, "##### READ REG #####");
    uint16_t register_data[T9062_REGISTERS_SIZE];
    for (i = 0; i < T9062_REGISTERS_SIZE; i++) {
        register_data[i] = 0xFFFF;
        ret = t9062_read_register(&t9062_sensor, i, &register_data[i]);
        if (ret != 0) {
            ESP_LOGE(TAG, "t9062_read_register() returned error %i", ret);
            ESP_LOGI(TAG, "register %i (%s): 0x%04X", i, t9062_register_name[i], register_data[i]);
        } else {
            ESP_LOGI(TAG, "register %i (%s): 0x%04X", i, t9062_register_name[i], register_data[i]);
        }
    }

#ifdef EXAMPLE_WRITE_REGISTER
    // write register
    ESP_LOGI(TAG, "##### WRITE REG #####");
    register_data[T9062_REG_CUST_ID2]++; // upcount the registers value
    ESP_LOGI(TAG, "writing 0x%04X to %s register", register_data[T9062_REG_CUST_ID2],
             t9062_register_name[T9062_REG_CUST_ID2]);
    ret = t9062_write_register(&t9062_sensor, T9062_REG_CUST_ID2, register_data[T9062_REG_CUST_ID2]);
    if (ret != 0) {
        ESP_LOGE(TAG, "t9062_writ_register() returned error %i", ret);
    } else {
        ESP_LOGI(TAG, "write register succeeded");
    }
#endif
#endif

    ESP_LOGI(TAG, "startup sequence finished, going into loop in 10s");
    vTaskDelay(pdMS_TO_TICKS(10000));

    while (1) {
        // read sensor
        t9062_read(&t9062_sensor);
        ESP_LOGI(TAG, "[app_main] t: %.1f°C rH: %.1f%% status: %i(0)", t9062_sensor.temperature, t9062_sensor.humidity, t9062_get_status(t9062_sensor.raw_data[0]));
        t9062_read(&t9062_sensor);
        ESP_LOGI(TAG, "[app_main] t: %.1f°C rH: %.1f%% status: %i(1)", t9062_sensor.temperature, t9062_sensor.humidity, t9062_get_status(t9062_sensor.raw_data[0]));
        vTaskDelay(pdMS_TO_TICKS(MAIN_TASK_LOOP_DELAY));
    }
}

#ifdef __cplusplus
}
#endif