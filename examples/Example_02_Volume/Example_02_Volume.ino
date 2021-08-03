/******************************************************************************
  Example_02_Volume.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/6/2021
  ~

  This example plays "2.wav" on output 1L at different volumes.

  Note, you can set the gain of a track either before you start it,
  or you can change the gain while it is playing, and the effect will be immediate.
  Range is -70 (muted) to 0 (unity) to 10 (adds +10dB of gain).

  ***  Special note about gain settings and WAV files:
  ***  Keep in mind that if your WAV file is normalized, 
  ***  so that the loudest value is full scale (0dB), 
  ***  then setting the gain (or target gain) to +10dB will cause clipping (distortion.)
  ***

  The WAV file (or files) needed for this example can be downloaded from the
  SparkFun Hookup Guide here:
  https://learn.sparkfun.com/tutorials/tsunami-super-wav-trigger-hookup-guide

  Resources:
  Wire.h (included with Arduino IDE)
  SparkFun_Tsunami_Qwiic.h (included in the src folder) http://librarymanager/All#SparkFun_Tsunami

  Development environment specifics:
  Arduino 1.8.15
  Tsunami Hardware Version v21
  Redboard Qwiic Version v10

  Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFun_Tsunami_Qwiic.h> //http://librarymanager/All#SparkFun_Tsunami_Super_WAV_Trigger

TsunamiQwiic tsunami;

void setup() {
  
  Serial.begin(115200);

  Serial.println("Tsunami Qwiic Example 2 - Volume");
  
  Wire.begin();

  // Check to see if Tsunami Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x13, I2C-port:Wire)
  if (tsunami.begin() == false)
  {
    Serial.println("Tsunami Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);      
  }; 

  // -20
  tsunami.trackGain(2,-20);
  tsunami.trackPlaySolo(2, 0); // track = 2 (aka "2.WAV"), output = 0 (aka "1L") 
  Serial.println("Now playing Track 2 at a gain of -20");
  delay(1000);

  // -10
  tsunami.trackGain(2,-10);
  tsunami.trackPlaySolo(2, 0); // track = 2 (aka "2.WAV"), output = 0 (aka "1L") 
  Serial.println("Now playing Track 2 at a gain of -10");
  delay(1000);

  // 0
  tsunami.trackGain(2, 0);
  tsunami.trackPlaySolo(2, 0); // track = 2 (aka "2.WAV"), output = 0 (aka "1L") 
  Serial.println("Now playing Track 2 at a gain of 0");
  delay(1000);

  // +10
  tsunami.trackGain(2, 10);
  tsunami.trackPlaySolo(2, 0); // track = 2 (aka "2.WAV"), output = 0 (aka "1L") 
  Serial.println("Now playing Track 2 at a gain of +10");
  delay(1000); 

  Serial.println("All done!");
}

void loop() 
{
 
}