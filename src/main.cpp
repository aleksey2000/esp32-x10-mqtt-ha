#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <HARestAPI.h>

#include <SPI.h>

#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <x10.h>
#include <x10constants.h>

#include <Support.h>
#include <web.h>
#include <my_wifi.h>
#include <x10_MySupport.h>
#include <mqtt.h>


#include <ArduinoHttpClient.h>



char progVerison[] = "0.0.1-init";

int status = WL_IDLE_STATUS;

//Hardware config

#define zcPin 4         // the zero crossing detect pin
#define dataPin 17       // the TX X10 data out pin
#define dataRecievePin 16       // the TX X10 data out pin
#define repeatTimes 3   // how many times each X10 message should repeat
#define IndicatorPin   2 // Blue indicator pin

// Hardware config


#ifndef SECRET

// Network config

char ssid[] = ""; // <--  PUT YOUR WIFI SSID HERE
char pass[] = ""; // <--  PUT YOUR WIFI PASS HERE

IPAddress local_IP(192, 168, 8, 2); // <--  PUT YOUR ESP32 IP HERE
IPAddress gateway(192, 168, 8, 1); // <--  PUT YOUR GATEWAY IP HERE
IPAddress subnet(255, 255, 255, 0); // <--  PUT YOUR SUBNET MASK HERE
IPAddress primaryDNS(4, 4, 4, 4);  // <--  PUT YOUR DNS 1 HERE
IPAddress secondaryDNS(8, 8, 8, 8); // <--  PUT YOUR DNS 2 HERE

char mqttClientName[] = "light_esp32"; // <--  PUT YOUR ESP32 MQTT ID HERE
char mqttLogin[] = ""; // <--  PUT YOUR MQTT LOGIN HERE
char mqttPassword[] = ""; // <--  PUT YOUR MQTT PASS HERE

// Network config

//Home assistant config


IPAddress server(192,168,8,1); // <--  PUT YOUR HOME ASSISTANT IP HERE
const char* ha_ip = ""; // <--  PUT YOUR HOME ASSISTANT IP HERE
uint16_t ha_port = 8123; // Could be 443 is using SSL
const char* ha_pwd = "Bearer ";  //long-lived password. On HA, Profile > Long-Lived Access Tokens > Create Token. Put it after "Bearer " word
String fingerprint = ""; // Certificate fingerprint in case ssl (not tested)

//Home assistant config

#endif


volatile bool unit1_on = false;
volatile bool unit2_on = false;
volatile bool unit3_on = false;

volatile int unit1_level = -1;
volatile int unit2_level = -1;
volatile int unit3_level = -1;

// use for no hardreset at the fist loop
bool newstart = 0;
int wifidelay = 200;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

void sendUpdate();

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);
WebServer webServer(80);


char mqttTopic[] = "light/STATE";
char mqttCommandTopic[] = "lightCommand";

char unit_1_CommandTopic[] = "lightCommand/unit_1";
char unit_2_CommandTopic[] = "lightCommand/unit_2";
char unit_3_CommandTopic[] = "lightCommand/unit_3";

char unit_1_DimCommandTopic[] = "lightCommand/unit_1_dim";
char unit_2_DimCommandTopic[] = "lightCommand/unit_2_dim";
char unit_3_DimCommandTopic[] = "lightCommand/unit_3_dim";

char unit_1_BrightCommandTopic[] = "lightCommand/unit_1_bright";
char unit_2_BrightCommandTopic[] = "lightCommand/unit_2_bright";
char unit_3_BrightCommandTopic[] = "lightCommand/unit_3_bright";


char callbackCrap[100];
char x10debugMessage[100];

//int debug_counter = 0;
int max_bright = 20;
float max_bright_float = max_bright;

const char* host = "esp32_light"; // <--  PUT YOUR ESP32 NETWORK ID HERE
int mqttSendIntervalSeconds = 5;


int internalTicksCounter = 0;


//NEW
my_wifi my_wifi_Client(ssid, pass, IndicatorPin);
web web_client(webServer, progVerison);
x10_MySupport myX10;
mqtt myMqtt(client, IndicatorPin, mqttTopic);
//NEW



void callback(char* topic, byte* payload, unsigned int length) {

  String topicStr = topic;
  memcpy(callbackCrap, (char *)payload, length);
  callbackCrap[length] = '\0';

  Serial.print("Got callback for: ");
  Serial.println(topicStr);
 
  
       if (topicStr == "lightCommand/1d")
          {
            myX10.unit_dim (1, atoi(callbackCrap), IndicatorPin);
          }

       if (topicStr == "lightCommand/1m")
          {
            myX10.unit_bright (1, atoi(callbackCrap), IndicatorPin);
          }

        if (topicStr == "lightCommand/1s")
          {
            myX10.getStatus(IndicatorPin);
          }

        if (topicStr == "lightCommand/1h")
          {
            myX10.getHAIL(IndicatorPin);
          }

// ONOFF
  
 if (topicStr == "lightCommand/unit_1")
    {
      if (!strncmp((char *)payload, "ON", length)) {
          unit1_on = true;
          
        } else if (!strncmp((char *)payload, "OFF", length)) {
          unit1_on = false;
          myX10.unit_off(1, IndicatorPin);
        }
    }
  if (topicStr == "lightCommand/unit_2")
    {
      if (!strncmp((char *)payload, "ON", length)) {
          unit2_on = true; 
        } else if (!strncmp((char *)payload, "OFF", length)) {
          myX10.unit_off(2, IndicatorPin);
          unit2_on = false;
        }
    }

  if (topicStr == "lightCommand/unit_3")
    {
      if (!strncmp((char *)payload, "ON", length)) {
          unit3_on = true;   
        } else if (!strncmp((char *)payload, "OFF", length)) {
          myX10.unit_off(3, IndicatorPin);
          unit3_on = false;
        }
    }

// DIM AND BRIGHT



       if (topicStr == "lightCommand/unit_1_dim")
        {
         if(atoi(callbackCrap) >= max_bright / 2)
          {
              myX10.unit_bright(1, max_bright, IndicatorPin);
              myX10.unit_dim(1, max_bright - atoi(callbackCrap), IndicatorPin);
            }
            else
            {
              myX10.unit_bright(1, 1, IndicatorPin);
              myX10.unit_dim(1, max_bright, IndicatorPin);
              myX10.unit_bright(1, atoi(callbackCrap), IndicatorPin);
          }


          unit1_on = true;
          unit1_level =  atoi(callbackCrap);       
        }
    
       if (topicStr == "lightCommand/unit_2_dim")
        {

          Serial.println("Adjusting unit2");

          if(atoi(callbackCrap) >= max_bright / 2)
          {
              Serial.println("Unit2 dim up");

              myX10.unit_bright(2, max_bright, IndicatorPin);

              Serial.println("Unit2 dim bright done");

              myX10.unit_dim(2, max_bright - atoi(callbackCrap), IndicatorPin);

              Serial.println("Unit2 dim up done");
            }
            else
            {
              Serial.println("Unit2 dim low");

              myX10.unit_bright(2, 1, IndicatorPin);
              myX10.unit_dim(2, max_bright, IndicatorPin);
              myX10.unit_bright(2, atoi(callbackCrap), IndicatorPin);

              Serial.println("Unit2 dim low done");
          }


          unit2_on = true;
          unit2_level = atoi(callbackCrap);
        }
    
       if (topicStr == "lightCommand/unit_3_dim")
        {
          if(atoi(callbackCrap) >= max_bright / 2)
          {
              myX10.unit_bright(3, max_bright, IndicatorPin);
              myX10.unit_dim(3, max_bright - atoi(callbackCrap), IndicatorPin);
            }
            else
            {
              myX10.unit_bright(3, 1, IndicatorPin);
              myX10.unit_dim(3, max_bright, IndicatorPin);
              myX10.unit_bright(3, atoi(callbackCrap), IndicatorPin);
          }


          unit3_on = true;
          unit3_level = atoi(callbackCrap);
        }   

  sendUpdate();
}


void blink_times_hi_low(int times)
{
  for(int i = 0; i < times; i++)
            {
              digitalWrite(IndicatorPin, HIGH);
              delay(200);
              digitalWrite(IndicatorPin, LOW);
            }
}

void blink_times_low_hi(int times)
{
  for(int i = 0; i < times; i++)
            { 
              digitalWrite(IndicatorPin, LOW);
              delay(200);
              digitalWrite(IndicatorPin, HIGH);
            }
}


bool try2fetchDataFromHA(int maxIterations)
{

Serial.println("Fetching state from HomeAssistant");

Support::ServerData serverData;

    serverData.ha_ip = ha_ip;
    serverData.ha_port = ha_port;
    serverData.password = ha_pwd;

    Support::LightData lightData1 = Support::getUnitState(wifiClient, serverData, "light.unit_1");
    Support::LightData lightData2 = Support::getUnitState(wifiClient, serverData, "light.unit_2");
    Support::LightData lightData3 = Support::getUnitState(wifiClient, serverData, "light.unit_3");

    int iterator = 0;

    Serial.println("Fetching lightData1");
    while (!lightData1.isFetched || iterator < maxIterations) 
                {
                  lightData1 = Support::getUnitState(wifiClient, serverData, "light.unit_1");
                  
                  Serial.print("Attempt #");
                  Serial.println(iterator);
                  Serial.print("Result: ");
                  Serial.println(lightData1.isFetched);
                  if(lightData1.isFetched)
                  {
                    break;
                  }
                  
                  iterator++;
                  delay(2000);
                  blink_times_hi_low(5);

                  
                }

    Serial.println("Done");

    if(!lightData1.isFetched) return false;

    iterator = 0;

    Serial.println("Fetching lightData2");
   
    while (!lightData2.isFetched || iterator < maxIterations)
                {
                  lightData2 = Support::getUnitState(wifiClient, serverData, "light.unit_2");
                  
                  Serial.print("Attempt #");
                  Serial.println(iterator);
                  Serial.print("Result: ");
                  Serial.println(lightData2.isFetched);
                  if(lightData2.isFetched)
                  {
                    break;
                  }
                  
                  iterator++;
                  delay(2000);
                  blink_times_hi_low(5);

                  

                }

    Serial.println("Done");

    if(!lightData2.isFetched) return false;

    iterator = 0;     

    Serial.println("Fetching lightData3");

    while (!lightData3.isFetched || iterator < maxIterations)
                {
                  lightData3 = Support::getUnitState(wifiClient, serverData, "light.unit_3");
                  
                  Serial.print("Attempt #");
                  Serial.println(iterator);
                  Serial.print("Result: ");
                  Serial.println(lightData3.isFetched);
                  if(lightData3.isFetched)
                  {
                    break;
                  }

                  iterator++;
                  delay(2000);
                  blink_times_hi_low(5);

                  
                }

    Serial.println("Done");

    if(!lightData3.isFetched) return false;

    if(lightData1.isOn)
                {
                  unit1_level = lightData1.brightness /  (255 / max_bright_float);
                  unit1_on = true;
                }
    if(lightData2.isOn)
                {
                  unit2_level = lightData2.brightness /  (255 / max_bright_float);
                  unit2_on = true;
                }
    if(lightData3.isOn)
                {
                  unit3_level = lightData3.brightness /  (255 / max_bright_float);
                  unit3_on = true;
                }

    return true;

}


void sendUpdate()
{
  String u1 = "";
            if (unit1_on) {
             u1 = "{ \"State\": \"ON\",  \"Value\": \"" ;
            } else {
             u1 = "{ \"State\": \"OFF\", \"Value\": \"";
            }
            
            u1.concat(String(unit1_level));
            u1.concat("\"}") ;

            myMqtt.sendMesasgeToMqtt("UNIT_1", u1);

            String u2 =  (unit2_on) ? "{ \"State\": \"ON\",  \"Value\": \"" : "{ \"State\": \"OFF\", \"Value\": \"";
            u2.concat(String(unit2_level));
            u2.concat("\"}") ;
            myMqtt.sendMesasgeToMqtt("UNIT_2", u2);

            String u3 =  (unit3_on) ? "{ \"State\": \"ON\",  \"Value\": \"" : "{ \"State\": \"OFF\", \"Value\": \"";
            u3.concat(String(unit3_level));
            u3.concat("\"}") ;
            myMqtt.sendMesasgeToMqtt("UNIT_3", u3);

}

void setup() {
  
//Serial.begin(115200);
Serial.begin(57600);
Serial.println("");
Serial.printf("Firmware version: %s \r\n", progVerison);



 delay(10);

    Serial.println("Starting init");

    pinMode(IndicatorPin, OUTPUT);

    digitalWrite(IndicatorPin, HIGH);
    newstart = 1;

    my_wifi_Client.WIFI_Connect(local_IP, gateway, subnet, primaryDNS, secondaryDNS, wifidelay);


    if (!MDNS.begin(host)) { //http://esp32_light.local
    Serial.println("Error setting up MDNS responder!");
        while (1) {
          delay(1000);
        }
      }
    Serial.println("mDNS responder started");
    
    web_client.webServerInit();

    digitalWrite(IndicatorPin, LOW);

    delay(200);

    digitalWrite(IndicatorPin, HIGH);


    delay(200);
    digitalWrite(IndicatorPin, LOW);



    if(!try2fetchDataFromHA(10))
    {
      Serial.println("Init from HA failed");
     // reset_all_lights();
    }

  Serial.println("Strting mqtt");

   if (myMqtt.connect(mqttClientName, mqttLogin, mqttPassword)) {
        
        Serial.println("Authenticated in MQTT");

        myMqtt.subscribe(unit_1_CommandTopic);
        myMqtt.subscribe(unit_2_CommandTopic);
        myMqtt.subscribe(unit_3_CommandTopic);

        myMqtt.subscribe(unit_1_DimCommandTopic);
        myMqtt.subscribe(unit_2_DimCommandTopic);
        myMqtt.subscribe(unit_3_DimCommandTopic);

        myMqtt.subscribe(unit_1_BrightCommandTopic);
        myMqtt.subscribe(unit_2_BrightCommandTopic);
        myMqtt.subscribe(unit_3_BrightCommandTopic);

        char oneD[] = "lightCommand/1d";
        char oneM[] = "lightCommand/1m";
        char oneS[] = "lightCommand/1s";
        char oneH[] = "lightCommand/1h";

        myMqtt.subscribe(oneD);
        myMqtt.subscribe(oneM);
        myMqtt.subscribe(oneS);
        myMqtt.subscribe(oneH);

        Serial.println("MQTT subscribed");


        char lighR[] = "lightResp";
        char subs[] = "Subscribed";

        myMqtt.publish(lighR, subs);

        Serial.println("MQTT published state");

      }


    delay(200);
    digitalWrite(IndicatorPin, LOW);

myX10.init(zcPin, dataPin, dataRecievePin);

// Serial.begin(57600);
 Serial.println("Started");



internalTicksCounter = 1000 * mqttSendIntervalSeconds;

Support::blink_times(IndicatorPin, 5);
}

void loop() {

  
  webServer.handleClient();
   client.loop();
   delay(1);

    if (myX10.received()) { 
        myX10.process();
    //    debug_counter++;
    } 

    if(internalTicksCounter == 1000 * mqttSendIntervalSeconds)
      {  
          if ( WiFi.status() != WL_CONNECTED )
            { 
              my_wifi_Client.reconnect();
              Serial.println("Reconnecting wifi");
              Support::blink_times(IndicatorPin, 10);
            }

        delay(5000);

        internalTicksCounter = 0;
        Support::blink_times_w_delay(IndicatorPin, 1, 200);
      }
            
      else
      {
        if(internalTicksCounter >= 10 * 1000 * mqttSendIntervalSeconds)
        {
          internalTicksCounter = 0;
        }

        internalTicksCounter++;
      }
      
}

