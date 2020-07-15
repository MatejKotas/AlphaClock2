void setup()
{
  a5Init();

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

  if (brightness == 0) {
    brightness = 1;
  }

  updateBrightness();

  updateDisplay = true;

  AlarmsDone = 255; // Prevent any alarms from going off after startup

  StopwatchStart = 0;
  StopwatchPause = 0;
  StopwatchRunning = false;
  StopwatchShifted = 0;
  StopwatchLap = false;
  StopwatchSeperator = false;
  
  milliseconds = millis() / 2;

  FadeUpdateTimer = milliseconds;
  UpdateEEPROMTimer = milliseconds;

  for (int i = 0; i < Buttons; i++) {
    ButtonReadyTimes[i] = milliseconds;
  }
}
