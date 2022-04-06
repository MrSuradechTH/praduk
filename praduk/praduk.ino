#include <RTClib.h>
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
uint8_t s_now,s_old;

uint8_t pump = 23,lamp = 19;
uint8_t state[] = {0,0};
uint8_t state_check[] = {0,0};

int buzzer = 18,ch = 0;
String note[] = {"e","e","e","c","e","g"};
uint8_t level[] = {5,5,5,5,5,5};
float delay_note[] = {0.5,1,1,0.5,1,1};
float speed = 3; //only 0-infinity
uint8_t levelup = 3;
bool song_play,start;

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
  
  //low on hight off
  if (now.minute() < 30) {
    state[0] = 1;
  }else {
    state[0] = 0;
  }

  if (now.hour() >= 18 || now.hour() < 5) {
    state[1] = 1;
  }else {
    state[1] = 0;
  }

  s_now = now.second();
  if (s_now != s_old) {
    s_old = s_now;
    Serial.println(String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + " " + String(now.day()) + "/" + String(now.month()) + "/" + String(now.year() + 543) + " " + daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.println("Temperature: " + String(rtc.getTemperature()) + " C");
    for (int x = 0;x < 2;x++) {
      if (state_check[x] != state[x]) {
        song_play = true;
      }
      state_check[x] = state[x];
    }

    if (song_play == true && start == true) {
      song();
      song_play = false;
    }else {
      start = true;
      song_play = false;
    }

    for (int x = 0;x < 2;x++) {
        if (x == 0) {
          if (state[x] == 1) {
            digitalWrite(pump, LOW);
            Serial.println("pump on");
          }else {
            digitalWrite(pump, HIGH);
            Serial.println("pump off");
          }
        }else {
          if (state[x] == 1) {
            digitalWrite(lamp, LOW);
            Serial.println("lamp on");
          }else {
            digitalWrite(lamp, HIGH);
            Serial.println("lamp off");
          }
        }
    }
    Serial.println();
  }
  delay(10);
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
    delay(int(delay_note[x] * int(1000 / (speed + 0.0001))));
  }
  ledcWrite(ch, 0);
}
