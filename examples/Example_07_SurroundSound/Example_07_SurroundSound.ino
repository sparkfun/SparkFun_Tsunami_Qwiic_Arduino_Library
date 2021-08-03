/******************************************************************************
  Example_08_SurroundSound.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/6/2021
  ~

  This example plays 8 MONO WAVE files each on its own dedicated output,
  similar to what you might do for a 7.1 Surround Sound setup.
  It loads them up, and then starts them at the same time.

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

  Serial.println("Tsunami Qwiic Example 08 - Surround Sound");
  
  Wire.begin();

  // Check to see if Tsunami Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x13, I2C-port:Wire)
  if (tsunami.begin() == false)
  {
    Serial.println("Tsunami Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);      
  }; 

  // Load up all the tracks of the surround sound
  // With each call of trackLoad() we will assign the track to the output
  tsunami.trackLoad(1, 0, false); // track 1 on output 0 (aka "1L"), Lock = false (voice stealing active)
  tsunami.trackLoad(2, 1, false); // track 2 on output 1 (aka "1R"), Lock = false (voice stealing active)
  tsunami.trackLoad(3, 2, false); // track 3 on output 2 (aka "2L"), Lock = false (voice stealing active)
  tsunami.trackLoad(4, 3, false); // track 4 on output 3 (aka "2R"), Lock = false (voice stealing active)
  tsunami.trackLoad(5, 4, false); // track 5 on output 4 (aka "3L"), Lock = false (voice stealing active)
  tsunami.trackLoad(6, 5, false); // track 6 on output 5 (aka "3R"), Lock = false (voice stealing active)
  tsunami.trackLoad(7, 6, false); // track 7 on output 6 (aka "4L"), Lock = false (voice stealing active)
  tsunami.trackLoad(8, 7, false); // track 8 on output 7 (aka "4R"), Lock = false (voice stealing active)

  // Start playing them all at the same time.
  // Note, they will remain in "sample-sync" as they play back - sweet!
  tsunami.resumeAllInSync();

  Serial.println("All done!");
}

void loop() 
{

}