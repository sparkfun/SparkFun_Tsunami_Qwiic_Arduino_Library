// **************************************************************
//     Filename: SparkFun_Tsunami_Qwiic.h
// Date Created: 6/6/2021
//
//     Comments: Robertsonics Tsunami v2 Qwiic library
//
// Programmers: Jamie Robertson, info@robertsonics.com
//
// **************************************************************

#ifndef _20210606_TSUNAMIQWIIC_H_
#define _20210606_TSUNAMIQWIIC_H_

#define TSUNAMI_NUM_OUTPUTS	8

#define CMD_GET_VERSION				1
#define CMD_GET_SYS_INFO			2
#define CMD_TRACK_CONTROL			3
#define CMD_STOP_ALL				4
#define CMD_MASTER_VOLUME			5
#define CMD_TRACK_VOLUME			8
#define CMD_TRACK_FADE				10
#define CMD_RESUME_ALL_SYNC			11
#define CMD_SAMPLERATE_OFFSET		12
#define CMD_SET_TRIGGER_BANK		14
#define CMD_SET_INPUT_MIX			15
#define CMD_SET_MIDI_BANK			16
#define CMD_GET_TRACK_STATUS		17

#define TRK_PLAY_SOLO				0
#define TRK_PLAY_POLY				1
#define TRK_PAUSE					2
#define TRK_RESUME					3
#define TRK_STOP					4
#define TRK_LOOP_ON					5
#define TRK_LOOP_OFF				6
#define TRK_LOAD					7

#define MAX_MESSAGE_LEN				32
#define VERSION_STRING_LEN			22

#define IMIX_OUT1	0x01
#define IMIX_OUT2	0x02
#define IMIX_OUT3	0x04
#define IMIX_OUT4	0x08

#include <Wire.h>

class TsunamiQwiic
{
public:
	TsunamiQwiic() {;}
	~TsunamiQwiic() {;}
	bool begin(uint8_t deviceAddress = 0x13, TwoWire &wirePort = Wire);
	uint8_t getAddress();
	bool getVersion(char *pDst);
	int getNumTracks(void);
	bool isTrackPlaying(int trk);
	void masterGain(int out, int gain);
	void stopAllTracks(void);
	void resumeAllInSync(void);
	void trackPlaySolo(int trk, int out, bool lock);
	void trackPlayPoly(int trk, int out, bool lock);
	void trackLoad(int trk, int out, bool lock);
	void trackStop(int trk);
	void trackPause(int trk);
	void trackResume(int trk);
	void trackLoop(int trk, bool enable);
	void trackGain(int trk, int gain);
	void trackFade(int trk, int gain, int time, bool stopFlag);
	void samplerateOffset(int out, int offset);
	void setTriggerBank(int bank);
	void setInputMix(int mix);
	void setMidiBank(int bank);

private:
	void trackControl(int trk, int code, int out, int flags);
	void write(uint8_t * buf, int cnt);
	
	TwoWire *_i2cPort = NULL; //The generic connection to user's chosen I2C hardware
	uint8_t _deviceAddress;   // Address of Tsunami

	char version[VERSION_STRING_LEN];
};

#endif
