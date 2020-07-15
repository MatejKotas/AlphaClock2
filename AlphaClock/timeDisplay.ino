const char MonthNames[36] = "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC";

void ShowTime() {
  byte SecondTemp2 = second();

  if (SecondTemp != SecondTemp2 || updateDisplay) {
    SecondTemp = SecondTemp2;
    updateDisplay = false;

    byte HourTemp = hour();
    byte MinuteTemp = minute();

    char temp[5] = "     ";
    byte spinnerChar;

    if (Mode24H) {
      temp[0] = ToCurrentBase(HourTemp, 1);
      temp[1] = ToCurrentBase(HourTemp, 0);
    }
    else {
      byte mod12 = HourTemp % 12;
      temp[0] = ToCurrentBase(mod12, 1);
      temp[1] = ToCurrentBase(mod12, 0);
    }
    temp[2] = ToCurrentBase(MinuteTemp, 1);
    temp[3] = ToCurrentBase(MinuteTemp, 0);

    char tempDP[5] = "01200";

    if (Mode24H) {
      if (!UseSpinner) {
        temp[4] = ToCurrentBase(SecondTemp, 1);
        tempDP[3] = '1';
        tempDP[4] = '2';
      }
      else {
        temp[4] = 'a';
        if (SecondTemp <= 7) {
          a5editFontChar('a',0,0,32); // up
        }
        else if (SecondTemp <= 15) {
          a5editFontChar('a',0,0,64); // up-right
        }
        else if (SecondTemp <= 22) {
          a5editFontChar('a',32,0,0); // right
        }
        else if (SecondTemp <= 30) {
          a5editFontChar('a',0,0,128); // down-right
        }
        else if (SecondTemp <= 37) {
          a5editFontChar('a',0,1,0); // down
        }
        else if (SecondTemp <= 45) {
          a5editFontChar('a',0,2,0); // down-left
        }
        else if (SecondTemp <= 52) {
          a5editFontChar('a',16,0,0); // left
        }
        else {
          a5editFontChar('a',0,0,16); // up-left
        }
      }
    }
    else {
      if (HourTemp >= 12) {
        temp[4] = 'P';
      }
      else {
        temp[4] = 'A';
      }
    }
    
    if (AlarmsEnabled > 0) {
      tempDP[0] = '2';
    }

    Display(temp, tempDP, true);
  }

  if (GetButtonPair(MinusButton, PlusButton)) {
    State = MenuState;
    MenuTransition();
  }
  else if (GetButton(TimeSetButton)) {
    setTime(RTC.get());
    State = TimeMenuState;
    MenuTransition();
    option = TimeMenuItemDate;
  }
  else if (GetButton(AlarmSetButton)) {
    AlarmMenuTransition();
  }
  else if (GetButton(PlusButton)) {
    brightness++;
    updateBrightness();
  }
  else if (GetButton(MinusButton)) {
    brightness--;
    updateBrightness();
  }
}

void TimeTransition() {
  SecondTemp = 255;
  State = TimeState;
}

void ShowDate() {
  if (updateDisplay) {
    updateDisplay = false;
    char temp[5] = "     ";
    char tempDP[5] = "00100";

    byte timeTemp = (month() - 1) * 3;
    temp[0] = MonthNames[timeTemp];
    temp[1] = MonthNames[timeTemp + 1];
    temp[2] = MonthNames[timeTemp + 2];

    timeTemp = day();
    temp[3] = ToCurrentBase(timeTemp, 1);
    temp[4] = ToCurrentBase(timeTemp, 0);

    Display(temp, tempDP, true);
  }

  if (!bitRead(ButtonsState, TimeSetButton))  {
    TimeTransition();
  }
}

void ShowDateTransition() {
  State = ShowDateState;
  updateDisplay = true;
}
