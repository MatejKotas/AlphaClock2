#define AlarmBeeps 4
#define AlarmBeepRate 200
#define AlarmBeepDuration 100

void AlarmMenuTransition() {
  MenuTransition();
  State = AlarmMenuState;
  for (int i = 0; i < Alarms - 1; i++) {
    CurrentAlarm = i + 1;
    DecreaseAlarm(Snoozed[CurrentAlarm - 1] * SnoozeMinutes);
    Snoozed[CurrentAlarm - 1] = 0;
  }
  CurrentAlarm = 0;
}

void ShowAlarmNow() {
  if (NextBeepStateChange - milliseconds > MillisHalfOverflow) {
    if (BeepState < AlarmBeeps) {
      a5tone(880, AlarmBeepDuration);
      NextBeepStateChange = milliseconds + AlarmBeepRate;
      BeepState++;
    }
    else {
      BeepState = 0;
      NextBeepStateChange = milliseconds + (AlarmBeepRate * AlarmBeeps);
    }
  }
  else if (GeneralTimer - milliseconds > MillisHalfOverflow) {
    TimeTransition();
  }

  if (GetButton(TimeSetButton)) {
    DecreaseAlarm(Snoozed[CurrentAlarm - 1] * SnoozeMinutes);
    Snoozed[CurrentAlarm - 1] = 0;
    TimeTransition();
  }
  else if (GetButton(AlarmSetButton)) {
    IncreaseAlarm(SnoozeMinutes);
    Snoozed[CurrentAlarm - 1]++;
    bitSet(AlarmsDone, CurrentAlarm);
    TimeTransition();
  }
}

void AlarmNowTransition() {
  startEEPROMSaveTimer();
  State = AlarmNowState;
  NextBeepStateChange = milliseconds;
  GeneralTimer = milliseconds + (10 * 60 * 1000); // In alarm now mode, generalTimer is timer for turning off alarm. Turn off alarm in 10 minutes
  Display("ALARM", "00000", true);
}

void CheckAlarm() {
  byte hourTemp = hour();
  byte minuteTemp = minute();

  if (AlarmsDone > 0) {
    AlarmsDone &= 0b00111111;

    if (minuteTemp != 0) {
      bitClear(AlarmsDone, 0);
    }

    for (int i = 0; i < Alarms - 1; i++) {
      if (minuteTemp != AlarmTimesMinute[i]) {
        bitClear(AlarmsDone, i + 1);
      }
    }
  }

  if (AlarmsEnabled > 0 && State != AlarmMenuState) {
    AlarmsEnabled &= 0b00111111;

    if (minuteTemp == 0 && !bitRead(AlarmsDone, 0) && bitRead(AlarmsEnabled, 0)) {
      bitSet(AlarmsDone, 0);
      a5tone(880, 200);
    }
    for (int i = 0; i < Alarms - 1; i++) {
      if (minuteTemp == AlarmTimesMinute[i] && hourTemp == AlarmTimesHour[i] && !bitRead(AlarmsDone, i + 1) && bitRead(AlarmsEnabled, i + 1)) {
        bitSet(AlarmsDone, i + 1);
        CurrentAlarm = i + 1;
        AlarmNowTransition();
      }
    }
  }
}

void ShowAlarmMenu() {
  byte optionValue = Menu();

  if (GetHoldButton_oneTrigger(AlarmSetButton)) {
    StopwatchTransition();
    return;
  }

  if (updateDisplay) {
    updateDisplay = false;
    if (editing == 0) {
      CurrentAlarm = changeOption(CurrentAlarm, optionValue, Alarms);

      char temp_DP[5] = "00000";
      if (bitRead(AlarmsEnabled, CurrentAlarm)) {
        temp_DP[0] = '2';
      }

      if (CurrentAlarm == 0) {
        Display("SIGNL", temp_DP, true);
      }
      else {
        char temp[5] = " AL_ ";
        temp[3] = CurrentAlarm + '0';

        Display(temp, temp_DP, true);
      }
    }
    else if (editing == 1) {
      char temp_DP2[5] = "01200";

      if (bitRead(AlarmsEnabled, CurrentAlarm)) {
        temp_DP2[0] = '2';
      }

      if (CurrentAlarm == 0) { // SIG
        Display(" _00 ", temp_DP2, false);
      }
      else {
        if (optionValue == 1) {
          IncreaseAlarm(1);
        }
        else if (optionValue == 255) {
          DecreaseAlarm(1);
        }
        char temp2[5] = "     ";

        // <SpaghettiCode meta="Displaing alarm">
        if (Mode24H) {
          temp2[0] = ToCurrentBase(AlarmTimesHour[CurrentAlarm - 1], 1);
          temp2[1] = ToCurrentBase(AlarmTimesHour[CurrentAlarm - 1], 0);
        }
        else {
          byte mod12 = AlarmTimesHour[CurrentAlarm - 1];
          if (mod12 > 12) {
            mod12 -= 12;
          }
          temp2[0] = ToCurrentBase(mod12, 1);
          temp2[1] = ToCurrentBase(mod12, 0);

          if (AlarmTimesHour[CurrentAlarm - 1] >= 12) {
            temp2[4] = 'P';
          }
          else {
            temp2[4] = 'A';
          }
        }
        if (temp2[0] == '0') {
          temp2[0] = ' ';
        }
        temp2[2] = ToCurrentBase(AlarmTimesMinute[CurrentAlarm - 1], 1);
        temp2[3] = ToCurrentBase(AlarmTimesMinute[CurrentAlarm - 1], 0);
        // </SpaghettiCode>

        Display(temp2, temp_DP2, false);
      }
    }
    else if (editing == 2) {
      bitWrite(AlarmsEnabled, CurrentAlarm, !bitRead(AlarmsEnabled, CurrentAlarm));
      editing = 1;
      updateDisplay = true;
    }
  }
}

void DecreaseAlarm(byte amount) {
  AlarmTimesMinute[CurrentAlarm - 1] -= amount;
  if (AlarmTimesMinute[CurrentAlarm - 1] >= 255) {
    AlarmTimesMinute[CurrentAlarm - 1] += 60;
    AlarmTimesHour[CurrentAlarm - 1]--;
    AlarmTimesHour[CurrentAlarm - 1] += 24;
    AlarmTimesHour[CurrentAlarm - 1] %= 24;
  }
}

void IncreaseAlarm(byte amount) {
  AlarmTimesMinute[CurrentAlarm - 1] += amount;

  if (AlarmTimesMinute[CurrentAlarm - 1] >= 60) {
    AlarmTimesMinute[CurrentAlarm - 1] -= 60;
    AlarmTimesHour[CurrentAlarm - 1]++;
    AlarmTimesHour[CurrentAlarm - 1] %= 24;
  }
}
