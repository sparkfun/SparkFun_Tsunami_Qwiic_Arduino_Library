/******************************************************************************
  Example1_PlayFile.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/6/2021
  ~

  This example plays the WAV file named "1.wav" on the SD card.
  Note, to learn more about file naming rules for the Tsunami,
  Please visit: 
  https://learn.sparkfun.com/tutorials/tsunami-hookup-guide/all#tsunami-demonstration

  Resources:
  Wire.h (included with Arduino IDE)
  SparkFun_Tsunami_Qwiic.h (included in the src folder) http://librarymanager/All#SparkFun_Tsunami

  Development environment specifics:
  Arduino 1.8.13
  Tsunami Hardware Version v21
  Redboard Qwiic Version v10

  Distributed as-is; no warranty is given.
******************************************************************************/

#include <Wire.h>            // Used to establish serial communication on the I2C bus

#include <SparkFun_Tsunami_Qwiic.h>

#define LED 13
#define BUTTON 2

TsunamiQwiic tq;

bool gButtonState = 0;
byte gQwiicData = 0;
char gResponse[36];
byte gResponseCnt = 0;
int gNumTracks = 0;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(BUTTON, INPUT_PULLUP);
  
  Serial.begin(115200);
  Serial.println("Tsunami Qwiic Example 1 - Play File");
  
  Wire.begin();
  tq.start();
}

void loop() {
  gButtonState = digitalRead(BUTTON);
  Serial.print("gButtonState: ");
  Serial.println(gButtonState);
  if (gButtonState == LOW) {
    digitalWrite(LED, HIGH);
    tq.getVersion(gResponse);
    Serial.print("Version: ");
    Serial.println(gResponse);
    gNumTracks = tq.getNumTracks();
    Serial.print("Number of Tracks: ");
    Serial.println(gNumTracks);

    if (tq.isTrackPlaying(1) == false) {
      Serial.println("Starting Track 10");
      tq.trackPlaySolo(10, 0, false);
    }
    delay(1000);
    digitalWrite(LED, LOW);
  }
  delay(500);
}