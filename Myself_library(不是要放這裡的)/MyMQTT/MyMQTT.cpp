#include<MyMQTT.h>
#include<Arduino.h>
#include<PubSubClient.h>
#include<WiFiClient.h>
#include<WString.h>
#include<core_esp8266_features.h>
#include<HardwareSerial.h>


MyMQTT::MyMQTT(char* user,char* pw,char* out,char* in){
    WiFiClient mqtt;
    PubSubClient _MQTT_Client(mqtt);
    _user = user;
    _pw = pw;
    _out = out;
    _in = in;
}
void MyMQTT::init(String server,int port){
    _MQTT_Client.setServer(server.c_str(),port);
}
PubSubClient MyMQTT::getMQTTClient(){
    return _MQTT_Client;
}

void MyMQTT::reconnect(){
    while (!_MQTT_Client.connected())
    {
        const char* clientId = "icebox123";
        if(_MQTT_Client.connect(clientId,_user,_pw)){
            _MQTT_Client.publish(_out,"reconnect");
        }else{
            delay(3000);
        }
    }
    

}