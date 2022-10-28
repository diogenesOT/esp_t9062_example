esp-t9062-example
====================
![Build Status](https://github.com/diogenesOT/esp_t9062_example/actions/workflows/build.yml/badge.svg)


This project includes a sample program and a program to run unit/integration tests for the [esp_t9062](https://github.com/diogenesOT/esp_t9062) library.



# How to use

## Prerequisites
- ESP-IDF installed and activated (typically `get_idf`)
- ESP32
- T9062 sensor

Clone the repository and connect the sensor to the ESP32.

| T9062 | ESP32 |
|-------|-------|
| V_ss | GND |
| V_dd | 19 |
| SCL | 22 |
| SDA | 21 |

## Run example
1. `cd esp_t9062_example`
2. `idf.py build flash monitor`

### Example output

```
...
I (0) cpu_start: Starting scheduler on APP CPU.
I (315) example-main.c: [app_main] app started
I (325) example-main.c: ##### POWER SENSOR #####
I (395) example-main.c: ##### SETUP I2C #####
I (395) example-main.c: ##### READ SENSOR #####
I (395) example-main.c: t9062_read() t: 20.1°C rh: 65.9%
I (395) example-main.c: ##### READ REG #####
I (475) example-main.c: register 0 (PDM_CLIP_HIGH): 0x0000
I (475) example-main.c: register 1 (PDM_CLIP_LOW): 0x0000
I (475) example-main.c: register 2 (ALARM_HIGH_ON): 0x3FFF
I (485) example-main.c: register 3 (ALARM_HIGH_OFF): 0x3FFF
I (485) example-main.c: register 4 (ALARM_LOW_ON): 0x0000
I (495) example-main.c: register 5 (ALARM_LOW_OFF): 0x0000
I (495) example-main.c: register 6 (CUST_CONFIG): 0x0028
I (505) example-main.c: register 7 (RESERVED): 0x0000
I (515) example-main.c: register 8 (CUST_ID2): 0x75CB
I (515) example-main.c: register 9 (CUST_ID3): 0x0000
I (525) example-main.c: ##### WRITE REG #####
I (525) example-main.c: writing 0x75CC to CUST_ID2 register
I (605) example-main.c: write register succeeded
I (605) example-main.c: startup sequence finished, going into loop in 10s
I (10605) example-main.c: [app_main] b Free memory: 294644 bytes
W (10605) esp_t9062.h: status bits check failed. 0x02
W (10605) esp_t9062.h: [t9062_read()] sensor not in normal mode: 0x004(adr:0x28)
I (10755) example-main.c: [app_main] t: 20.7°C rH: 65.8%
I (10755) example-main.c: [app_main] t: 20.7°C rH: 65.8%
I (15755) example-main.c: [app_main] b Free memory: 294644 bytes
I (15755) example-main.c: [app_main] t: 20.6°C rH: 65.7%
I (15755) example-main.c: [app_main] t: 20.6°C rH: 65.7%
...
```

## Run tests
1. `cd esp_t9062_example/test`
2. `idf.py menuconfig` and turn off task wdt
3. `idf.py build flash monitor`</br>
programm runs all tests on startup, thereafter single tests can be executed
4. type [test_id] + [ENTER] to run specific test

### Example output

```
...
I (0) cpu_start: Starting scheduler on APP CPU.

#### Running all the registered tests #####

Running set sensor power ON...
/home/peter/software/2022/tmp/esp_t9062_example/components/esp_t9062/test/test_esp_t9062.c:37:set sensor power ON:PASS
Running set sensor power OFF...
/home/peter/software/2022/tmp/esp_t9062_example/components/esp_t9062/test/test_esp_t9062.c:39:set sensor power OFF:PASS
Running i2c driver installation...
/home/peter/software/2022/tmp/esp_t9062_example/components/esp_t9062/test/test_esp_t9062.c:41:i2c driver installation:PASS
Running read sensor...
/home/peter/software/2022/tmp/esp_t9062_example/components/esp_t9062/test/test_esp_t9062.c:47:read sensor:PASS
Running read sensor - wrong address...
E (2750) esp_t9062.h: [t9062_read()] fatal error : 0x107(adr:0x20)
E (3750) esp_t9062.h: [t9062_read()] fatal error : 0x107(adr:0x20)
E (4750) esp_t9062.h: [t9062_read()] fatal error : 0x107(adr:0x20)
E (5750) esp_t9062.h: [t9062_read()] fatal error : 0x107(adr:0x20)
E (6750) esp_t9062.h: [t9062_read()] fatal error : 0x107(adr:0x20)
E (6750) esp_t9062.h: [t9062_read()] error: 0x107(adr:0x20)
/home/peter/software/2022/tmp/esp_t9062_example/components/esp_t9062/test/test_esp_t9062.c:60:read sensor - wrong address:PASS
Running read register...
/home/peter/software/2022/tmp/esp_t9062_example/components/esp_t9062/test/test_esp_t9062.c:73:read register:PASS

-----------------------
6 Tests 0 Failures 0 Ignored 
OK

#### Starting interactive test menu #####



Press ENTER to see the list of tests.


Here's the test menu, pick your combo:
(1)     "set sensor power ON" [set_sensor_power]
(2)     "set sensor power OFF" [set_sensor_power]
(3)     "i2c driver installation" [t9062]
(4)     "read sensor" [t9062]
(5)     "read sensor - wrong address" [t9062]
(6)     "read register" [t9062]

Enter test for running.
...
```