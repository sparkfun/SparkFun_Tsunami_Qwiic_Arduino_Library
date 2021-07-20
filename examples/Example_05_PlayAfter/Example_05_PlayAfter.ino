/******************************************************************************
  Example_05_PlayAfter.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/6/2021
  ~

  This example plays "5.WAV".
  After it is done playing,
  It will immediately start playing "6.WAV".
  This is useful if you'd like to play a collection of tracks
  in order, such as a playlist.

  It does this by continously polling (aka asking) if the Tsunami is still playing.
  It does this by calling the "isTrackPlaying()" function every 10ms.
  As soon as the Tsunami responds with a false, then it starts "6.WAV"

  *** Special not about timing and polling the Tsunami
  *** Make sure to add in some small delays (10ms is fine) between each time you 
  *** Talk to the Tsunami.
  *** If you call the funtion "isTrackPlaying" too frequently,
  *** This can cause the audio to stop playing properly, 
  *** and you may experience some stuttering/distortion.

  Resources:
  Wire.h (included with Arduino IDE)
  SparkFun_Tsunami_Qwiic.h (included in the src folder) http://librarymanager/All#SparkFun_Tsunami

  Development environment specifics:
  Arduino 1.8.13
  Tsunami Hardware Version v21
  Redboard Qwiic Version v10

  Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFun_Tsunami_Qwiic.h> //http://librarymanager/All#SparkFun_Tsunami_Super_WAV_Trigger

TsunamiQwiic tsunami;

void setup() {
  
  Serial.begin(115200);

  Serial.println("Tsunami Qwiic Example 5 - Play After");
  
  Wire.begin();

  // Check to see if Tsunami Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x13, I2C-port:Wire)
  if (tsunami.begin() == false)
  {
    Serial.println("Tsunami Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);      
  }; 

  tsunami.trackPlaySolo(5, 0); // track = 5 (aka "5.WAV"), output = 0 (aka "1L") 
  Serial.println("Playing track 5 right now...");

  while(tsunami.isTrackPlaying(5) == true) // keep asking if the Tsunami is playing track 5
  {
    delay(10); // this delay is important! (to avoid overtaxing the I2C bus or the Tsunami)
  }

  Serial.println("Track 5 is done.");

  tsunami.trackPlaySolo(6, 0); // track = 6 (aka "6.WAV"), output = 0 (aka "1L") 
  Serial.println("Playing track 6 right now...");

  while(tsunami.isTrackPlaying(6) == true) // keep asking if the Tsunami is playing track 6
  {
    delay(10); // this delay is important! (to avoid overtaxing the I2C bus or the Tsunami)
  }

  Serial.println("Track 6 is done.");
}

void loop() 
{

}