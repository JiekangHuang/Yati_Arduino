#pragma once

#define AI7697_Serial                    Serial
#define SIM7020E_Serial                  Serial1

#define SIM7020E_POWER_PIN               4
#define MESS_BUFSIZE                     100
#define TIME_OUT_MS                      10000
#define WAIT_RESPONSE_TIME               2000
#define WAIT_BOOTING                     5000

typedef enum {
    ERROR_NONE,
    ERROR_KEY,
    ERROR_PTR_NULL,
    ERROR_TIME_OUT
} ERR_CODE;

typedef enum {
    AUTOMATIC,
    MANUAL,
    MANUAL_DEREGISTER,
    SET_ONLY_FORMAT,
    MANUAL_FAILS_AUTOMATIC
} COPS_MODE;

typedef enum {
    LONG_FORMAT_ALPHANUMERIC,
    SHORT_FORMAT_ALPHANUMERIC,
    GSM_LOCATION
} COPS_FORMAT;

typedef struct {
    uint8_t mode;
    uint8_t format;
    String location;
} Oper_Select_t;

typedef struct {
    String server;
    String port;
    uint16_t time_out;
    uint16_t bufsize;
} mqtt_setting_t;

typedef struct {
    String client_id;
    String username;
    String password;
} mqtt_connect_t;

//Function
int initSIM7020E(String *apn, Oper_Select_t *cops_config);
int pushSIM7020E(mqtt_setting_t *mqtt_setting, mqtt_connect_t *mqtt_connect, String *topic, uint32_t *value);
