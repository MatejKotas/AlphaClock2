void loop()
{
  milliseconds = millis() / 2;

  Fade();

  ManageButtons();

  if (CheckTimer - milliseconds > MillisHalfOverflow) { // Triggers every 500 ms
    Check();
  }

  switch (State) {
    case TimeState:
      ShowTime();
      break;
    case MenuState:
      ShowMainMenu();
      break;
    case TimeMenuState:
      ShowTimeMenu();
      break;
    case AlarmMenuState:
      ShowAlarmMenu();
      break;
    case AlarmNowState:
      ShowAlarmNow();
      break;
    case ShowDateState:
      ShowDate();
      break;
    case StopwatchState:
      ShowStopwatch();
      break;
  }
}

void Check() {
  bool nightTemp = isNight();
  CheckTimer = milliseconds + CheckTimerInterval;
  if (nightTemp != night) {
    night = nightTemp;
    updateNightLight(night);

    if (night) {
      brightness = NightBrightness;
    }
    else {
      brightness = DayBrightness;
    }
    updateBrightness();
  }
  if (EEPROMUpdatePending && UpdateEEPROMTimer - milliseconds > MillisHalfOverflow) {
    writeEEPROM();
  }
  CheckAlarm();
}
