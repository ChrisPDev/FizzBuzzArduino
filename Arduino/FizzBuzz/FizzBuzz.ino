#include <Arduino_MKRIoTCarrier.h>
#include <bsec.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <SD.h>
//#include "ardino_secrets.h"

MKRIoTCarrier carrier;

char ssid[] = "ChristofferHotspot";
char pass[] = "Doomreaver021292!";

int status = WL_IDLE_STATUS;

int wifiCheckTimer = 0;

int curNum = 1;
int fizzNum = 3;
int buzzNum = 5;

int r, g, b;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while(!Serial) {
    ;
  }

  if(WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");

    while(true);
  }

  String fv = WiFi.firmwareVersion();
  
  if(fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while(status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);

    status = WiFi.begin (ssid, pass);

    delay(10000);
  }

  Serial.println("You're connected to the network");
  printCurrentNet();
  printWifiData();

  CARRIER_CASE = false;
  carrier.begin();

  newGame();
  Serial.println("Setup done");
}

void loop() {
  if(carrier.Light.colorAvailable()) {
    carrier.Light.readColor(r, g, b);

    if(r > 0 && r < 500 && g > 0 && g < 300 && b > 0 && b < 300) {
      //Serial.println("Det er mørkt");
      carrier.Buttons.update();
      // Serial.println(curNum);

      if(carrier.Buttons.onTouchDown(TOUCH0)){
        // Fizz
        if(curNum % fizzNum == 0){
          correctAnswerMessage("Fizz");
        }
        else {
          wrongAnswer();
        }
      }
      
      if(carrier.Buttons.onTouchDown(TOUCH1)){
        // Buzz
        if(curNum % buzzNum == 0){
          correctAnswerMessage("Buzz");
        }
        else {
          wrongAnswer();
        }
      }
      
      if(carrier.Buttons.onTouchDown(TOUCH2)){
        // FizzBuzz
        if(curNum % fizzNum == 0 && curNum % buzzNum == 0){
          correctAnswerMessage("FizzBuzz");
        }
        else {
          wrongAnswer();
        }
      }
      
      if(carrier.Buttons.onTouchDown(TOUCH3)){
        // Next
        if(curNum % buzzNum == 0 && curNum % fizzNum == 0){
          wrongAnswer();
        }
        else if(curNum % buzzNum == 0) {
          wrongAnswer();
        }
        else if(curNum % fizzNum == 0) {
          wrongAnswer();
        }
        else {
          correctAnswerNumber(curNum);
        }
      }

      if(carrier.Buttons.onTouchDown(TOUCH4)){
        newGame();
      }
    }
    else {
      //Serial.println("Det er lyst");
      newGame();
    }
  }

  if(wifiCheckTimer % 10000 == 0) {
    printCurrentNet();
  }

  wifiCheckTimer++;
}
void printWifiData() {
  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");
  Serial.println(ip);

  byte mac[6];

  WiFi.macAddress(mac);

  Serial.print("MAC address: ");

  printMacAddress(mac);
}
void printCurrentNet() {
  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  byte bssid[6];

  WiFi.BSSID(bssid);

  Serial.print("BSSID: ");

  printMacAddress(bssid);

  long rssi = WiFi.RSSI();

  Serial.print("Signal strength (RSSI): ");

  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();

  Serial.print("Encryption Type: ");

  Serial.println(encryption, HEX);

  Serial.println();
}
void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if(mac[i] < 16) {
      Serial.print("0");
    }

    Serial.print(mac[i], HEX);

    if (i > 0) {
      Serial.print(":");
    }
  }

  Serial.println();
}

void correctAnswerMessage(String message) {
  ledCircle(0x008000, 1);
  carrier.display.fillScreen(ST77XX_GREEN);
  carrier.display.setCursor(40, 110);
  carrier.display.println(message);
  Serial.println(message);
  curNum++;
  
  resetScreen();
}
void correctAnswerNumber(int number) {
  ledCircle(0x008000, 1);
  carrier.display.fillScreen(ST77XX_GREEN);
  carrier.display.setCursor(40, 110);
  carrier.display.println(number);
  Serial.println(number);
  curNum++;

  resetScreen();
}
void wrongAnswer() {
  ledCircle(0xFF0000, 1);
  carrier.display.fillScreen(ST77XX_RED);
  carrier.display.setCursor(40, 110);
  carrier.display.println("Game over");
  Serial.println("Game over");    
  curNum = 1;

  resetScreen();
}
void ledCircle(uint32_t hexColor, int length) {
  int led = 0;
  uint32_t color = hexColor;
  
  for(int i = 0; i < length; i++) {
    led = 0;
    for(int j = 0; j < 5; j++) {
      carrier.leds.setPixelColor(led, color);
      carrier.leds.show();
      delay(50);
      carrier.leds.setPixelColor(led, 0);
      carrier.leds.show();

      delay(150);

      led++;
    }
  }
}
void newGame() {
  curNum = 1;

  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextSize(3);
  carrier.display.setTextColor(ST77XX_WHITE);

  carrier.display.setCursor(40, 75);
  carrier.display.println("Velkommen");  
  carrier.display.setCursor(40, 110);
  carrier.display.println("til");
  carrier.display.setCursor(40, 145);
  carrier.display.println("FizzBuzz!");
  delay(2000);

  ledCircle(0x0000FF, 5);

  resetScreen();
}
void resetScreen() {
  delay(250);
  carrier.display.fillScreen(ST77XX_BLACK);
  
  carrier.display.setCursor(40, 110);
  carrier.display.println(curNum);
}