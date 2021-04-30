/*
  Reads morse code from a photo resistor sent from a cellphones flashlight.
  Decodes the morsecode message with Decode() function from Morsecode.h.
  Displays the decode messages to a lcd display

  Required components
    -photoresistor
    -10k puldown resistor on photoresistor
    -Cap 1microfarad for stable switching on photoresistor, filters out the jitters digitalreading the photoresistor
    -lcd on Rs 12, en 11, d4 7, d5 6, d6 5, d7 7

  Coded by
    Gideon Jonkers
*/

#include "Morsecode.h"
#include <LiquidCrystal.h>
#define dot 100
#define dah (3*dot)
#define space (7*dot)

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

void setup() {
  //Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(13,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), setTime, CHANGE); //interrupt for the light signal
}

static unsigned long timer = 0;
static unsigned long timerH = 0;  //timer for High pulse
static unsigned long timerHp = 0; //timer previous high pulse
static unsigned long timerL = 0;  //timer for Low pulse
static unsigned long timerLp = 0; //timer previous Low pulse
String text;
static int cntr = 0;
bool lsignal = false;
int lengthcode = 0;
int scntr = 0;
bool right = false;

void loop() {
  
  high:
  if(timerH != timerHp)
  {
    timerHp = timerH;
  }
  if((timerH >= (dot - 70) && timerH < (dot+70)))
  {
    text += '.';
    //Serial.println("Dot");
    timerH = 0;
    timerHp = 0;
    cntr++;
    goto Lowe;
  }
  else if((timerH >= (dah - 70) && timerH < (dah+70)))
  {
    text += '_';
    //Serial.println("dah");
    timerH = 0;
    timerHp = 0;
    cntr++;
    goto Lowe;
  }
  if(lengthcode > 16 && lsignal == false)
  {
    lcd.scrollDisplayLeft();
    delay(500);
  }
  goto high;
  
  Lowe:
  if(timerL != timerLp)
  {
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
    //Serial.println(' ');
    timerL = 0;
    timerLp = 0;
    cntr++;
    goto high;
  }
  else if((timerL >= (space - 70) && timerL < (space+70)))
  {
    text += '/';
    //Serial.println(' ');
    //Serial.println(' ');
    timerL = 0;
    timerLp = 0;
    cntr++;
    goto high;
  }
  if(millis() - timer > (space+100))
  {
      text += ' ';
      lsignal = false;
      lengthcode = Decode(text).length();
      lcd.clear();
      lcd.print(Decode(text));
      //Serial.println(text);
      //Serial.println(Decode(text));
      timer = millis();
      text = "";
      //delay(10000);
      goto high;
  }
  goto Lowe;

}

void setTime()
{
  if(digitalRead(2) == HIGH){ timerL = millis() - timer;}
  if(digitalRead(2) == LOW) {timerH = millis() - timer;}
  timer = millis();
  lsignal = true;
}
