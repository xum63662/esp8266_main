#include<ESP8266WiFi.h>
#include<MySQL_Connection.h>
#include<MySQL_Cursor.h>
#include<LiquidCrystal_I2C.h>
#include<WiFiClient.h>

WiFiClient client;
MySQL_Connection conn(&client);\
MySQL_Cursor *cursor;
void setup(){
    Serial.begin(9600);

    
}

void loop(){
    Serial.println("test");
    delay(1000);
}