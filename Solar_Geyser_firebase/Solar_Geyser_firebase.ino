#include <NTPClient.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Firebase_ESP_Client.h>

//Provide a token generation procces info
#include <addons/TokenHelper.h>
#define WIFI_SSID "HUAWEI-5GCPE-D14F"
#define WIFI_PASSWORD "5g4t3j2r1d1"

#define API_KEY "AIzaSyAFgLpq_ihkgsbcRgFXd0AlH5nBpbxQsQs"

#define FIREBASE_PROJECT_ID "solar-geyser-59631"

#define USER_EMAIL "gideon.jonkers04@gmail.com"
#define USER_PASSWORD "5g4t3j2r1d"

//Define Firebas data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

time_t ntptime;

bool done = false;
bool hourdone = false;
bool bUpdatedone = false;

double var[2];

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200);

void setup()
{
  //pinMode(16, OUTPUT);
  pinMode(2,OUTPUT);
  
  Serial.begin(9600);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  digitalWrite(2,LOW);

  //Update Time
  while(year()<2021)
  {
    UpdateTime();
    delay(500);
  }

  //Firebase begin
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  unsigned long timer = millis();
  while(millis()-timer < 5000)
  {
    yield();
  }
  
  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  
  Firebase.reconnectWiFi(true);
}


void loop() {
  digitalWrite(2, second()%2);
  if(minute() % 5 == 4)
  {
    done = false;
  }
  if(minute() == 0)
  {
    var[0] = analogRead(A0)/204.8f;
    var[1] = analogRead(A0)/204.8f;
    if (Firebase.ready())
    {
      FirebaseJson content;

      //aa is the collection id, bb is the document id.
      String documentPath = String(year()) + "/" + String(month()) + "/" + String(day()) + "/H" + String(hour());
              
      content.clear();
      content.set("fields/m" + String(minute()) + "/doubleValue", (var[0]+var[1])/2);
        
      Serial.println("Update a document... " + documentPath);

      if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw()))
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      else
        Serial.println(fbdo.errorReason());
      ESP.deepSleep((300000 - (1000*second()))*1000);
    }
  }
  if ((minute() % 5) == 0 && done == false)
  {
     var[0] = analogRead(A0)/204.8f;
     var[1] = analogRead(A0)/204.8f;
     if(Firebase.ready())
     {
        //For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
        FirebaseJson content;

        //aa is the collection id, bb is the document id.
        String documentPath = String(year()) + "/" + String(month()) + "/" + String(day()) + "/H" + String(hour());

        //If the document path contains space e.g. "a b c/d e f"
        //It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"
        content.clear();
        content.set("fields/m" + String(minute()) + "/doubleValue", (var[0]+var[1])/2);
        
        Serial.println("Update a document... " + documentPath);

        /** if updateMask contains the field name that exists in the remote document and 
         * this field name does not exist in the document (content), that field will be delete from remote document
        */

        if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw(),"m"+String(minute()) /* updateMask */))
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        else
            Serial.println(fbdo.errorReason());
        done = true;
        ESP.deepSleep((300000 - (1000*second()))*1000);
     }
   }
}

void UpdateTime()
{
  timeClient.begin();
  timeClient.update();
  ntptime = timeClient.getEpochTime();
  setTime(ntptime);
  timeClient.end();
}
