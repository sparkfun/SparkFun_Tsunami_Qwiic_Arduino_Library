// **************************************************************
//     Filename: TsunamiQwiic.cpp
// Date Created: 6/6/2021
//
//     Comments: Robertsonics Tsunami v2 Qwiic library
//
// Programmers: Jamie Robertson, info@robertsonics.com
//
// **************************************************************

#include "TsunamiQwiic.h"
#include <Arduino.h>


// **************************************************************
void TsunamiQwiic::start(void) {

  	//Wire.begin();
}

// **************************************************************
bool TsunamiQwiic::getVersion(char *pDst) {

uint8_t txbuf[8];
char * pStr = pDst;

	txbuf[0] = CMD_GET_VERSION;
	write(txbuf, 1);
	delay(2);
	Wire.requestFrom(0x13, VERSION_STRING_LEN);
	while (Wire.available()) {
		*pDst++ = Wire.read();
	}
	*pDst = 0;
	return true;
}

// **************************************************************
int TsunamiQwiic::getNumTracks(void) {

int i = 0;
uint8_t txbuf[8];
uint8_t rxbuf[MAX_MESSAGE_LEN];
uint16_t numTracks;

	txbuf[0] = CMD_GET_SYS_INFO;
	write(txbuf, 1);
	delay(2);
	Wire.requestFrom(0x13, 3);
	while (Wire.available()) {
		rxbuf[i++] = Wire.read();
	}
	numTracks = rxbuf[2];
	numTracks = (numTracks << 8) + rxbuf[1];
	return numTracks;
}


// **************************************************************
bool TsunamiQwiic::isTrackPlaying(int trk) {

bool fResult = false;
uint8_t txbuf[8];

	if (trk <= 0)
		return false;

	txbuf[0] = CMD_GET_TRACK_STATUS;
	txbuf[1] = (uint8_t)(trk - 1);
	txbuf[2] = (uint8_t)((trk - 1) >> 8);
	write(txbuf, 3);
	delay(2);
	Wire.requestFrom(0x13, 1);
	while (Wire.available()) {
		fResult = Wire.read();
	}
	return fResult;
}

// **************************************************************
void TsunamiQwiic::masterGain(int out, int gain) {

uint8_t txbuf[8];
unsigned short vol;
uint8_t o;

	o = out & 0x03;
	txbuf[0] = CMD_MASTER_VOLUME;
	txbuf[1] = o;
	vol = (unsigned short)gain;
	txbuf[2] = (uint8_t)vol;
	txbuf[3] = (uint8_t)(vol >> 8);
	write(txbuf, 4);
}


// **************************************************************
void TsunamiQwiic::trackPlaySolo(int trk, int out, bool lock) {

int flags = 0;

	if (lock)
	  flags |= 0x01;
	trackControl(trk, TRK_PLAY_SOLO, out, flags);
}

// **************************************************************
void TsunamiQwiic::trackPlayPoly(int trk, int out, bool lock) {
  
int flags = 0;

	if (lock)
	  flags |= 0x01;
	trackControl(trk, TRK_PLAY_POLY, out, flags);
}

// **************************************************************
void TsunamiQwiic::trackLoad(int trk, int out, bool lock) {
  
int flags = 0;

	if (lock)
	  flags |= 0x01;
	trackControl(trk, TRK_LOAD, out, flags);
}

// **************************************************************
void TsunamiQwiic::trackStop(int trk) {

	trackControl(trk, TRK_STOP, 0, 0);
}

// **************************************************************
void TsunamiQwiic::trackPause(int trk) {

	trackControl(trk, TRK_PAUSE, 0, 0);
}

// **************************************************************
void TsunamiQwiic::trackResume(int trk) {

	trackControl(trk, TRK_RESUME, 0, 0);
}

// **************************************************************
void TsunamiQwiic::trackLoop(int trk, bool enable) {
 
	if (enable)
	trackControl(trk, TRK_LOOP_ON, 0, 0);
	else
	trackControl(trk, TRK_LOOP_OFF, 0, 0);
}

// **************************************************************
void TsunamiQwiic::trackControl(int trk, int code, int out, int flags) {
  
uint8_t txbuf[8];
uint8_t o;

	o = out & 0x07;
	txbuf[0] = CMD_TRACK_CONTROL;
	txbuf[1] = (uint8_t)code;
	txbuf[2] = (uint8_t)trk;
	txbuf[3] = (uint8_t)(trk >> 8);
	txbuf[4] = (uint8_t)o;
	txbuf[5] = (uint8_t)flags;
	write(txbuf, 6);
}

// **************************************************************
void TsunamiQwiic::stopAllTracks(void) {

uint8_t txbuf[8];

	txbuf[0] = CMD_STOP_ALL;
	write(txbuf, 1);
}

// **************************************************************
void TsunamiQwiic::resumeAllInSync(void) {

uint8_t txbuf[8];

	txbuf[0] = CMD_RESUME_ALL_SYNC;
	write(txbuf, 1);
}

// **************************************************************
void TsunamiQwiic::trackGain(int trk, int gain) {

uint8_t txbuf[8];
unsigned short vol;

	txbuf[0] = CMD_TRACK_VOLUME;
	txbuf[1] = (uint8_t)trk;
	txbuf[2] = (uint8_t)(trk >> 8);
	vol = (unsigned short)gain;
	txbuf[3] = (uint8_t)vol;
	txbuf[4] = (uint8_t)(vol >> 8);
	write(txbuf, 5);
}

// **************************************************************
void TsunamiQwiic::trackFade(int trk, int gain, int time, bool stopFlag) {

uint8_t txbuf[12];
unsigned short vol;

	txbuf[0] = CMD_TRACK_FADE;
	txbuf[1] = (uint8_t)trk;
	txbuf[2] = (uint8_t)(trk >> 8);
	vol = (unsigned short)gain;
	txbuf[3] = (uint8_t)vol;
	txbuf[4] = (uint8_t)(vol >> 8);
	txbuf[5] = (uint8_t)time;
	txbuf[6] = (uint8_t)(time >> 8);
	txbuf[7] = stopFlag;
	write(txbuf, 8);
}

// **************************************************************
void TsunamiQwiic::samplerateOffset(int out, int offset) {

uint8_t txbuf[8];
unsigned short off;
uint8_t o;

	o = out & 0x03;
	txbuf[0] = CMD_SAMPLERATE_OFFSET;
	txbuf[1] = (uint8_t)o;
	off = (unsigned short)offset;
	txbuf[2] = (uint8_t)off;
	txbuf[3] = (uint8_t)(off >> 8);
	write(txbuf, 4);
}

// **************************************************************
void TsunamiQwiic::setTriggerBank(int bank) {

uint8_t txbuf[8];

	txbuf[0] = CMD_SET_TRIGGER_BANK;
	txbuf[1] = (uint8_t)bank;
	write(txbuf, 2);
}

// **************************************************************
void TsunamiQwiic::setInputMix(int mix) {

uint8_t txbuf[8];

	txbuf[0] = CMD_SET_INPUT_MIX;
	txbuf[1] = (uint8_t)mix;
	write(txbuf, 2);
}

// **************************************************************
void TsunamiQwiic::setMidiBank(int bank) {

uint8_t txbuf[8];

	txbuf[0] = CMD_SET_MIDI_BANK;
	txbuf[0] = (uint8_t)bank;
	write(txbuf, 2);
}

// **************************************************************
void TsunamiQwiic::write(uint8_t * buf, int cnt) {

int bCnt = 0;
int i;

	Wire.beginTransmission(0x13);
	for (i = 0; i < cnt; i++)
		Wire.write(buf[bCnt++]);
	Wire.endTransmission();
}




