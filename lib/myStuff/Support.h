#ifndef Support_h
#define Support_h

#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#include <ArduinoHttpClient.h>
#include "WiFiClient.h"
#include <ArduinoJson.h>


class Support
{

    public:

    struct LightData {
          bool isOn = false;
          int brightness = 0;
          bool isFetched = false;
        } ;

    struct ServerData {
            String ha_ip;
            uint16_t ha_port;
            String password;
        } ;

        static void hard_restart();
        static LightData getUnitState(WiFiClient &wifiClient, ServerData serverData, String unit_id);
        static void blink_times_hi_low(int IndicatorPin, int times);
        static void blink_times(int IndicatorPin, int times);
        static void blink_times_low_hi(int IndicatorPin, int times);
        static void blink_times_w_delay(int IndicatorPin, int times, int delayVal);
      
};

struct LightData {
          bool isOn = false;
          int brightness = 0;
        } ;

struct ServerData {
            String ha_ip;
            uint16_t ha_port;
            String password;
        } ;

#endif

