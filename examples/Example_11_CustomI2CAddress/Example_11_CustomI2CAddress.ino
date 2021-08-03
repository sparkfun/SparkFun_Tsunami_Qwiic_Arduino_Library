/******************************************************************************
  Example_11_CustomI2CAddress.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/6/2021
  ~

  This example uses the Tsunami with a non-default I2C address.
  The default I2C address is 0x13, so for this example we will try out 0x14.
  To change the Address on your Tsunami, you must modify the INI file on the uSD card.
  If you include the following line, it will set the address to 0x14.

  #QWIC 20

  *** Note, in the INI file, the value is expressed as a decimal
  (0x14 HEX = 20 DEC)
  
  You can also use the Configurator EXE tool to create INI files.
  Learn more about this tool here:
  http://robertsonics.com/tsunami-downloads/

  This example plays the WAV file named "1.wav" on the SD card.

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

  Serial.println("Tsunami Qwiic Example 11 - Custom I2C address");
  
  Wire.begin();

  // Check to see if Tsunami Qwiic is present on the bus
  // Note, here we are calling begin() with our custom address: 0x14
  if (tsunami.begin(0x14) == false)
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