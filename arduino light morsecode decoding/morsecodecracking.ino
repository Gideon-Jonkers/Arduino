/*
  Reads morse code from a photo resistor sent from a cellphones flashlight.
  Decodes the morsecode message with Decode() function from Morsecode.h.
  Displays the decode messages to a lcd display

  Required components
    -photoresistor
    -10k puldown resistor on photoresistor
    -Cap 1microfarad for stable switching on photoresistor, filters out the jitters for digitalreading the photoresistor
    -lcd on Rs 12, en 11, d4 7, d5 6, d6 5, d7 7

  Coded by
    Gideon Jonkers
*/

#include "Morsecode.h"
#define dot 100
#define dah (3*dot)
#define space (7*dot)


void setup() {
  delay(1000);
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), setTime, CHANGE); //interrupt for the light signal
}

unsigned long timer = 0;
unsigned long timerH = 0;  //timer for High pulse
unsigned long timerHp = 0; //timer previous high pulse
unsigned long timerL = 0;  //timer for Low pulse
unsigned long timerLp = 0; //timer previous Low pulse
String text;

void loop() {
  
  high:
  if(timerH != timerHp)
  {
    //Serial.println(timerH);
    timerHp = timerH;
  }
  if((timerH >= (dot - 70) && timerH < (dot+70)))
  {
    text += '.';
    Serial.println("Dot");
    timerH = 0;
    timerHp = 0;
    goto Lowe;
  }
  else if((timerH >= (dah - 70) && timerH < (dah+70)))
  {
    text += '_';
    Serial.println("dah");
    timerH = 0;
    timerHp = 0;
    goto Lowe;
  }
  goto high;
  
  Lowe:
  if(timerL != timerLp)
  {
    //Serial.println(timerL);
    timerLp = timerL;
  }
  if((timerL >= (dot - 70) && timerL < (dot+70)))
  {
    timerL = 0;
    timerLp = 0;
    goto high;
  }
  else if((timerL >= (dah - 70) && timerL < (dah+70)))
  {
    text += ' ';
    Serial.println(' ');
    timerL = 0;
    timerLp = 0;
    goto high;
  }
  else if((timerL >= (space - 70) && timerL < (space+70)))
  {
    text += '/';
    Serial.println(' ');
    Serial.println(' ');
    timerL = 0;
    timerLp = 0;
    goto high;
  }
  if(millis() - timer > (space+100))
  {
      text += ' ';
      //Serial.println(text);
      Serial.println(Decode(text));
      timer = millis();
      text = "";
      delay(10000);
      goto high;
  }
  goto Lowe;

}

void setTime()
{
  if(digitalRead(2) == HIGH){timerL = millis() - timer;}
  if(digitalRead(2) == LOW) {timerH = millis() - timer;}
  timer = millis();
}
