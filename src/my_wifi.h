#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <Support.h>





class my_wifi
{
private:
    char *ssid;
    char *pass;
    int IndicatorPin;
    IPAddress local_IP;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress primaryDNS;
    IPAddress secondaryDNS; 

public:
    my_wifi(char *s_ssid, char *s_pass, int IndicatorPin);
    void init_wifi();
    void WIFI_Connect(IPAddress local_IP, IPAddress gateway, IPAddress subnet, 
                        IPAddress primaryDNS, IPAddress secondaryDNS, 
                        int wifidelay);
    void reconnect();
};


