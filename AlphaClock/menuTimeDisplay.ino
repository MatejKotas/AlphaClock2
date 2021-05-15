#define TimeMenuItems 2

#define TimeMenuItemTime 0
#define TimeMenuItemDate 1

#define TimeTimeMenuItems 3
#define HourSubMenuItem 0
#define MinuteSubMenuItem 1
#define SecondSubMenuItem 2

#define TimeDateMenuItems 3
#define MonthMenuItem 0
#define DayMenuItem 1
#define YearMenuItem 2

void ShowTimeMenu() {
  if (GetHoldButton_oneTrigger(TimeSetButton)) {
    ShowDateTransition();
    return;
  }
  byte optionValue = Menu();

  if (State != TimeMenuState) {
    RTC.set(now());
    return;
  }

  byte SecondTemp2 = second();

  if (updateDisplay || SecondTemp != SecondTemp2) {
    updateDisplay = false;
    SecondTemp = SecondTemp2;

    if (editing == 0) {
      option = changeOption(option, optionValue, TimeMenuItems);
    }
    switch (option) {
      case TimeMenuItemTime:
        TimeTimeMenu(optionValue);
        break;
      case TimeMenuItemDate:
        DateMenu(optionValue);
        break;
    }
  }
}

void TimeTimeMenu(byte optionValue) {
  switch (editing) {
    case 0:
      Display("TIME ", "00002", true);
      break;
    case 1:
      subOption = changeOption(subOption, optionValue, TimeTimeMenuItems);
      switch (subOption) {
        case HourSubMenuItem:
          Display("HOUR ", "00000", true);
          break;
        case MinuteSubMenuItem:
          Display(" MIN ", "00000", true);
          break;
        case SecondSubMenuItem:
          Display(" SEC ", "00000", true);
          break;
      }
      break;
    case 3:
      editing = 2;
      break;
    case 2:
      long optionValueLong = optionValue;
      if (optionValue == 255) {
        optionValueLong = -1;
      }
      switch (subOption) {
        case HourSubMenuItem:
          adjustTime(optionValueLong * 3600);
          DisplayNumber(hour(), "20000", false);
          break;
        case MinuteSubMenuItem:
          adjustTime(optionValueLong * 60);
          DisplayNumber(minute(), "20000", false);
          break;
        case SecondSubMenuItem:
          adjustTime(optionValueLong);
          DisplayNumber(second(), "20000", false);
          break;
      }
      break;
  }
}

void DateMenu(byte optionValue) {
  if (editing == 0) {
    Display("DATE ", "00002", true);
  }
  else if (editing == 1) {
    subOption = changeOption(subOption, optionValue, TimeDateMenuItems);
    switch (subOption) {
      case MonthMenuItem:
        Display("MONTH", "00000", true);
        break;
      case DayMenuItem:
        Display(" DAY ", "00000", true);
        break;
      case YearMenuItem:
        Display("YEAR ", "00000", true);
        break;
    }
  }
  else if (editing == 2) {
    long optionValueLong = optionValue;
    if (optionValue == 255) {
      optionValueLong = -1;
    }

    switch (subOption) {
      case MonthMenuItem:
        ShowMonth(optionValueLong);
        break;

      case DayMenuItem:
        setTime(hour(), minute(), second(), day() + optionValueLong, month(), year());
        DisplayNumber(day(), "20000", false);
        break;

      case YearMenuItem:
        setTime(hour(), minute(), second(), day(), month(), year() + optionValueLong);
        DisplayNumber(year(), "20000", false);
        break;
    }
  }
  else if (editing == 3) {
    editing = 2;
  }
}

void ShowMonth(long optionValueLong) { // Must do because of compiler bug
  setTime(hour(), minute(), second(), day(), month() + optionValueLong, year());
  char temp[5] = "     ";

  byte monthTemp = (month() - 1) * 3;
  temp[1] = MonthNames[monthTemp];
  temp[2] = MonthNames[monthTemp + 1];
  temp[3] = MonthNames[monthTemp + 2];

  Display(temp, "20000", false);
}
