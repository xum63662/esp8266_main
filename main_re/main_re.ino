#include<MySQL_Connection.h>
#include<MySQL_Cursor.h>
#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<PubSubClient.h>
#include<DHT.h>
#include<LiquidCrystal_I2C.h>
#include<MyMQTT.h>
#include<MySQL.h>
#include <Ticker.h>

//pin
#define relay_pin 13
#define button_up 14
#define button_down 12
#define DHTPin 2
#define Backlight 10
//other define
#define DHTType DHT11
#define target_max 30
#define target_min 11
#define mqtt_out "outTopic"
#define mqtt_in "inTopic"
//sql command and mqtt user
//sql command
char com_select[3][50] = {{"SELECT temp FROM remote.value"},
                    {"SELECT target FROM remote.value"},
                    {"SELECT relay FROM remote.value"}};
char com_update[3][50]={{"UPDATE remote.value SET temp = "},
                    {"UPDATE remote.value SET target = "},
                    {"UPDATE remote.value SET relay"}};
//選擇
#define temp_sql 0
#define target_sql 1
#define relay_sql 2

//user and pw
//Wi-Fi
const char* SSID = "iPhone";
const char* PW = "abcd5678";
//MySQL
String server = "monsenhome.ddns.net";
char user_sql[] = "remote";
char pw_sql[] = "Asdfg8520A";
//MQTT
char* user_mqtt = "ref";
char* pw_mqtt = "Asdfg8520A";

//mysql setting
IPAddress result; 
WiFiClient MySQL_client;
MySQL_Connection conn(&MySQL_client);
MySQL_Cursor *cursor;
//MQTT setting
MyMQTT mqtt(user_mqtt,pw_mqtt,mqtt_out,mqtt_in);
PubSubClient MQTT_client;
//LCD setting
LiquidCrystal_I2C lcd(0x27,16,2);
//DHT setting 
DHT dht(DHTPin,DHTType);
//Tickker setting
Ticker ticker;

//setting default
int temp = 1.00;
int target = 27;
int relay = 0;

int now_temp = temp;
int now_target = target;
int now_relay = relay;

int but_up = 0;
int but_down = 0;

//mqtt function
void callback(char* topic,byte* payload,unsigned int lenght){
    Serial.println(payload[1]);
    if(payload[0] == '0'){
        MQTT_client.publish(mqtt_out,"Up");
        but_up++;
    }else if(payload[0] == '1'){
        MQTT_client.publish(mqtt_out,"Down");
        but_down++;
    }
}

void setup(){
    //lcd begin
    lcd.init();
    lcd.setCursor(0,0);
    lcd.backlight();
    WiFi.begin(SSID,PW);
    Serial.begin(115200);
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(F("."));
        delay(100);
    }
    int err = WiFi.hostByName(server.c_str(),result);
    //MQTT
    mqtt.init(server,1883);
    MQTT_client = mqtt.getMQTTClient();
    MQTT_client.setCallback(callback);
}

void loop(){
   

}

void lcd_print(){
    
}