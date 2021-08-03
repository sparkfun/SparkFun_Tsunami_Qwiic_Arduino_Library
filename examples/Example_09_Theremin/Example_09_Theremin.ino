/******************************************************************************
  Example_09_Theremin.ino
  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 6/24/2021
  ~

  This example uses the Tsunami sampleRateOffset feature to re-create the old musical instrument,
  The Theremin.

  In addition to setting up the hardware described below, you also will need to ensure
  there is a 440Hz 30s sine WAV file named "17.WAV" on the uSD card.

  The WAV file (or files) needed for this example can be downloaded from the
  SparkFun Hookup Guide here:
  https://learn.sparkfun.com/tutorials/tsunami-super-wav-trigger-hookup-guide

  It uses the following hardware:
  - SparkFun Tsunami Super WAV Trigger Qwiic
  - SparkFun RedBoard Qwiic as the controller.
  - SparkFun Qwiic Distance Sensors (VL53L1X)
  - Qwiic Mux to control communication between two distance sensors and Tsunami
  - Qwiic cables to wire everything up.

  The Tsunami will continuousely loop a 400Hz Sine wav ("17.WAV") on output "1L".
  The first distance sensor will control volume.
  The second distance sensor will control the pitch of the sine wave.

  You can also load up some more backing tracks to have a complete Theremin play-along device!

  Hardware Qwiic connections:
  Redboard --->>> Qwic Mux "Main" port
  Mux port 7 --->>> sensor A --->>> Tsunami
  Mux port 0 --->>> sensor B
 
  Tsunami output 1L to amp and speaker

  Resources:
  Wire.h (included with Arduino IDE)
  SparkFun_Tsunami_Qwiic.h (included in the src folder) http://librarymanager/All#SparkFun_Tsunami

  Development environment specifics:
  Arduino 1.8.13
  Tsunami Hardware Version v21
  Redboard Qwiic Version v10

  Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFun_Tsunami_Qwiic.h>
#include "SparkFun_VL53L1X.h"
#include "SparkFun_I2C_Mux_Arduino_Library.h"

#define LED                         13          // Redboard LED
#define BUTTON                      2           // Button connected to D2
#define DEBUG1                      3           // Debug output 1 on D3, used for scope timing
#define DEBUG2                      4           // Debug output 2 on D4, used for scope timing

#define TRACK_SINE_WAV              17          // 440Hz 30s sine wave as "17.WAV"
#define OUTPUT_CHANNEL              0           // output to play the track on

// Change these defines to match which Mux ports your distance sensors are connected to.
#define MUX_PITCH_PORT              0
#define MUX_VOLUME_PORT             7

#define BUTTON_SCAN_MS              21
#define HEARTBEAT_UPDATE_MS         250
#define PITCH_UPDATE_MS             12

#define VOLUME_MEASURE_PERIOD_MS    50
#define PITCH_MEASURE_PERIOD_MS     20

#define MIN_GAIN_DB                 -60
#define MAX_GAIN_DB                 0
#define MIN_PITCH                   -32768      // Down one octave
#define MAX_PITCH                   32767       // Up one octave
#define MIN_DISTANCE_MM             50          // Minimum distance in mm
#define MAX_DISTANCE_MM             550         // Maximum distance in mm

TsunamiQwiic tsunami;
SFEVL53L1X **distanceSensor;
QWIICMUX mux;

unsigned long last_button_scan;
unsigned long last_heartbeat_update;
unsigned long last_pitch_update;

byte gLedState = 0;
byte gButtonState = 0;
byte gButtonNewState;

char versionResponse[36]; // an array of chars to store the version string
int NumTracks = 0; // used to store the value returned from the tsunami

int pitchDistance = MAX_DISTANCE_MM;
float lastPitchDistance = 0.0;
int lastPitch = MAX_PITCH;

// A simple running average filter to smooth out pitch changes.
#define PITCH_FILTER_COUNT  6.0
#define PITCH_FILTER_MULT   ((PITCH_FILTER_COUNT - 1.0) / PITCH_FILTER_COUNT)
float pitchFiltMult = PITCH_FILTER_MULT;
float pitchAccumulator = 0.0;
float pitchAverage = 0.0;

int volumeDistance = MAX_DISTANCE_MM;
float lastVolumeDistance = 0.0;
int lastGain = MIN_GAIN_DB;


// ****************************************************************************
void setup() {

    pinMode(LED, OUTPUT);
    digitalWrite(LED, gLedState);

    pinMode(DEBUG1, OUTPUT);
    digitalWrite(DEBUG1, LOW);
    pinMode(DEBUG2, OUTPUT);
    digitalWrite(DEBUG2, LOW);

    pinMode(BUTTON, INPUT_PULLUP);
    gButtonState = digitalRead(BUTTON);

    Wire.begin();  
    Serial.begin(115200);

    distanceSensor = new SFEVL53L1X *[2];
    distanceSensor[0] = new SFEVL53L1X(Wire);
    distanceSensor[1] = new SFEVL53L1X(Wire);

    if (mux.begin() == false) {
        Serial.println("Qwiic Mux not found");
        while(1);
    }
    mux.setPort(MUX_PITCH_PORT);
    if (distanceSensor[0]->begin() != 0) {
        Serial.println("Pitch distance sensor not found");
    }
    else {
        distanceSensor[0]->setTimingBudgetInMs(PITCH_MEASURE_PERIOD_MS);
        distanceSensor[0]->setIntermeasurementPeriod(PITCH_MEASURE_PERIOD_MS);
        distanceSensor[0]->setDistanceModeShort();
        distanceSensor[0]->startRanging();
    }
    
    mux.setPort(MUX_VOLUME_PORT);
    if (distanceSensor[1]->begin() != 0) {
        Serial.println("Volume distance sensor not found");
    }
    else {
        distanceSensor[1]->setTimingBudgetInMs(VOLUME_MEASURE_PERIOD_MS);
        distanceSensor[1]->setIntermeasurementPeriod(VOLUME_MEASURE_PERIOD_MS);
        distanceSensor[1]->setDistanceModeShort();
        distanceSensor[1]->startRanging();
    }

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

    tsunami.trackLoop(TRACK_SINE_WAV, true);
    tsunami.trackGain(TRACK_SINE_WAV, MIN_GAIN_DB);
    tsunami.trackPlayPoly(TRACK_SINE_WAV, 0, true);

    last_button_scan = millis();
    last_heartbeat_update = millis();
    last_pitch_update = millis();
}

// ****************************************************************************
void loop() {

float fTmp;
int iTmp;

    // Scan the button if time to do so. Just for something to do, we'll request and print
    // Tsunami's version string and the number of tracks.
    if ((millis() - last_button_scan) > BUTTON_SCAN_MS) {
        last_button_scan = millis();
        gButtonNewState = digitalRead(BUTTON);
        if (gButtonNewState != gButtonState) {
            gButtonState = gButtonNewState;
            if (!gButtonState) {
                tsunami.getVersion(versionResponse);
                Serial.println(versionResponse);
                NumTracks = tsunami.getNumTracks();
                Serial.print("Number of Tracks: ");
                Serial.println(NumTracks);
            }
        }      
    }

    // Service the pitch distance sensor. 20ms seems to be about the fastest we can sample the distance sensor. We need
    //  to update pitch more often than this however, so here we just grab the current value, do a sanity check for stray values
    //  and limit to our defined range.
    mux.setPort(MUX_PITCH_PORT);
    if (distanceSensor[0]->checkForDataReady()) {
        digitalWrite(DEBUG1, HIGH);
        fTmp = distanceSensor[0]->getDistance();
        distanceSensor[0]->clearInterrupt();        
        if (abs(fTmp - lastPitchDistance) < 200.0) {
            iTmp = (int)(fTmp + 0.5);
            if (iTmp > MAX_DISTANCE_MM) iTmp = MAX_DISTANCE_MM;
            else if (iTmp < MIN_DISTANCE_MM) iTmp = MIN_DISTANCE_MM;
            pitchDistance = iTmp;
        }
        lastPitchDistance = fTmp;
        digitalWrite(DEBUG1, LOW); 
    }

    // Service the volume distance sensor. We're using Tsunami built in faders to smooth out the volume changes, so
    //  we don't need to read this sensor as often.
    mux.setPort(MUX_VOLUME_PORT);
    if (distanceSensor[1]->checkForDataReady()) {
        digitalWrite(DEBUG2, HIGH);
        fTmp = distanceSensor[1]->getDistance();
        distanceSensor[1]->clearInterrupt();
        
        if (abs(fTmp - lastVolumeDistance) < 200.0) {
            iTmp = (int)(fTmp + 0.5);
            if (iTmp > MAX_DISTANCE_MM) iTmp = MAX_DISTANCE_MM;
            else if (iTmp < MIN_DISTANCE_MM) iTmp = MIN_DISTANCE_MM;
            if (iTmp != volumeDistance) {
                volumeDistance = iTmp;
                fTmp = (float)((volumeDistance - MIN_DISTANCE_MM) / (float)(MAX_DISTANCE_MM - MIN_DISTANCE_MM));
                fTmp = (float)MAX_GAIN_DB - (float)(MAX_GAIN_DB - MIN_GAIN_DB) * fTmp;
                iTmp = (int)(fTmp - 0.5);
                if (iTmp != lastGain) {
                    lastGain = iTmp;
                    tsunami.trackFade(TRACK_SINE_WAV, iTmp, (VOLUME_MEASURE_PERIOD_MS + 20), false);
                    Serial.println(iTmp);
                }
            }
        }
        lastVolumeDistance = fTmp;
        digitalWrite(DEBUG2, LOW);   
    }

    // Update Tsunami samplerate offset. Here's where we update Tsunami's samplerate to adjust pitch. We're using a
    //  simply running average filter to smooth out the pitch changes.
    if ((millis() - last_pitch_update) > PITCH_UPDATE_MS) {
        last_pitch_update = millis();
            
        pitchAccumulator = (pitchAccumulator * pitchFiltMult) + (float)pitchDistance;
        pitchAverage = pitchAccumulator / PITCH_FILTER_COUNT;
        fTmp = (float)(pitchAverage - MIN_DISTANCE_MM) / (float)(MAX_DISTANCE_MM - MIN_DISTANCE_MM);
        fTmp = (float)MIN_PITCH + (float)(MAX_PITCH - MIN_PITCH) * fTmp;
        iTmp = (int)(fTmp + 0.5);
        if (iTmp != lastPitch) {
            lastPitch = iTmp;
            tsunami.samplerateOffset(OUTPUT_CHANNEL, iTmp); // Note, first argument is OUTPUT (not track)
        }
    }      

    // Blink the heartbeat LED
    if ((millis() - last_heartbeat_update) > HEARTBEAT_UPDATE_MS) {
        last_heartbeat_update = millis();
               
        if (gLedState == 0) gLedState = 1;
        else gLedState = 0;
        digitalWrite(LED, gLedState);   
    }

}