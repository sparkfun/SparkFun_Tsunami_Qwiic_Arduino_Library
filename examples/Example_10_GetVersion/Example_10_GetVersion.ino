/******************************************************************************
  Example_10_GetVersion.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/6/2021
  ~

  This example gets the version of the firmware on your Tsunami and prints it to terminal.
  It also gets the number of tracks on the uSD card and prints that!

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

char versionResponse[36]; // an array of chars to store the version string
int NumTracks = 0; // used to store the value returned from the tsunami

void setup() {
  
  Serial.begin(115200);

  Serial.println("Tsunami Qwiic Example 10 - Get Version");
  
  Wire.begin();

  // Check to see if Tsunami Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x13, I2C-port:Wire)
  if (tsunami.begin() == false)
  {
    Serial.println("Tsunami Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);      
  }; 

  tsunami.getVersion(versionResponse);
  Serial.print("Version: ");
  Serial.println(versionResponse);

  NumTracks = tsunami.getNumTracks();
  Serial.print("Number of Tracks: ");
  Serial.println(NumTracks);
}

void loop() 
{

}