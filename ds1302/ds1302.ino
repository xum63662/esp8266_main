#include<RtcDS1302.h>
#include<ThreeWire.h>

ThreeWire mywire(4,2,5);
RtcDS1302<ThreeWire> Rtc(mywire);

void setup(){
    Rtc.Begin();
    delay(3000);
    Serial.begin(9600);
    Serial.print(__DATE__);
    Serial.print(" ");
    Serial.print(__TIME__);
    Serial.println("");

    RtcDateTime compiled = RtcDateTime(__DATE__,__TIME__);
    printDateTime(compiled);
    Serial.println();

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
      Serial.println("RTC is older than compile time!  (Updating DateTime)");
      Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
      Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
      Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
    Serial.println("all seting is compiled");
}

void loop(){
  printTime(Rtc.GetDateTime());
  delay(1000);

}

void printDateTime(const RtcDateTime& dt)
{
  char datestring[26];

  snprintf_P(datestring,
          countof(datestring),
          PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
          dt.Month(),
          dt.Day(),
          dt.Year(),
          dt.Hour(),
          dt.Minute(),
          dt.Second());
  Serial.print(datestring);
}

void printTime(const RtcDateTime& dt){
  char datestring[20];
  snprintf_P(datestring, countof(datestring), PSTR("%02u:%02u"),dt.Hour(),dt.Minute());
  Serial.println(datestring);
}