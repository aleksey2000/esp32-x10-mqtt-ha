
#include "HARestAPI.h"

/* Ask user to supply appropiate Client 
HARestAPI::HARestAPI(void)	{
	this->wclient = &dwclient;
}

void HARestAPI::useSSL(void) {
	_ssl = true;
	this->wsclient = &dwsclient;
} 
*/

HARestAPI::HARestAPI(WiFiClient &client)	{
	this->wclient = &client;
}

HARestAPI::HARestAPI(WiFiClientSecure &client)	{
	_ssl = true;
	this->wsclient = &client;
}

void HARestAPI::setHAServer(String HAServer) {
    _serverip = HAServer;
	if(_ssl)
	  _port = 443;
	else
	  _port = 8123;
}

void HARestAPI::setHAServer(String HAServer, uint16_t Port) {
    _serverip = HAServer;
	_port = Port;
}

void HARestAPI::setHAServer(String HAServer, uint16_t Port, String Password) {
    _serverip = HAServer;
	_port = Port;
	_password = Password;
	_passwordset = true;
}

void HARestAPI::setHAPassword(String Password) {
	_password = Password;
	_passwordset = true;
}

void HARestAPI::setRGB(uint8_t Red, uint8_t Green, uint8_t Blue) {
	_red = Red;
	_green = Green;
	_blue = Blue;
	_rgbset = true;
}

void HARestAPI::setDebugMode(bool Debug) {
	_debug = Debug;
}

void HARestAPI::setComponent(String Component) {
	_component = Component;
}

void HARestAPI::setURL(String URL) {
	_url = URL;
}

void HARestAPI::setFingerPrint(String FingerPrint) {
	_fingerprint = FingerPrint;
}

String HARestAPI::sendGetHA(String URL, String message) {
  String posturl, replystr;
  
  if(_debug) { Serial.print("Requesting URL: "); Serial.println(URL); }

  posturl =     "GET " + URL + " HTTP/1.1\r\n" +
                "Host: " + _serverip + ":" + _port + "\r\n" +
                "User-Agent: ESP8266 Rest API Client\r\n" +
                "Accept: */*\r\n";// +
           //    "Connection: close\r\n\r\n";
  if(_passwordset)
    //posturl = posturl + "x-ha-access: " + _password + "\r\n";               //legacy password
    posturl = posturl + "Authorization: Bearer " + _password + "\r\n";  //long-lived password
    posturl = posturl + "Content-Type: application/json" + "\r\n" +
                    "Content-Length: " + 3000 + "\r\n\r\n";
    //          "Content-Length: " + message.length() + "\r\n\r\n" + 
    //          message;
  if(_debug) {  Serial.println("Sending: "); Serial.println(posturl); }
	
    if(_debug){
      Serial.print("Connecting to ");
      Serial.println("IP: " + _serverip);
      //Serial.println("Port: " + _port);
    }
    if (!wclient->connect(_serverip.c_str(), _port)) {
      if(_debug) Serial.println("connection failed");
      return "Connection Failed";
    }
    

    wclient->setTimeout(1000);
    wclient->print(posturl);
    

    if(_debug) Serial.println("HTTP GET HA Request Sent!");
    
    
    Serial.println("");
    Serial.println("Recieved header:");
    Serial.println("");
    while (wclient->connected()) {
      String header = wclient->readStringUntil('\n');
      
      if (header == "\r") { 
        break; 
        } else 
        {
          Serial.println(header);
        }  // headers received
    }
	
  
  Serial.println("");
  Serial.println("Reading!");

  //wclient->flush();

//  if (wclient->available()) { wclient->flush(); wclient->stop(); }
      


  String line = wclient->readString();
  Serial.println("processing response...");


/*

String chunk = "";
int limit = 1;
String response="";

Serial.println(response);
do {
  if (wclient->connected()) {
  wclient->setTimeout(4000);
  chunk = wclient->readStringUntil('\n');
  response += chunk;
  Serial.println(chunk);
  }
} while (chunk.length() > 0 && ++limit < 1000);

*/

 // replystr = wclient->readString();
/*
  try
  {
    replystr = wclient->readStringUntil('\n');
  } catch (...) {
    Serial.println("Got exception");
  }
  Serial.println("Read done!");
*/
/*
    while (true) {
      String body = wclient->readStringUntil('\n');
      Serial.println("Recieving body");
      Serial.println( "B: " + body);
      if (body == "\r") break; // headers received
    }
*/

   
        //replystr = wclient->readString();

        

     // Serial.print(wclient->readString());
      

/*

DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(response.substring(bodypos));

  String device_token = root[String("user_code")];

*/


      Serial.println("Read done!");

      if (wclient->available()) { wclient->flush(); wclient->stop(); }
      
      
      if(_debug) {
        Serial.println("reply was:");
        Serial.println("==========");
        Serial.println(replystr);
        Serial.println("==========");
        Serial.println("closing connection");
      }
      return replystr;

      
  

}

bool HARestAPI::sendPostHA(String URL, String message) {
  bool reply;
  String posturl, replystr;
  
  if(_debug) { Serial.print("Requesting URL: "); Serial.println(URL); }
  
  posturl =     "POST " + URL + " HTTP/1.1\r\n" +
                "Host: " + _serverip + "\r\n" +
                "User-Agent: ESP8266 Rest API Client\r\n" +
                "Accept: */*\r\n";
  if(_passwordset)
    //posturl = posturl + "x-ha-access: " + _password + "\r\n";               //legacy password
    posturl = posturl + "Authorization: Bearer " + _password + "\r\n";  //long-lived password
    posturl = posturl + "Content-Type: application/json" + "\r\n" +
              "Content-Length: " + message.length() + "\r\n\r\n" + 
              message;

  if(_debug) {  Serial.println("Sending: "); Serial.println(posturl); }
	
  if(_ssl) {
	wsclient->setTimeout(1000);
    if(_debug){
      Serial.print("Connecting to ");
      Serial.println(_serverip);
    }
    if (!wsclient->connect(_serverip.c_str(), _port)) {
      if(_debug) Serial.println("connection failed");
      return false;
    }
    if(_debug & (_fingerprint.length() > 0) ) {
      if (wsclient->verify(_fingerprint.c_str(), _serverip.c_str())) {
        Serial.println("Certificate matches");
      } else {
        Serial.println("Certificate doesn't match");
      }
    }
    wsclient->print(posturl);
    if(_debug) Serial.println("HTTPS HA Request Sent!");
    while(wsclient->connected()){
      if(wsclient->available()){
        String line = wsclient->readStringUntil('\n');
        replystr += line + "\n";
        if (line == "\r") break;
      }
    }
    wsclient->flush();
    wsclient->stop();
  } else {
	wclient->setTimeout(1000);
    if(_debug){
      Serial.print("Connecting to ");
      Serial.println(_serverip);
    }
    if (!wclient->connect(_serverip.c_str(), _port)) {
      if(_debug) Serial.println("connection failed");
      return false;
    }
    wclient->print(posturl);
    if(_debug) Serial.println("HTTP HA Request Sent!");
    while(wclient->connected()){
      if(wclient->available()){
        String line = wclient->readStringUntil('\n');
        replystr += line + "\n";
        if (line == "\r") break;
      }
    }
    wclient->flush();
    wclient->stop();
  }
  
  if (replystr.startsWith("HTTP/1.1 200 OK")) {
    if(_debug) Serial.println("HA request successfull!");
    reply = true;
  } else {
    if(_debug) Serial.println("HA request failed");
    reply = false;
  }
  if(_debug) {
    Serial.println("reply was:");
    Serial.println("==========");
    Serial.println(replystr);
    Serial.println("==========");
    Serial.println("closing connection");
  }
  return reply;
}

bool HARestAPI::sendPostHA(String message) {
	return sendPostHA(_url, message);
}

bool HARestAPI::sendCustomHAData(String URL, String Message) {
	return sendPostHA(URL, Message);
}

bool HARestAPI::sendHA(void) {
	String Message = "{\"entity_id\":\"" + _component + "\"}";
	return sendPostHA(_url, Message);
}

bool HARestAPI::sendHAComponent(String Component) {
	String Message = "{\"entity_id\":\"" + Component + "\"}";
	return sendPostHA(_url, Message);
}

bool HARestAPI::sendHAComponent(String URL, String Component) {
	String Message = "{\"entity_id\":\"" + Component + "\"}";
	return sendPostHA(URL, Message);
}

bool HARestAPI::sendHAURL(String URL) {
	String Message = "{\"entity_id\":\"" + _component + "\"}";
	return sendPostHA(URL, Message);
}

bool HARestAPI::sendHALight(bool LightStatus) {
	String Message = "{\"entity_id\":\"" + _component + "\"}";
	if(LightStatus){
		return sendPostHA("/api/services/light/turn_on", Message);
	} else {
		return sendPostHA("/api/services/light/turn_off", Message);
	}
}

bool HARestAPI::sendHALight(bool LightStatus, String Component) {
	String Message = "{\"entity_id\":\"" + Component + "\"}";
	if(LightStatus){
		return sendPostHA("/api/services/light/turn_on", Message);
	} else {
		return sendPostHA("/api/services/light/turn_off", Message);
	}
}

bool HARestAPI::sendHASwitch(bool LightStatus) {
	String Message = "{\"entity_id\":\"" + _component + "\"}";
	if(LightStatus){
		return sendPostHA("/api/services/switch/turn_on", Message);
	} else {
		return sendPostHA("/api/services/switch/turn_off", Message);
	}
}

bool HARestAPI::sendHASwitch(bool LightStatus, String Component) {
	String Message = "{\"entity_id\":\"" + Component + "\"}";
	if(LightStatus){
		return sendPostHA("/api/services/switch/turn_on", Message);
	} else {
		return sendPostHA("/api/services/switch/turn_off", Message);
	}
}

bool HARestAPI::sendHARGBLight(void) {
	String Message = "{\"entity_id\":\"" + _component + "\"," +
                    "\"rgb_color\":["+ String(_red) + "," + String(_green) + "," + String(_blue) + "]}";
	return sendPostHA("/api/services/light/turn_on", Message);
}

bool HARestAPI::sendHARGBLight(String Component) {
	String Message = "{\"entity_id\":\"" + Component + "\"," +
                    "\"rgb_color\":["+ String(_red) + "," + String(_green) + "," + String(_blue) + "]}";
	return sendPostHA("/api/services/light/turn_on", Message);
}

bool HARestAPI::sendHARGBLight(uint8_t Red, uint8_t Green, uint8_t Blue) {
	String Message = "{\"entity_id\":\"" + _component + "\"," +
                    "\"rgb_color\":["+ String(Red) + "," + String(Green) + "," + String(Blue) + "]}";
	return sendPostHA("/api/services/light/turn_on", Message);
}

bool HARestAPI::sendHARGBLight(String Component, uint8_t Red, uint8_t Green, uint8_t Blue) {
	String Message = "{\"entity_id\":\"" + Component + "\"," +
                    "\"rgb_color\":["+ String(Red) + "," + String(Green) + "," + String(Blue) + "]}";
	return sendPostHA("/api/services/light/turn_on", Message);
}

bool HARestAPI::sendHAAutomation(bool AutoStatus) {
	String Message = "{\"entity_id\":\"" + _component + "\"}";
	if(AutoStatus){
		return sendPostHA("/api/services/automation/turn_on", Message);
	} else {
		return sendPostHA("/api/services/automation/turn_off", Message);
	}
}

bool HARestAPI::sendHAAutomation(bool AutoStatus, String Component) {
	String Message = "{\"entity_id\":\"" + Component + "\"}";
	if(AutoStatus){
		return sendPostHA("/api/services/automation/turn_on", Message);
	} else {
		return sendPostHA("/api/services/automation/turn_off", Message);
	}
}
