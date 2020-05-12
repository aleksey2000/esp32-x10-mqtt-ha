
#include "mqtt.h"

mqtt::mqtt(PubSubClient &client, int IndicatorPin, char *mqttTop)
{
  this->IndicatorPin = IndicatorPin;
  this->client = &client; 

  mqttTopic = new char[strlen(mqttTop) + 1];
  strcpy(mqttTopic, mqttTop);
}


bool mqtt::connect(char *mqttClientName, char *mqttLogin, char *mqttPassword)
{
  return client->connect(mqttClientName, mqttLogin, mqttPassword);
}

bool mqtt::subscribe(char *topic)
{
    return client->subscribe(topic);
}

bool mqtt::publish(char *topic, char *message)
{
  return client->publish(topic, message);
}


void mqtt::sendMesasgeToMqtt(String SubTopicName, String message)

{

  digitalWrite(IndicatorPin, HIGH);

  String topicFull = String(mqttTopic) + "/" + String(SubTopicName);

  char topic_array[50];
  topicFull.toCharArray(topic_array, 50);

  char message_array[150];
  message.toCharArray(message_array, 150);
  
  client->publish(topic_array, message_array);

  digitalWrite(IndicatorPin, LOW);

  SubTopicName = new char[0];
  message = new char[0];
  
}


