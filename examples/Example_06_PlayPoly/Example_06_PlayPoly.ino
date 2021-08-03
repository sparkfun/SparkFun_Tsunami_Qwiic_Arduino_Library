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
  You can call one long sound WAV file, and then call other files to 
  play on top of that. You can even call the same file many times.
  This becomes very useful when creating musical instruments
  or interesting sound environments.

  In this example we will simply play one WAV file, (19.WAV), which is 
  actually a long-ish (1 minute) recording of rain and thunder.
  Then we will randomly layer some bird sounds on top of that using 
  the playTrackPoly() function.

  The bird sounds are on example WAV files 20, 21 and 22.
  20_meadowlark_MONO.wav
  21_cardinal_MONO.wav
  22_dove_MONO.wav

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

int track = 20; // start with track 20 (meadowlark), and then we will increment through 21,22 and back.

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

  tsunami.trackLoop(19, true); // loop forever
  tsunami.trackPlayPoly(19, 0); // track = 19 (aka "19.WAV"), output = 0 (aka "1L") 
  Serial.println("Looping track 19 (rain) right now...");
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
  if(track == 23) track = 20; // cycle back to track 8.
}