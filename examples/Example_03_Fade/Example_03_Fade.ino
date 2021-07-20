/******************************************************************************
  Example_03_Fade.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/6/2021
  ~

  This example slowly fades in "3.WAV", let's it play for 5 seconds, then slowly fades it out.

  It does this in 5 steps:
  1. set gain to muted (-70)
  2. start the track
  3. use the fade command to fade from current gain to desired new gain (unity, aka 0).
  4. let it play for 5 seconds
  5. use the fade command to fade from current gain to muted

  ***  Special note about gain settings and WAV files:
  ***  Keep in mind that if your wave file is normalized, 
  ***  so that the loudest value is full scale (0dB), 
  ***  then setting the gain (or target gain) to +10dB will cause clipping (distortion.)
  ***

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

  Serial.println("Tsunami Qwiic Example 13 - Fade");
  
  Wire.begin();

  // Check to see if Tsunami Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x13, I2C-port:Wire)
  if (tsunami.begin() == false)
  {
    Serial.println("Tsunami Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);      
  }; 

  tsunami.trackGain(3,-70); // muted
  tsunami.trackPlaySolo(3, 0); // track = 3 (aka "3.WAV"), output = 0 (aka "1L") 
  tsunami.trackFade(3, 0, 5000, false); // track 3, fade to gain of 0, 
  // fade time = 5000ms, stopFlag is false = do not stop
  Serial.println("Fading IN track 3 right now...");
  delay(5000);

  Serial.println("Gain set to 10! Playing for 5 seconds...");
  delay(5000);

  tsunami.trackFade(3, -70, 5000, true); // track 3, fade to gain of -70, 
  // fade time = 5000ms, stopFlag is true = stop track when fade is done
  Serial.println("Fading OUT track 3 right now...");

  delay(5000);
  Serial.println("Track 3 stopped.");
}

void loop() 
{

}