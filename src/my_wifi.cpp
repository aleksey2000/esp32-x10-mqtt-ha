
#include "my_wifi.h"


my_wifi::my_wifi(char *s_ssid, char *s_pass, int r_IndicatorPin)
{

  ssid = new char[strlen(s_ssid)+1];
  strcpy(ssid,s_ssid);

  pass = new char[strlen(s_pass)+1];
  strcpy(pass, s_pass);

  IndicatorPin = r_IndicatorPin;
}


void my_wifi::init_wifi()
{


  WiFiMode_t prepareWiFi_m = WiFi.getMode();
  WiFi.persistent(false);
  if (prepareWiFi_m == 3) {
    WiFi.mode(WIFI_AP);
  } else {
    WiFi.mode(WIFI_OFF);
  }
  WiFi.persistent(true);
  WiFi.mode(prepareWiFi_m);
  WiFi.persistent(false);

  WiFi.begin(this->ssid, this->pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}



void my_wifi::WIFI_Connect(IPAddress local_IP, IPAddress gateway, IPAddress subnet, IPAddress primaryDNS, IPAddress secondaryDNS, int wifidelay)
{

  Serial.println("Connecting WiFi");

  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);

  this -> local_IP = local_IP;
  this -> gateway = gateway;
  this -> subnet = subnet;
  this -> primaryDNS = primaryDNS;
  this -> secondaryDNS = secondaryDNS;

  Serial.print("Local ip: ");
  Serial.println(this->local_IP);

  Serial.print("Gateway: ");
  Serial.println(this->gateway);

  Serial.print("Subnet: ");
  Serial.println(this->subnet);

  Serial.print("SSID: ");
  Serial.println(this->ssid);

  Serial.print("SSID Password: ");
  Serial.println(this->pass);


  for (int k = 0; k<25; k++) 
    {
      if ( WiFi.status() != WL_CONNECTED )
        {
          
          Serial.println("Wifi not connected");
          Serial.print("Running attemp #");
          Serial.println(k);

          wifidelay = wifidelay + 250;
          WiFi.disconnect();
          delay(wifidelay);
          WiFi.mode(WIFI_STA);
          delay(wifidelay);
          WiFi.begin(ssid, pass);
          delay(wifidelay);
          delay(wifidelay);
        } else
        {
          Serial.println("Wifi connected succsessfuly.");
          k=25;
        }
    }
  if ( WiFi.status() != WL_CONNECTED )
    {
    delay (10000);
      Support::hard_restart();
    delay (2000);
    }
}


void my_wifi::reconnect() {

    if ( WiFi.status() != WL_CONNECTED )
        {
           digitalWrite(IndicatorPin, HIGH);
           WiFi.disconnect();
           WiFi.mode(WIFI_STA);
           WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
           delay(1000);
           WiFi.begin(ssid, pass);
           digitalWrite(IndicatorPin, LOW);
          
           delay(2000);
           if ( WiFi.status() != WL_CONNECTED )
           {
             for(int i = 0; i<5; i++)
             {
              digitalWrite(IndicatorPin, HIGH);
              delay(500);
              digitalWrite(IndicatorPin, LOW);
             }
            
              Support::hard_restart();       
              delay (2000);     
            }
        }
}