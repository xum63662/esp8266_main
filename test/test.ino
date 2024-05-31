#include <LiquidCrystal_I2C.h>

#include<Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);


float temp = 33.5;
int target = 22;
int relay = 0;

void setup(){
    lcd.init();
    lcd.backlight();

}

void loop(){
    lcd.setCursor(0,0);
    lcd.print(temp);
    lcd.print(char(223));
    lcd.print("C");
    lcd.setCursor(12,0);
    lcd.print(target);
    lcd.print(char(223));
    lcd.print("C");
    lcd.setCursor(6,1);
    if(relay == 1){
        lcd.print(" ON");
    }else if(relay == 0){
        lcd.print("OFF");
    }
    delay(1000);
}