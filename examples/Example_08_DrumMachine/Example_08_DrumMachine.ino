/******************************************************************************
  Example_08_DrumMachine.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 5/18/2021
  ~

  This example plays drum beats.

  The example plays 4 measures of each available drum beat.
  Note, this is written to work with the mono firmware of the Tsunami.
  Output is on 1L (aka "0").

  You can program the kick, snare, high-hat, and ride using the included header files (tabs in Arduino).
  It currently supports 16th not divisions of the measure.
  You can also hook up a potentiometer to A0 to actively adjust tempo.
  For this, you must uncomment some code below in the function "playMeasure()".

  Resources:
  Wire.h (included with Arduino IDE)
  SparkFun_Tsunami_Qwiic.h (included in the src folder) http://librarymanager/All#SparkFun_Tsunami

  Development environment specifics:
  Arduino 1.8.13
  Tsunami Hardware Version v21
  Tsunami Firmware Version v1.94m (MONO FIRMWARE)
  Redboard Qwiic Version v10

  Distributed as-is; no warranty is given.
******************************************************************************/

#include "track_numbers.h"
#include "drum_beats.h"
#include "set_functions.h"

int delay_ms_tsunami_com = 10; // used to delay between sequenctial I2C writes
// to avoid commanding the Tsunami too often and causing audio issues.

#include <SparkFun_Tsunami_Qwiic.h>

TsunamiQwiic tsunami;                // Our Tsunami object

float bpm = 120.0; // beats per minute (how many 1/4 notes there are in a minute)

char versionResponse[36]; // an array of chars to store the version string
int NumTracks = 0; // used to store the value returned from the tsunami


void setup()
{

  Serial.begin(115200);

  Serial.println("Tsunami Qwiic Example 9 - Drum Machine");

  Wire.begin();

  // Check to see if Tsunami Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x13, I2C-port:Wire)
  if (tsunami.begin() == false)
  {
    Serial.println("Tsunami Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);
  };

  delay(delay_ms_tsunami_com);
  // Send a stop-all command and reset the sample-rate offset, in case we have
  //  reset while the Tsunami was already playing.
  tsunami.stopAllTracks();
  delay(delay_ms_tsunami_com);

  tsunami.samplerateOffset(0, 0);
  delay(delay_ms_tsunami_com);

  tsunami.masterGain(0, 0); // Reset the master gain to 0dB
  delay(delay_ms_tsunami_com);

  tsunami.getVersion(versionResponse);
  Serial.print("Version: ");
  Serial.println(versionResponse);

  NumTracks = tsunami.getNumTracks();
  Serial.print("Number of Tracks: ");
  Serial.println(NumTracks);
}

void loop()
{
  set_four_on_the_floor();
  play_measure();
  play_measure();
  play_measure();
  play_measure();

  set_half_time_groove();
  play_measure();
  play_measure();
  play_measure();
  play_measure();

  set_half_time_groove_ride();
  play_measure();
  play_measure();
  play_measure();
  play_measure();
}


void play_measure()
{
  // loop through all 16th bar time divisions,
  // play each WAV (or combo of WAVS) at each division.

  long start_time_micros = micros();

  for (int i = 0 ; i < 16 ; i++)
  {
    //int val = analogRead(A0); // option to control tempo speed, hookup a potentiometer to A0
    //bpm = map(val, 0, 1023, 0, 240);
    bpm = 120.0;
    Serial.print("bpm: ");
    Serial.println(bpm);

    if (kick[i] == 1) tsunami.trackLoad(kick_t, 0, false);
    delay(delay_ms_tsunami_com);
    if (snare[i] == 1) tsunami.trackLoad(snare_t, 0, false);
    delay(delay_ms_tsunami_com);
    if (hat[i] == 1) tsunami.trackLoad(hat_t, 0, false);
    delay(delay_ms_tsunami_com);
    if (ride[i] == 1) tsunami.trackLoad(ride_t, 0, false);
    delay(delay_ms_tsunami_com);

    tsunami.resumeAllInSync();

    long sixteenth_note_delay = bpm_to_16th_delayms(bpm);
    while ((micros() - start_time_micros) < (sixteenth_note_delay * 1000)) delayMicroseconds(1);
    start_time_micros = micros();
  }
}

long bpm_to_16th_delayms(float bpm)
{
  float seconds_f = 60.0 / bpm; // beats-per-minute to beats-per-second
  seconds_f /= 4.0; // to quarter notes
  seconds_f *= 1000.0; // to milliseconds
  return long(seconds_f);
}