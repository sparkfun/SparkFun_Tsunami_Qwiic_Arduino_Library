/******************************************************************************
  track_numbers.h
  (used with Example_08_DrumMachine.ino)

  Example for the SparkFun Tsunami Super WAV Trigger Qwiic
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: Jamie Robertson (info@robertsonics.com), Pete Lewis
  Date Created: 5/18/2021
  ~

  This file contains the defines for the track numbers.

  Note, it also has the global int variables that are used elsewhere in the example.

  Distributed as-is; no warranty is given.
******************************************************************************/

// WAVE File number defines
// (these match up with the actual WAV file names on the uSD card)
#define KICK 100
#define SNARE 101
#define HAT 102
#define RIDE 103

// set default tracks assigned to actively used kick/snare/hat/etc. varialbles
// variables used for playback in main program
int kick_t = KICK;
int snare_t = SNARE;
int hat_t = HAT;
int ride_t = RIDE;
