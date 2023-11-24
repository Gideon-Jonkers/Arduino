#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

char *ssid = "TestEsp";
char *pass = "123456789";

double buf[400];
double bufv[400];
int red = 1;
double val;
long int ttime = millis();
String listt;

ESP8266WebServer server(80);

void setup(void)
{
  pinMode(2,OUTPUT);
  WiFi.begin(ssid,pass);
  while(WiFi.status() !=WL_CONNECTED)
  {
    digitalWrite(2,!digitalRead(2));
    delay(500);
  }
  digitalWrite(2,LOW);
  MDNS.begin("esp8266");
  server.on("/", handleRoot);
  server.on("/image.svg", DrawLine);
  server.on("/list", list);
  server.begin();
}

void loop(void)
{
  server.handleClient();
  MDNS.update();
  if(millis()-ttime > 10000){
    ttime = millis(); 
    updateTemp();
  }
}

void handleRoot()
{
  char temp[200];
  snprintf(temp, 200,

  "<html>\
  <head>\
  <meta http-equiv='refresh' content='5'/>\
  </head>\
  <body>\
    <h1>ESP8266 Data loging</h1>\
    <p>Tempreture : %12d *C</p>\
    <img src=\"/image.svg\" />\
  </body>\
  </html>",
  val
  );
  server.send(200,"text/html",temp);
}

void DrawLine()
{
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(255, 255, 255)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  for (int x = 1; x < 399; x++) {
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140-buf[x], x+1, 140-buf[x+1]);
    out += temp;
  }
  out += "</g>\n</svg>\n";
  

  server.send(200, "image/svg+xml", out);
}

void updateTemp()
{
  val = analogRead(A0)*3.3/10.24;
  bufv[red] = val;
  buf[red] = val*3;
  red++;
  if(red > 399)
  {
    red=1;
  }
}

void list()
{
  listt = "";
  char temp[400];
  int x = 1;
  listt += "<ol>\n";
  while(bufv[x] != 0)
  {
    sprintf(temp, "<li> %02d *C</li>\n",bufv[x]);
    listt += temp;
  }
  listt += "</ol>\n";
  server.send(200,"text/html",listt);
}
