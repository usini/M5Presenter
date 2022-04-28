#include <Arduino.h>
#include <BleKeyboard.h>
#include <M5Unified.h>
#include <elapsedMillis.h>
#include "bluetooth_icon.h"

BleKeyboard bleKeyboard("M5Presenter");
bool blestate = false;
int level = 0;
elapsedMillis timer;
unsigned int interval = 1000;

int seconds = 0;
int minutes = 0;
int hours = 0;

void setup() {
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setBrightness(128);
  // pinMode(10, OUTPUT);
  // digitalWrite(10, LOW);
  //M5.Power.setLed(128);
  Serial.println("");
  Serial.println("");
  Serial.println("Begin Bluetooth Connection");
  bleKeyboard.begin();
}
void loop() {
  M5.update();

  if (timer >= interval) {
    M5.Lcd.clear();
    M5.Lcd.setCursor(85, 0);
    level = M5.Power.getBatteryLevel();
    M5.Lcd.print(String(level) + "%");

    timer = 0;
    seconds++;
    if (seconds == 60) {
      minutes++;
      seconds = 0;
    }
    if (minutes == 60) {
      hours++;
      minutes = 0;
    }
    String hours_s = String(hours);
    String minutes_s = String(minutes);
    String seconds_s = String(seconds);
    if (hours < 10) {
      hours_s = "0" + hours_s;
    }
    if (minutes < 10) {
      minutes_s = "0" + minutes_s;
    }
    if (seconds < 10) {
      seconds_s = "0" + seconds_s;
    }

    M5.Lcd.setCursor(10, 26);
    M5.Lcd.print(hours_s + ":" + minutes_s + ":" + seconds_s);
    if (blestate) {
      M5.Lcd.drawBitmap(0, 0, 24, 24, image_data_Bluetooth);
    }
  }
  if (M5.BtnPWR.wasClicked()) {
    Serial.println("Button Power");
    ESP.restart();
  }
  if (bleKeyboard.isConnected()) {

    if (!blestate) {
      Serial.println(" --> Bluetooth Connected");
      blestate = true;
    }

    if (M5.BtnA.wasReleased()) {
      Serial.println("Pressed A");
      bleKeyboard.press(KEY_RIGHT_ARROW);
      bleKeyboard.release(KEY_RIGHT_ARROW);
    } else if (M5.BtnB.wasReleased()) {
      Serial.println("Pressed B");
      bleKeyboard.press(KEY_LEFT_ARROW);
      bleKeyboard.release(KEY_LEFT_ARROW);
    }
  } else {
    if (blestate) {
      M5.Lcd.clear();
      M5.Lcd.println("Disconnected");
      blestate = false;
      Serial.println("Disconnected");
    }
  }
}