#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9);

int buf[321];
int bufs[321];
int y;
int dif;
int difs;
int t, w;
int scnt, scnts, pos, ecnt, ecnts, per, pers, str, strs;
double maxval, minval, mis, mas;
unsigned long etime, stime,  xtime, xtimes;

void setup() {
  //pinMode(5,OUTPUT);


  //pinMode(3,OUTPUT);

  //tft.begin(32000000);
  tft.begin();

  tft.setRotation(3);

  tft.fillScreen(ILI9341_BLACK);
  //analogWrite(5,128);
  //OCR0B = 32;



}

void loop() {

  //analogWrite(6,10);
  //tft.drawLine(160, 10, 160, 230, ILI9341_WHITE);
  tft.drawLine(0, 10, 320, 10, ILI9341_WHITE);
  //tft.drawLine(0, 32, 320, 32, ILI9341_WHITE);
  //tft.drawLine(0, 76, 320, 76, ILI9341_WHITE);
  //tft.drawLine(0, 120, 320, 120, ILI9341_WHITE);
  //tft.drawLine(0, 164, 320, 164, ILI9341_WHITE);
  //tft.drawLine(0, 208, 320, 208, ILI9341_WHITE);
  tft.drawLine(0, 230, 320, 230, ILI9341_WHITE);
  

  //w = map(analogRead(A4),0,1023,0,170);

  tft.drawLine(0, map(t, 0, 1023, 229, 9), 320, map(t, 0, 1023, 229, 9) , ILI9341_BLACK);

  t = analogRead(A2);

  tft.drawLine(0, map(t, 0, 1023, 229, 9), 320, map(t, 0, 1023, 229, 9) , ILI9341_RED);

  scnt = 0;
  ecnt = 0;
  scnts = 0;
  ecnts = 0;
  maxval = 0;
  minval = 1023;
  etime = 0;

  while (scnt++ < 200) {
    pos = analogRead(A0);
    if (pos >= t) {
      while (ecnt++ < 200) {
        pos = analogRead(A0);
        if (pos <= t) {
          stime = micros();
          break;
        }
      }
      break;
    }
  }


  while (scnts++ < 200) {
    pos = analogRead(A0);
    if (pos >= t) {
      xtime = micros();
      while (ecnts++ < 200) {
        pos = analogRead(A0);
        if (pos <= t) {
          etime = micros();
          break;
        }
      }
      break;
    }
  }

  for (int i = 0; i < 321; i++) {
    y = analogRead(A0);
    buf[i] = y;
  }

  xtimes = xtime - stime;

  if (etime == 0 | stime == 0) {
    str = 0;
    dif = 0;
  }
  else {
    dif = etime - stime;
    str = 1000000 / dif;
  }

  for (int i = 0; i < 321; i++) {
    buf[i] = map(buf[i], 0, 1023, 228, 11);
  }

  for (int i = 0; i < 320; i++) {
    tft.drawLine(i, bufs[i], i, bufs[i + 1], ILI9341_BLACK);

    tft.drawLine(i, buf[i], i, buf[i + 1], ILI9341_BLUE);
    bufs[i] = buf[i];
  }

  for (int i = 1; i < 319; i++) {
    if (map(buf[i], 9, 229, 220, 0) > maxval) {
      maxval = map(buf[i], 9, 229, 220, 0);
    }
    if (map(buf[i], 9, 229, 220, 0) < minval) {
      minval = map(buf[i], 9, 229, 220, 0);
    }
  }
  per = xtimes * 100 / dif;
  per = map(per, 0, 100, 100, 0);

  //tft.setCursor(10,2);
  //tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  //tft.print("MaxVolts: " + String(mas/43.8) + " MinVolts: " + String(mis/43.8));
  //tft.setCursor(10,2);
  //tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  //tft.print("MaxVolts: " + String(maxval/43.8) + " MinVolts: " + String(minval/43.8));
  //mas = maxval;
  //mis = minval;

  tft.setCursor(10, 232);
  tft.setTextColor(ILI9341_BLACK);  tft.setTextSize(1);
  tft.print(String(strs) + " Hz " + pers + "%");
  tft.setCursor(10, 232);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.print(String(str) + " Hz " + per + "%");
  pers = per;
  strs = str;

}
