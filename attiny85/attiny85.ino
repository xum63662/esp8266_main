int time = 0;

void setup(){
    pinMode(0,INPUT);
    pinMode(1,OUTPUT);
    digitalWrite(0,HIGH);
}

void loop(){
    if(time == 0 && digitalRead(0) == LOW){
        time++;
        digitalWrite(1,HIGH);
        while(digitalRead(0) == HIGH);
        delay(10);
    }if(time == 1 && digitalRead(0) == LOW){
        time--;
        digitalWrite(1,LOW);
        while(digitalRead(0) == HIGH);
        delay(10);
    }
}