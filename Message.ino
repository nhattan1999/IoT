#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <string.h>
#include <Servo.h>

//ID va Password cua Wifi
const char ssid[] = "Android";  
const char pass[] = "12071998";       

WiFiClient  client;
 
Servo myservo; 
int pos = 0;

//ID, Read/Write API key cua channel
unsigned long counterChannelNumber = 1081839;            
const char * myCounterReadAPIKey = "INA26RGN62MB324E"; 
const char * myCounterWriteAPIKey = "LVV1HPZWM4B8IAQS";
const int FieldNumber1 = 1;  
boolean check = true;

//Khoi tao 
void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  myservo.attach(14);  //Chan tin hieu ket noi Servo (GPIO14)

  //Kiem tra xem da ket noi duoc vơi Wifi hay chua
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println(" ....");
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      delay(5000);
    }
    Serial.println("Connected to Wi-Fi Succesfully.");
  }
}

//Cac gia tri hop le
char s1[]= "Thong";
char s2[]= "Tan";
char s3[]= "Duyen";
char s4[]= "Tri";
int c = 1;

//Vong lap vo han
void loop()
{
  String name = ThingSpeak.readStringField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey); //Lay du lieu cua FieldNumber1 tu ThingSpeak
  Serial.println(name); //In name ra Serial Monitor
  name = name + '*';
  int i = 0;

  char s[20];
  while(name[i]!='*') //Chuyen du lieu tu name sang s
  { s[i] = name[i];
    i++;
  }

  int d1, d2, d3, d4;
  d1 = d2 = d3 = d4 =0;
  
  //Dem so luong phan tư giong nhau cua s so voi s1, s2, s3, s4
  for(int j=0; j<i; j++)  
  {
    if(s[j] == s1[j])
      d1++;
    if(s[j] == s2[j])
      d2++;
    if(s[j] == s3[j])
      d3++;
    if(s[j] == s4[j])
      d4++;    
    //Serial.print(s[j]);
    }

  //Kiem tra xem s co khop voi 1 trong cac xau s1, s2, s3, s4 hay khong
  if ((d1 != i) && (d2 != i) && (d3 != i) && (d4 != i))
    check = false;
  else
    check = true;
  
  //Thuc hien dieu khien Servo neu s khop voi 1 trong cac xau trong tap du lieu
  if(check)
  {
    //Dieu chinh Servo quay tu 90  ve lai 20
    for(pos = 90; pos>=20; pos-=1)    
      {                                
        myservo.write(pos);              
        delay(80);                       
      } 
      
    //Delay 8s de xe di vao  
    delay(8000); 

      
    //Day du lieu rong len channel de tranh viec lap vo han     
    name = " ";
    ThingSpeak.writeField(counterChannelNumber, FieldNumber1, name, myCounterWriteAPIKey);  

    //Dieu chinh Servo quay tu 20 den 90
    for(pos = 20; pos <= 90; pos += 1)  
       {                                  
         myservo.write(pos);              
         delay(200);                      
       }    
  }  
}
