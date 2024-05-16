void setup(){
    Serial.begin(9600);
}
float finel_temp;
int finel_target;
boolean finel_relay; 

void SQL(String choose,float value_float,int value_int,boolean value_boolean){
    String command = "";
    if(choose ==(String)"select"){
        if(value_float != NULL){
            command = "SELECT temp FROM value";
        }if(value_int != NULL){
            command = "SELECT target FROM value";
        }if(value_boolean != NULL){
            command = "SELECT relay FROM value";
        }

    }else if(choose ==(String)"update"){
        if(value_float != NULL){
            command = "UPDATE value SET temp = " + (String)value_float;
        }if(value_int != NULL){
            command = "UPDATE value SET temp = " + (String)value_int;
        }if(value_boolean != NULL){
            if(value_boolean == 1){
                command = "UPDATE value SET temp = "+ (String)"YES";
            }else{
                command = "UPDATE value SET temp = "+(String)"NO";
            }
            
        }
    }else{
        Serial.print("ERROR choose");
    }
    Serial.println(command);
}

void loop(){
    Serial.println("Enter command(select or update)");
    while(!Serial.available());
    String result_command = Serial.readString();
    Serial.println("Enter temp(number or NULL)");
    while(!Serial.available());
    String result_float = Serial.readString();
    if(result_float == "NULL"){
        finel_temp = NULL;
    }else{finel_temp = result_float.toFloat();}
    Serial.println("Enter target(number or NULL)");
    while(!Serial.available());
    String result_int = Serial.readString();
    if(result_int == "NULL"){
        finel_target == NULL;
    }else{finel_target = result_int.toInt();}
    Serial.println("Enter relay(YES or NO)");
    while(!Serial.available());
    String result_relay = Serial.readString();
    if(result_relay == "NULL"){
        finel_relay = NULL;
    }else if(result_relay == "YES"){finel_relay == 1;}
    else if(result_relay == "NO"){finel_relay == 0;}
    Serial.println("OUTPUT");
    SQL(result_command,finel_temp,finel_target,finel_relay);
    delay(2000);

}