#include <Arduino.h>
#include "sim7020e.h"

static String _intToHexString(uint32_t *num);
static uint32_t _HexStringToint(String *num);
static int _setSIM7020Ecommand(String *command);

String _intToHexString(uint32_t *num)
{
    String str = String(*num);
    String hex_str;
    for (int i = 0; i < str.length(); i++) {
        hex_str += String(str[i], HEX);
    }
    return hex_str;
}

uint32_t _HexStringToint(String * str)
{
    return 0;
}

int _setSIM7020Ecommand(String *command)
{
    String str;
    uint32_t pre_time;
    AI7697_Serial.println(*command);
    SIM7020E_Serial.print(*command + '\r');
    delay(WAIT_RESPONSE_TIME);
    pre_time = millis();
    do
    {
        delay(500);
        str = SIM7020E_Serial.readStringUntil('\n');
        AI7697_Serial.print('.');
        if (millis() - pre_time > TIME_OUT_MS) {
            AI7697_Serial.println("time out !!");
            return ERROR_TIME_OUT;
        }
    } while (str != "OK\r" && str != "ERROR\r");
    AI7697_Serial.println(str);

    if (str == "OK\r") {
        return ERROR_NONE;
    }
    else {
        return ERROR_KEY;
    }
}

int initSIM7020E(String * apn, Oper_Select_t * cops_config)
{
    if (apn == NULL || cops_config == NULL) {
        return ERROR_PTR_NULL;
    }

    //-----------------------------------
    SIM7020E_Serial.begin(38400);
    pinMode(SIM7020E_POWER_PIN, OUTPUT);
    digitalWrite(SIM7020E_POWER_PIN, LOW);
    delay(WAIT_BOOTING);

    String cgd_cont = "AT+CGDCONT=1,\"IP\",\"" + *apn + "\"";
    String cops = "AT+COPS=" + String(cops_config->mode) + ',' + \
        String(cops_config->format) + ",\"" + cops_config->location + "\"";

    String cmd = "AT";
    if (_setSIM7020Ecommand(&cmd) == ERROR_NONE) {
        AI7697_Serial.println(_setSIM7020Ecommand(&cgd_cont));
        if (_setSIM7020Ecommand(&cops) == ERROR_NONE) {
            return ERROR_NONE;
        }
    }
    return ERROR_KEY;
}

int pushSIM7020E(mqtt_setting_t * mqtt_setting, mqtt_connect_t * mqtt_connect, String * topic, uint32_t *value)
{
    if (mqtt_setting == NULL || mqtt_connect == NULL || topic == NULL || value == NULL) {
        return ERROR_PTR_NULL;
    }

    //--------------------------------
    const String CMQCON = "AT+CMQCON=0,3,\"MYID\",5000,0,0";

    String cmq_new = "AT+CMQNEW=\"" + mqtt_setting->server + "\",\"" + \
        mqtt_setting->port + "\"," + String(mqtt_setting->time_out) + ',' + \
        String(mqtt_setting->bufsize);

    String cmq_con = "AT+CMQCON=0,3,\"" + mqtt_connect->client_id + "\"," + \
        String(TIME_OUT_MS) + ",0,0,\"" + mqtt_connect->username + "\",\"" + \
        mqtt_connect->password + "\"";

    String message = _intToHexString(value);
    String cmq_pub = "AT+CMQPUB=0" + (",\"" + *topic + "\",0,0,0,") +
        String(message.length()) + ",\"" + message + "\"";

    String clear_mqttid = "AT+CMQDISCON=0";
    _setSIM7020Ecommand(&clear_mqttid);
    if (_setSIM7020Ecommand(&cmq_new) == ERROR_NONE) {
        if (_setSIM7020Ecommand(&cmq_con) == ERROR_NONE) {
            if (_setSIM7020Ecommand(&cmq_pub)) {
                return ERROR_NONE;
            }
        }
    }
    return ERROR_KEY;
}