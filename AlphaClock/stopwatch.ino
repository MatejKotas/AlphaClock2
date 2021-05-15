void StopwatchTransition() {
  GeneralTimer = milliseconds; // Acts as update display timer
  updateDisplay = true;

  State = StopwatchState;
}

void ShowStopwatch() {
  bool ToggleLap = GetButtonPair(PlusButton, MinusButton);
  updateDisplay |= ToggleLap;

  if (ToggleLap) {}
  else if (GetButton(AlarmSetButton)) {
    TimeTransition();
    return;
  }
  else if (GetHoldButton_oneTrigger(MinusButton)) {
    if (StopwatchStart != 0) {
      updateDisplay = true;
    }
    StopwatchStart = 0;
    StopwatchPause = 0;
    StopwatchShifted = 0;
    StopwatchRunning = false;
  }
  else if (GetButton(PlusButton)) {
    if (StopwatchShifted < 6) {
      StopwatchShifted++;
    }
    updateDisplay = true;
  }
  else if (GetButton(MinusButton)) {
    if (StopwatchShifted > 0) {
      StopwatchShifted--;
    }
    updateDisplay = true;
  }
  else if (GetButton(TimeSetButton)) {
    updateDisplay = true;
    if (StopwatchRunning) {
      StopwatchRunning = false;
      StopwatchPause = milliseconds;
    }
    else {
      StopwatchRunning = true;
      StopwatchStart += milliseconds - StopwatchPause;
    }
  }

  unsigned long difference = milliseconds - StopwatchStart;

  if (StopwatchRunning) {
    bool seperatorTemp = seperator(difference);
    if (seperatorTemp != StopwatchSeperator) {
      StopwatchSeperator = seperatorTemp;
      updateDisplay = true;
    }
  }
  else {
    StopwatchSeperator = true;
  }

  if (updateDisplay || GeneralTimer - milliseconds > MillisHalfOverflow) {
    GeneralTimer = milliseconds + (1000 / (Base * Base));
    updateDisplay = false;

    if (!StopwatchRunning) {
      difference = StopwatchPause - StopwatchStart;
      GeneralTimer = milliseconds + 10000; // Don't update regularly when paused
    }

    char temp[5] = "00000";
    char tempDP[5] = "00000";

    if (!StopwatchLap) {
      char StopwatchStore[11] = "00000000000";

      int differenceTemp = difference % 1000 * 10;
      int base_dividor = 10000 / Base;

      StopwatchStore[10] = ToCurrentBase(differenceTemp % base_dividor / 100, 0); // centiseconds
      StopwatchStore[9] = ToCurrentBase(differenceTemp / base_dividor, 0); // deciseconds

      difference /= 1000;

      differenceTemp = difference % 60;
      StopwatchStore[8] = ToCurrentBase(differenceTemp, 0); // seconds
      StopwatchStore[7] = ToCurrentBase(differenceTemp, 1); // 10 seconds
      difference /= 60;

      differenceTemp = difference % 60;
      StopwatchStore[6] = ToCurrentBase(differenceTemp, 0); // minutes
      StopwatchStore[5] = ToCurrentBase(differenceTemp, 1); // 10 minutes
      difference /= 60;

      differenceTemp = difference % 24;
      StopwatchStore[4] = ToCurrentBase(differenceTemp, 0); // hours
      StopwatchStore[3] = ToCurrentBase(differenceTemp, 1); // 10 hours
      difference /= 24;

      differenceTemp = difference % 7;
      StopwatchStore[2] = ToCurrentBase(differenceTemp, 0); // days
      difference /= 7;

      differenceTemp = difference % (Base * Base);
      StopwatchStore[1] = ToCurrentBase(differenceTemp, 0); // weeks
      StopwatchStore[0] = ToCurrentBase(differenceTemp, 1); // 10 weeks

      if (ToggleLap) {
        if (!StopwatchLap) {
          for (int i = 0; i < 11; i++) {
            StopwatchLapStore[i] = StopwatchStore[i];
          }
        }
      }

      int j = 0;
      for (int i = 6 - StopwatchShifted; i < 11 - StopwatchShifted; i++) {
        temp[j] = StopwatchStore[i];
        j++;
      }
    }
    else {
      int j = 0;
      for (int i = 6 - StopwatchShifted; i < 11 - StopwatchShifted; i++) {
        temp[j] = StopwatchLapStore[i];
        j++;
      }
      GeneralTimer = milliseconds + 10000; // Don't update regularly in lap mode
    }

    if (ToggleLap) {
      StopwatchLap = !StopwatchLap;
      if (!StopwatchLap) {
        updateDisplay = true;
      }
    }

    if (StopwatchSeperator) {
      int k = 0;
      for (int i = 6 - StopwatchShifted; i < 11 - StopwatchShifted; i++) {
        tempDP[k++] = StopwatchDP[i];
      }
    }

    if (tempDP[0] == '2') {
      tempDP[0] = '0';
    }
    else if (tempDP[0] == '3') {
      tempDP[0] = '1';
    }

    if (tempDP[4] == '1') {
      tempDP[4] = '0';
    }
    else if (tempDP[4] == '3') {
      tempDP[4] = '2';
    }

    Display(temp, tempDP, false);
  }
}

bool seperator(unsigned long difference) {
  return (difference % 1000 >= 500);
}
