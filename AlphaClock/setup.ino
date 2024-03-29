void setup()
{
  a5Init();

  Serial.println("Init.");

  readEEPROM();
  a5loadAltNumbers(Font);

  Fading = false;

  State = TimeState;

  SecondTemp = 0;
  EEPROMUpdatePending = false;

  RTCpresent = a5CheckForRTC();
  if (RTCpresent)
  {
    setSyncProvider(RTC.get);
    if (timeStatus() != timeSet) {
      Serial.println("RTC not synced.");
      RTCpresent = false;
    }
  }

  if (isNight()) {
    updateNightLight(true);
    brightness = NightBrightness;
    night = true;
  }
  else {
    updateNightLight(false);
    brightness = DayBrightness;
    night = false;
  }

  updateBrightness();

  updateDisplay = true;

  AlarmTriggered = false;

  StopwatchStart = 0;
  StopwatchPause = 0;
  StopwatchRunning = false;
  StopwatchShifted = 0;
  StopwatchLap = false;
  StopwatchSeperator = false;
  
  FadeUpdateTimer = milliseconds;
  UpdateEEPROMTimer = milliseconds;

  for (int i = 0; i < Buttons; i++) {
    ButtonReadyTimes[i] = milliseconds;
  }
}
