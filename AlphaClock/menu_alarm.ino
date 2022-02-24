#define AlarmBeeps 4
#define AlarmBeepRate 200
#define AlarmBeepDuration 100

void ChangeAlarm(int amount) { // Increases the Alarm time amount minutes. Decreases Alarm time when amount is negative.
  int temp = AlarmMinute + amount;
  int temp2 = AlarmHour;
  while (temp < 0) {
    temp += 60;
    temp2 -= 1;
  }

  AlarmMinute = temp % 60;
  AlarmHour = (temp2 + (temp / 60)) % 24;
}

void ShowAlarmNow() {
  if ((long)(NextBeepStateChange - milliseconds) < 0) {
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
  else if ((long)(GeneralTimer - milliseconds) < 0) {
    TimeTransition();
  }

  if (GetButton(TimeSetButton)) {
    ChangeAlarm(-AlarmSnoozed * SnoozeMinutes);
    AlarmSnoozed = 0;
    TimeTransition();
  }
  else if (GetButton(AlarmSetButton)) {
    ChangeAlarm(SnoozeMinutes);
    AlarmSnoozed++;
    TimeTransition();
  }
}

void AlarmNowTransition() {
  State = AlarmNowState;
  AlarmTriggered = true;
  BeepState = 0;
  NextBeepStateChange = milliseconds;
  GeneralTimer = milliseconds + (10 * 60 * 1000); // In alarm now mode, generalTimer is timer for turning off alarm. Turn off alarm in 10 minutes
  Display("ALARM", "00000", true);
}

void CheckAlarm() {
  byte hourTemp = hour();
  byte minuteTemp = minute();

  if (minuteTemp != 0)
    HourlySignalTriggered = false;

  if (minuteTemp != AlarmMinute)
    AlarmTriggered = false;

  if (State != AlarmMenuState) {
    if (minuteTemp == 0 && !HourlySignalTriggered && bitRead(AlarmEnabled, 7)) {
      HourlySignalTriggered = true;
      a5tone(880, 200);
    }
    else if (!AlarmTriggered && minuteTemp == AlarmMinute && hourTemp == AlarmHour && bitRead(AlarmEnabled, (weekday() - 2 + 7) % 7)) {
      AlarmNowTransition();
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
    switch (editing) {
      case 0:
        option = changeOption(option, optionValue, 9);

        if (option == 0) {
          char temp_DP[5] = "00000";
          if (AlarmEnabled & 0b01111111) {
            temp_DP[0] = '2';
          }

          Display("ALARM", temp_DP, true);
        }
        else if (option == 8) {
          char temp_DP[5] = "00000";
          if (bitRead(AlarmEnabled, 7)) {
            temp_DP[0] = '2';
          }

          Display("SIGNL", temp_DP, true);
        }
        else {
          option--;

          char temp[5] = " ||| ";
          byte index = option * 3;
          temp[1] = DayNames[index];
          temp[2] = DayNames[index + 1];
          temp[3] = DayNames[index + 2];

          char temp_DP[5] = "00000";
          if (bitRead(AlarmEnabled, option)) {
            temp_DP[0] = '2';
          }

          option++;

          Display(temp, temp_DP, true);
        }
        break;
      case 1:
        if (option == 0) {
          if (optionValue == 255) {
            ChangeAlarm(-1);
          }
          else if (optionValue == 1) {
            ChangeAlarm(1);
          }

          char temp[5] = "hhmm ";

          temp[2] = ToCurrentBase(AlarmMinute, 1);
          temp[3] = ToCurrentBase(AlarmMinute, 0);
          if (Mode24H) {
            temp[0] = ToCurrentBase(AlarmHour, 1);
            temp[1] = ToCurrentBase(AlarmHour, 0);
          }
          else {
            byte AlarmHourTemp = AlarmHour % 12;
            if (AlarmHourTemp == 0) {
              AlarmHourTemp = 12;
            }

            temp[0] = ToCurrentBase(AlarmHourTemp, 1);
            temp[1] = ToCurrentBase(AlarmHourTemp, 0);
            if (AlarmHour >= 12) {
              temp[4] = 'P';
            }
            else {
              temp[4] = 'A';
            }
          }

          char temp_DP[5] = "01200";
          if (AlarmEnabled & 0b01111111) {
            temp_DP[0] = '2';
          }

          Display(temp, temp_DP, false);
        }
        else {
          bitWrite(AlarmEnabled, option - 1, !bitRead(AlarmEnabled, option - 1));
          editing = 0;
          updateDisplay = true;
        }
        break;
      case 2:
        editing = 1;
        updateDisplay = true;
        if (AlarmEnabled & 0b01111111) {
          AlarmEnabled &= 0b10000000; // Disable alarm for all days of week
        }
        else {
          AlarmEnabled |= 0b01111111; // Enable alarm for all days of week
        }
        break;
    }
  }
}

void AlarmMenuTransition() {
  State = AlarmMenuState;
  ChangeAlarm(-AlarmSnoozed * SnoozeMinutes);
  AlarmSnoozed = 0;
  MenuTransition();
}
