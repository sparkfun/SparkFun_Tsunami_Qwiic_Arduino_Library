/******************************************************************************
  Example_06_PlayPoly.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/6/2021
  ~

  This example demonstrates the polyphonic feature of the Tsunami.
  This means that we can play multiple WAV files at the same time.
  This is called polyphony or layering audio!
  You can even call one long sound WAV file, and then call other files to 
  play on top of that. You can even call the same file many times.
  This becomes very useful when creating musical instruments
  or even interesting sound environments!

  In this example we will simply play one WAV file, 7.WAV, which is 
  actually a long-ish (1 minute) recording of rain and thunder.
  Then we will randomly layer some bird sounds on top of that using 
  the playTrackPoly() function.

  The bird sounds are on example WAV files 8,9,and 10.

  All of the WAV files needed for this example can be found here:
  https://github.com/sparkfun/SparkFun_Tsunami_Super_WAV_Trigger_Qwiic

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

int track = 8; // start with track 8, and then we will increment through 8,9,10.

void setup() {
  
  Serial.begin(115200);

  Serial.println("Tsunami Qwiic Example 6 - Play Poly");
  
  Wire.begin();

  // Check to see if Tsunami Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x13, I2C-port:Wire)
  if (tsunami.begin() == false)
  {
    Serial.println("Tsunami Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);      
  }; 

  tsunami.trackLoop(7, true); // loop forever
  tsunami.trackPlayPoly(7, 0); // track = 7 (aka "7.WAV"), output = 0 (aka "1L") 
  Serial.println("Looping track 7 right now...");
}

void loop() 
{
  int random_delay_seconds = random(1,4)*10; // delay a random amount of seconds (10, 20 or 30 seconds)
  Serial.print("Delaying for ");
  Serial.print(random_delay_seconds);
  Serial.println(" seconds.");
  delay(random_delay_seconds*1000);        
    
  tsunami.trackPlayPoly(track, 0);  // play track on top of whatever else is playing
                                    // in this example, it is really only track 7 that is currently playing.
  Serial.print("Playing track ");
  Serial.println(track);
  track = track + 1;
  if(track == 11) track = 8; // cycle back to track 8.
}