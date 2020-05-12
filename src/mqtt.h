#include <Arduino.h>
#include <PubSubClient.h>


class mqtt
{
private:
    int IndicatorPin;
    char *mqttTopic;
    PubSubClient *client;

public:
    mqtt(PubSubClient &client, int IndicatorPin, char *mqttTopic);
    void sendMesasgeToMqtt(String SubTopicName, String message);
    bool connect(char *mqttClientName, char *mqttLogin, char *mqttPassword);
    bool subscribe(char *topic);
    bool publish(char *topic, char *message);
};

