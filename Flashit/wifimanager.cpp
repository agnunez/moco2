extern "C" {
    #include "user_interface.h"  // Required for wifi_station_connect() to work
}

#include <Arduino.h>
#include <ESP8266WiFi.h>

#define FPM_SLEEP_MAX_TIME 0xFFFFFFF

void WiFiOn();
void WiFiOff();

//-------------------- USAGE ----------------------------------------------------------
/* 
void setup() {

    WiFiOn();
    delay(5000);
    WiFiOff();
    delay(5000);
    ESP.deepSleep(1e6 * 10, WAKE_RF_DEFAULT); // sleep 10 seconds

}

void loop() {
    ;
}
 */
//------------------------------------------------------------------------------

void WiFiOn() {

    wifi_fpm_do_wakeup();
    wifi_fpm_close();

    //Serial.println("Reconnecting");
    wifi_set_opmode(STATION_MODE);
    wifi_station_connect();
}


void WiFiOff() {

    //Serial.println("diconnecting client and wifi");
    //client.disconnect();
    wifi_station_disconnect();
    wifi_set_opmode(NULL_MODE);
    wifi_set_sleep_type(MODEM_SLEEP_T);
    wifi_fpm_open();
    wifi_fpm_do_sleep(FPM_SLEEP_MAX_TIME);

}