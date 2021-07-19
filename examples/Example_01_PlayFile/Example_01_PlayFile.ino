/******************************************************************************
  Example_01_PlayFile.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/6/2021
  ~

  This example plays the WAV file named "1.wav" on the SD card.
  Note, to learn more about file-naming rules for the Tsunami,
  Please visit: 
  https://learn.sparkfun.com/tutorials/tsunami-hookup-guide/all#tsunami-demonstration

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

  Serial.println("Tsunami Qwiic Example 1 - Play File");
  
  Wire.begin();

  // Check to see if Tsunami Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x13, I2C-port:Wire)
  if (tsunami.begin() == false)
  {
    Serial.println("Tsunami Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);      
  }; 

  tsunami.trackPlaySolo(1, 0); // track = 1 (aka "1.WAV"), output = 0 (aka "1L") 

  Serial.println("All done!");
}

void loop() 
{

}