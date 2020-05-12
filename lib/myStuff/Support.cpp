#include "Support.h"


void Support::hard_restart()
{
     Serial.println("Hard resetting...");
    esp_task_wdt_init(1,true);
    esp_task_wdt_add(NULL);
    while(true);
}


Support::LightData Support::getUnitState(WiFiClient &wifiClient, Support::ServerData serverData, String unit_id)
{
    Support::LightData result;

    HttpClient httpclient = HttpClient(wifiClient, serverData.ha_ip, serverData.ha_port);


    String path = "/api/states/" + unit_id;
    httpclient.beginRequest();
    httpclient.get(path);
    httpclient.sendHeader("User-Agent", "ESP8266 Rest API Client");
    httpclient.sendHeader("Authorization", serverData.password);
    httpclient.endRequest();

    int statusCode = httpclient.responseStatusCode();
    String response = httpclient.responseBody();


          if(statusCode == 200)
          {

             DynamicJsonDocument doc(1024);
             auto error = deserializeJson(doc, response); 

            if (error) {
                  Serial.print(F("deserializeJson() failed with code "));
                  Serial.println(error.c_str());
              }

              String state = doc["state"];

              if(state == "off")
              {
                      result.isOn = false;
              } else
              {       
                    int brightness = doc["attributes"]["brightness"];

                    result.isOn = true;
                    result.brightness = brightness;
              }

              result.isFetched = true;
          }
    return result;
}


void Support::blink_times_hi_low(int IndicatorPin, int times)
{
  for(int i = 0; i < times; i++)
            {
              digitalWrite(IndicatorPin, HIGH);
              delay(200);
              digitalWrite(IndicatorPin, LOW);
            }
}

void Support::blink_times(int IndicatorPin, int times)
{
  digitalWrite(IndicatorPin, LOW);

  for(int i = 0; i < times; i++)
            {
              
              digitalWrite(IndicatorPin, HIGH);
              delay(500);
              digitalWrite(IndicatorPin, LOW);
              delay(500);
            }
}

void Support::blink_times_w_delay(int IndicatorPin, int times, int delayVal)
{
  digitalWrite(IndicatorPin, LOW);

  for(int i = 0; i < times; i++)
            {
              
              digitalWrite(IndicatorPin, HIGH);
              delay(delayVal);
              digitalWrite(IndicatorPin, LOW);
              delay(delayVal);
            }
}


void Support::blink_times_low_hi(int IndicatorPin, int times)
{
  for(int i = 0; i < times; i++)
            { 
              digitalWrite(IndicatorPin, LOW);
              delay(500);
              digitalWrite(IndicatorPin, HIGH);
            }
}




