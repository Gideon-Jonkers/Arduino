//#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include <FS.h>

char *ssid = "HUAWEI-5GCPE-D14F";
char *pass = "5g4t3j2r1d1";

ESP8266WebServer server(80);

int hour = 5;
int minut = 0;
int lminut = 1;
int second = 0;
int milli = 0;
double tempA[24][60];
double mean;
int readt;

long int timer = micros();

bool handleRead(){
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
    file.close();
    return true;
}


void setup() {
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  while(WiFi.status() !=WL_CONNECTED)
  {
    digitalWrite(2,!digitalRead(2));
    delay(500);
  }
  IPAddress muIp = WiFi.localIP();
  Serial.println(muIp);
  SPIFFS.begin();
  handleRead();
  digitalWrite(2,LOW);
  MDNS.begin("esp8266");
  server.on("/", handleC);
  server.on("/tempj", HTTP_GET,[](){
    String argh = server.arg("hour");
    String json ="{";
    json+= "\"ar\": [" + String(tempA[argh.toInt()][0]);
    for(int i = 1;i<60;i++){
      json += ", " + String(tempA[argh.toInt()][i]);
    }
    json += "]}";
    server.send(200,"text/json",json);
    json = String();
  });

  server.on("/setDate",HTTP_GET,[](){
   String argh = server.arg("hour");
   String argm = server.arg("minut");
   String argS = server.arg("second");
   second = argS.toInt();
   minut = argm.toInt();
   hour = argm.toInt(); 
  });
  
  server.begin();

}

void loop() {
  server.handleClient();
  MDNS.update();
  if(micros() - timer>999){
    timer = micros()-(micros()-timer-999);
    milli++;
  }
  if(milli>999){
    milli=0;
    second++;
  }
  if(second>59){
    second=0;
    minut++;
  }
  if(minut>59){
    minut=0;
    hour++;
  }
  if(hour>23){
    hour=0;
  }
  if(lminut!=minut){
    tempA[hour][minut] = analogRead(A0)*33/102.4;
    lminut=minut;
  }
}

void handleC()
{
  handleRead();
}

//String tempR(){
  //double val = analogRead(A0)*3.3/10.24;
  //return String(val);
//}
//String tempAr(){
  //String argv = server.arg("arg");
 // return String(tempA[(argv.toInt())]);
//}
