#ifndef MyMQTT_h
#define MyMQTT_h

#include "Arduino.h"
#include"PubSubClient.h"
#include"WiFiClient.h"
#include"WString.h"



class MyMQTT
{
    public:
        MyMQTT(char* user,char* pw,char* out,char* in);
        void init(String server,int port);
        void reconnect();
        PubSubClient getMQTTClient();
    private:
        PubSubClient _MQTT_Client;
        char* _user;
        char* _pw;
        char* _out;
        char* _in;
        WiFiClient mqtt;


        

};
#endif