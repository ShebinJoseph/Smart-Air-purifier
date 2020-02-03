
#include <SoftwareSerial.h>
String apiKey = "ENTER API KEY HERE";
int IN1 = 4;
#define ON   0
#define OFF  1
SoftwareSerial ser(2, 3); // RX, TX || TX-->2 and RX-->3 for transmiting data
void setup()
{
 relay_init();//initialize the relay
 ser.begin(9600);
    Serial.begin(9600);
}

void loop()
{
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.print("AIR QUALITY:");
  Serial.print(voltage);
  Serial.println("ppm");
  if(voltage>0.22)
  {
    relay_SetStatus(ON);//turn on RELAY
   }
   else
   {
   relay_SetStatus(OFF);//turn off RELAY
   }
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(voltage);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.println(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    Serial.println("CIPCLOSE");
  }
    
  // thingspeak needs 15 sec delay between updates
 delay(16000);  
}

void relay_init(void)//initialize the relay
{
  //set all the relays OUTPUT
  pinMode(IN1, OUTPUT);
  //relay_SetStatus(OFF); //turn off all the relay
}

void relay_SetStatus(char status_1)
{
  digitalWrite(IN1, status_1);
}
