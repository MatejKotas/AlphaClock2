#include "alphafive.h"
#include <Wire.h>
#include <DS1307RTC.h>
#include <EEPROM.h>
#include <TimeLib.h>

unsigned long FadeUpdateTimer;
bool Fading;

unsigned long milliseconds;

unsigned long CheckTimer;
#define CheckTimerInterval 500

unsigned long GeneralTimer; // Changes function based on mode

// Stopwatch

unsigned long StopwatchStart;
unsigned long StopwatchPause;
bool StopwatchRunning;
byte StopwatchShifted; // Display shifting
const char StopwatchDP[11] = "01321212100";
bool StopwatchLap;
char StopwatchLapStore[11] = "0000000000";
bool StopwatchSeperator;

// Alarms

#define Alarms 6

byte AlarmTimesHour[Alarms - 1]; // Alarm 0 is sig, always subtract 1 from index
byte AlarmTimesMinute[Alarms - 1];
byte AlarmsEnabled;
byte AlarmsDone;

byte CurrentAlarm;

byte Snoozed[Alarms - 1];
#define SnoozeMinutes 5

byte BeepState;
unsigned long NextBeepStateChange;

// EEPROM

unsigned long UpdateEEPROMTimer;
bool EEPROMUpdatePending;

// Brightness variables

byte brightness;

const byte BrightnessLevel[] = { // Copied form original sketch
  0, 1, 5, 10, 15, 19, 15, 19, 5, 10, 15, 19
};
const byte BrightnessMode[] = { // Copied form original sketch
  0, 0, 0, 0,  0,  0,  1,  1,  2, 2,  2,  2
};

#define BrightnessLevels 12

bool RTCpresent;
bool night;

bool updateDisplay;

// Night Light

#define NightLightMax 20
#define NightLightMultiplier 12 // 255/20

// State variables

byte State;

#define TimeState 0
#define MenuState 1
#define TimeMenuState 2
#define AlarmMenuState 3
#define AlarmNowState 4
#define ShowDateState 5
#define StopwatchState 6

// Base 10 / 16 / 8

byte Base;

// Time variables

byte SecondTemp;

// Settings store variables

byte Font;
#define Fonts 10
bool UseBrightnessAdjustment;
byte NightStart;
byte NightEnd;
byte NightBrightness;
byte DayBrightness;
bool Mode24H;
byte LightDay;
byte LightNight;

bool UseSpinner;

// Factory defaults

#define DefaultFont 0
#define DefaultBrightnessAdjustmentEnabled 0
#define DefaultNightStart 20
#define DefaultNightEnd 8
#define DefaultNightBrightness 1
#define DefaultDayBrightness 20
#define DefaultMode24H 1
#define DefaultLightDay 0
#define DefaultLightNight 10

#define DefaultUseSpinner 1
#define DefaultBase 10
