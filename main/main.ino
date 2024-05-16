#include<MySQL_Connection.h>
#include<MySQL_Cursor.h>
#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<PubSubClient.h>
#include<DHT.h>
#include<LiquidCrystal_I2C.h>

#define relay_pin 13


#define button_up 14
#define button_down 12

//WIFI
const char* SSID = "TP-Link_73E0";
const char *PW = "88015779";
//MySQL
String server = "monsenqwq.tplinkdns.com";
char user[] = "remote";
char password[] = "Asdfg8520A";
IPAddress result;

//SQL command
//select
String select_temp = "SELECT temp FROM remote.value";
String select_target = "SELECT target FROM remote.value";
String select_relay = "SELECT relay FROM remote.value";
//update
String update_temp = "UPDATE remote.value SET temp = ";
String update_target = "UPDATE remote.value SET target = ";
String update_relay = "UPDATE remote.value SET relay = ";

WiFiClient client;
MySQL_Connection conn(&client);
MySQL_Cursor *cursor;
//MQTT
const char* mqtt_server = "broker.emqx.io";
PubSubClient MQTT_client(client);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
//LCD
LiquidCrystal_I2C lcd(0x27,16,2);
#define Backlight 10
//DHT
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

//value
#define target_max 30
#define target_min 11

int target = 27;
float temp = 1.00;
int relay = 0;

int now_target = 27;
float now_temp = 1.00;
int now_relay = 0;

int tmp_up = 0;
int tmp_down = 0;


//MQTT Function
void callback(char* topic,byte* payload,unsigned int length){
    if(payload[0] == '0'){
        MQTT_client.publish("outTopic","Up");
        tmp_up++;
        
    }else if(payload[1] == '1'){
        MQTT_client.publish("outTopic","Down");
        tmp_down++;
    }else{
        MQTT_client.publish("outTopic","NO THIS ONE");
    }
}

void reconnect(){
    while(!MQTT_client.connected()){
        String clientId = "ESP8266Client-";
        clientId = String(random(0xffff),HEX);
        if(MQTT_client.connect(clientId.c_str())){
            MQTT_client.publish("outTopic","reconnent test");
            MQTT_client.subscribe("inTopic");
        }else{
            delay(5000);
        }
    }
}

void MySQL_command(String choose,float temp,int target,int relay){
    conn.connect(IPAddress(192,168,0,99),8889,user,password);

    cursor = new MySQL_Cursor(&conn);
    if(choose == (String)"update"){
        String tmp = "";
        if(temp != 0){
            tmp = update_temp + (String)temp;
            cursor->execute(tmp.c_str());
        }if(target != 0){
            tmp = update_target +(String)target;
            cursor->execute(tmp.c_str());
        }if(relay != 2){
            tmp = update_relay +(String)relay;
            cursor->execute(tmp.c_str());
        }
    }
}



void setup(){
    analogWrite(Backlight,2);
    relay_switch(target);
    Serial.begin(115200);
    Serial.println(F("set setting..."));
    WiFi.begin(SSID,PW);
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(F("."));
        delay(100);
        
    }
    Serial.println(F("OK!"));
    Serial.println(WiFi.localIP());
    int err = WiFi.hostByName(server.c_str(),result);
    if(err == 1){
        Serial.print(F("IP:"));
        Serial.println(result);
    }else{
        Serial.print(F("Error code:"));
        Serial.println(err);
    }
    //MySQL
    
    db_reset();

    //MQTT
    MQTT_client.setServer(mqtt_server,1883);
    MQTT_client.setCallback(callback);
    MQTT_client.subscribe("inTopic");
    MQTT_client.publish("outTopic","Hello");
    //Button and Relay
    Serial.println(F("Hello,World!"));
    pinMode(button_down,INPUT);
    pinMode(button_up,INPUT);
   
    pinMode(relay_pin,OUTPUT);
    //lcd
    lcd.init();
    lcd.setCursor(0,0);
    lcd.backlight();
    dht.begin();


    
}
void loop(){
    //MQTT
    if(!MQTT_client.connected()){
        reconnect();
    }
    MQTT_client.loop();
    get_value();
    lcd_print(now_temp,now_target,now_relay);
    mqtt_sensor();
    relay_switch(relay);

    delay(1000);

    
    
    
}

void db_reset(){
    MySQL_command("update",temp,target,relay);
    Serial.println(F("MySQL Reset"));
}

void get_value(){
    now_temp = dht.readTemperature();
    Serial.println(now_temp);
    if(digitalRead(button_up) == LOW || tmp_up == 1 ){
        if(target < target_max){
            now_target++;
            tmp_up = 0;
        }
    }else if(digitalRead(button_down) == LOW || tmp_down == 1 ){
        if(target > target_min){
            now_target--;
            tmp_down = 0;
        }
    }
    Serial.println(now_target);
    if((now_temp - now_target) > 2 && relay == 0){
        now_relay = 1;
    }else if((now_temp - now_target) <=0.5&& relay == 1){
        now_relay = 0;
    }
    Serial.println(now_relay);
    delay(1000);

}

void lcd_print(float t,int tar,int re){
    lcd.setCursor(0,0);
    lcd.print(t);
    lcd.print(char(223));
    lcd.print("C");
    lcd.setCursor(12,0);
    lcd.print(tar);
    lcd.print(char(223));
    lcd.print("C");
    lcd.setCursor(6,1);
    if(re == 1){
        lcd.print(" ON");
    }else if(re == 0){
        lcd.print("OFF");
    }
}

void mqtt_sensor(){
    if(now_temp != temp){
        temp = now_temp;
        MySQL_command("update",temp,0,2);
    }if(now_target != target){
        target = now_target;
        MySQL_command("update",0,target,2);
    }if(relay != now_relay){
        relay = now_relay;
        MySQL_command("update",0,0,relay);
    }
      
}

void relay_switch(int switch_r){
    if(switch_r == 1){
        digitalWrite(relay_pin,HIGH);
    }else if(switch_r == 0){
        digitalWrite(relay_pin,LOW);
    }
}