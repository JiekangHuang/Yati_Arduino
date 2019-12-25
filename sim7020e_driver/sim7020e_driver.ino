/*
 Name:		sim7020e_driver.ino
 Created:	2019/12/24 下午 04:22:01
 Author:	s2862
*/
#include <TimedAction.h>
#include "sim7020e.h"

#ifndef AI7697_Serial
#define AI7697_Serial                  Serial
#endif // AI7697_Serial

#define TIME_OUT 20000

//#define TEST

String apn = "twm.nbiot";
String server = "io.adafruit.com";
String port = "1883";
String client_id = "myid";
String topic = "Zack_Huang/feeds/text";
String username = "Zack_Huang";
String password = "aio_rSXi37Kt5UjqzLCQJ5SNaUf1MHsp";

uint32_t pre_time;

void setup()
{
    AI7697_Serial.begin(115200);

    //----------------------------
    Oper_Select_t config;
    config.mode = 1;
    config.format = 2;
    config.location = "46697";
    uint8_t error = initSIM7020E(&apn, &config);
    AI7697_Serial.println(error);
}

void loop()
{
    String command;
    while (AI7697_Serial.available() > 0) {
        command = AI7697_Serial.readStringUntil('\r');
        SIM7020E_Serial.print(command + '\r');
    }
    while (SIM7020E_Serial.available() > 0) {
        command = SIM7020E_Serial.readStringUntil('\r');
        AI7697_Serial.println(command);
    }

#ifdef TEST
    mqtt_setting_t new_mqtt;
    new_mqtt.server = server;
    new_mqtt.port = port;
    new_mqtt.time_out = TIME_OUT_MS;
    new_mqtt.bufsize = MESS_BUFSIZE;

    mqtt_connect_t mqtt_connect;
    mqtt_connect.client_id = client_id;
    mqtt_connect.username = username;
    mqtt_connect.password = password;

    uint32_t arr[] = { 1234,12345,123456,1234567 };
    uint32_t *ptr = arr;
    uint32_t cur_time = millis();
    static uint8_t i = 0;
    if (cur_time - pre_time >= 10000) {
        pre_time = cur_time;
        uint8_t error = pushSIM7020E(&new_mqtt, &mqtt_connect, &topic, (ptr + i++));
        if (i == 4) {
            i = 0;
        }
    }
#endif
}