#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

uint8_t pump = 23,lamp = 19;
uint8_t state[] = {0,0};
uint8_t state_check[] = {0,0};

int buzzer = 18,ch = 0;
String note[] = {"e","e","e","c","e","g","n","g","n","c","g","e",  "c","g","n","e","e","e","c","e","g"};
uint8_t level[] = {5,5,5,5,5,5,0,4,0,5,4,4,5,4,0,5,5,5,5,5,5};
float delay_note[] = {0.5,1,1,0.5,1,1,1,1,1,1,1,1, 1,1,0,0.5,1,1,0.5,1,1};
uint8_t speed = 1;
uint8_t levelup = 0;
void setup () {
  Serial.begin(115200);
  if (!rtc.begin()) {
    Serial.println("RTC not connected!");
    while(true) {
      delay(10);
    }
  }
  pinMode(pump, OUTPUT);
  pinMode(lamp, OUTPUT);
 
  ledcAttachPin(buzzer, ch);
  
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//   January 21, 2014 at 3am you would call:
//   rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void loop () {
  DateTime now = rtc.now();
  Serial.println(String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + " " + String(now.day()) + "/" + String(now.month()) + "/" + String(now.year() + 543) + " " + daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.println("Temperature: " + String(rtc.getTemperature()) + " C");
  Serial.println();

  for (int x = 0;x < 2;x++) {
    state_check[x] = state[x];
  }
  
  //low on hight off
  if (now.hour() >= 8 && now.hour() < 16) {
    Serial.println("pump on");
    state[0] = 1;
    digitalWrite(pump, LOW);
  }else {
    Serial.println("pump off");
    state[0] = 0;
    digitalWrite(pump, HIGH);
  }

  if (now.hour() >= 18 || now.hour() < 5) {
    Serial.println("lamp on");
    state[1] = 1;
    digitalWrite(lamp, LOW);
  }else {
    Serial.println("lamp off");
    state[1] = 0;
    digitalWrite(lamp, HIGH);
  }
  
  for (int x = 0;x < 2;x++) {
    if (state_check[x] != state[x]) {
      song();
    }
  }
  
  delay(10);
//  digitalWrite(buzzer, HIGH);
//  delay(10000);
//  digitalWrite(buzzer, LOW);
//  delay(10000);
}

void song() {
  for (int x = 0;x < int(sizeof(level) / sizeof(uint8_t));x++) {
//    Serial.println(note[x] + ":" + String(int(delay_note[x] * 1000)));
    if (note[x] == "c") {
      ledcWriteNote(ch, NOTE_C, level[x] + levelup);
    }else if (note[x] == "d") {
      ledcWriteNote(ch, NOTE_D, level[x] + levelup);
    }else if (note[x] == "e") {
      ledcWriteNote(ch, NOTE_E, level[x] + levelup);
    }else if (note[x] == "f") {
      ledcWriteNote(ch, NOTE_F, level[x] + levelup);
    }else if (note[x] == "g") {
      ledcWriteNote(ch, NOTE_G, level[x] + levelup);
    }else if (note[x] == "a") {
      ledcWriteNote(ch, NOTE_A, level[x] + levelup);
    }else if (note[x] == "b") {
      ledcWriteNote(ch, NOTE_B, level[x] + levelup);
    }else if (note[x] == "n") {
      ledcWrite(ch, 0);
    }
    delay(int(delay_note[x] * int(500 / speed)));
  }
  ledcWrite(ch, 0);
}
