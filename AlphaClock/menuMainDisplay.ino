#define MainMenuItems 8

#define SoundMenuItem 0
#define FontMenuItem 1
#define Menu24HItem 2
#define NightMenuItem 3
#define LightMenuItem 4
#define BrightnessMenuItem 5
#define LedTestMenuItem 6
#define BaseMenuItem 7

#define NightSubMenuItems 2
#define NightStartSubMenuItem 0
#define NightEndSubMenuItem 1


#define LightSubMenuItems 2
#define LightDaySubMenuItem 0
#define LightNightSubMenuItem 1

#define BrightnessSubMenuItems 3
#define BrightnessUseAdjustmentSubMenuItem 0
#define BrightnessDaySubMenuItem 1
#define BrightnessNightSubMenuItem 2

#define Menu24HSubMenuItems 2
#define Enable24HModeSubMenuItem 0
#define Menu24HSpinnerSubMenuItem 1

void ShowMainMenu() {
  byte optionValue = Menu();

  if (updateDisplay) {
    updateDisplay = false;
    if (editing == 0) {
      option = changeOption(option, optionValue, MainMenuItems);
    }

    switch (option) {
      case FontMenuItem:
        switch (editing) {
          case 0:
            Display("FONT ", "00000", true);
            break;
          case 1:
            Font = changeOption(Font, optionValue, Fonts);
            a5loadAltNumbers(Font);
            Display("01237", "31111", false);
            break;
          case 2:
            editing = 1;
            updateDisplay = true;
            break;
        }
        break;
      case Menu24HItem:
        Hour24SubMenu(optionValue);
        break;
      case NightMenuItem:
        NightSubMenu(optionValue);
        break;
      case LightMenuItem:
        LightSubMenu(optionValue);
        break;
      case BrightnessMenuItem:
        BrightnessSubMenu(optionValue);
        break;
      case LedTestMenuItem:
        switch (editing) {
          case 0:
            Display("LED T", "00000", true);
            break;
          case 1:
            if (optionValue == 1) {
              brightness++;
              updateBrightness();
            }
            else if (optionValue == 255) {
              brightness--;
              updateBrightness();
            }

            a5editFontChar('b', 255, 255, 255);
            Display("bbbbb", "00000", false); // Light up all leds
            break;
          case 2:
            editing = 1;
            updateDisplay = true;
            break;
        }
        break;
      case BaseMenuItem:
        switch (editing) {
          case 0:
            Display("BASE ", "00000", true);
            break;
          case 1:
            Base = changeOptionMin(Base, optionValue, 8, 37/*0 to 9, A to Z*/);
            char temp[5] = "DEC__";
            temp[3] = Base / 10 + '0';
            temp[4] = Base % 10 + '0';
            Display(temp, "20100", false);
            break;
          case 2:
            editing = 1;
            updateDisplay = true;
            break;
        }
        break;
      case SoundMenuItem:
        switch (editing) {
          case 0:
            Display("SOUND", "00000", true);
            break;
          case 1:
            EnableSound = changeOption(EnableSound, optionValue, 2);
            DisplayBool(EnableSound, "20000", false);
            break;
          case 2:
            editing = 1;
            updateDisplay = true;
            break;
        }
        break;
    }
  }
}

void NightSubMenu(byte optionValue) {
  switch (editing) {
    case 0:
      Display("NIGHT", "00002", true);
      break;
    case 1:
      subOption = changeOption(subOption, optionValue, NightSubMenuItems);
      switch (subOption) {
        case NightStartSubMenuItem:
          Display("START", "00000", true);
          break;
        case NightEndSubMenuItem:
          Display(" END ", "00000", true);
          break;
      }
      break;
    case 2:
      switch (subOption) {
        case NightStartSubMenuItem:
          NightStart = changeOption(NightStart, optionValue, 24);
          DisplayHour(NightStart, "20000", false);
          break;
        case NightEndSubMenuItem:
          NightEnd = changeOption(NightEnd, optionValue, 24);
          DisplayHour(NightEnd, "20000", false);
          break;
      }
      break;
    case 3:
      editing = 2;
      updateDisplay = true;
      break;
  }
}

void LightSubMenu(byte optionValue) {
  switch (editing) {
    case 0:
      Display("LIGHT", "00002", true);
      break;
    case 1:
      subOption = changeOption(subOption, optionValue, LightSubMenuItems);
      switch (subOption) {
        case LightDaySubMenuItem:
          Display(" DAY ", "00000", true);
          break;
        case LightNightSubMenuItem:
          Display("NIGHT", "00000", true);
          break;
      }
      break;
    case 2:
      switch (subOption) {
        case LightDaySubMenuItem:
          LightDay = changeOption(LightDay, optionValue, NightLightMax + 1);
          if (optionValue != 0) {
            updateNightLight(night);
          }
          DisplayNumber(LightDay, "20000", false);
          break;
        case LightNightSubMenuItem:
          LightNight = changeOption(LightNight, optionValue, NightLightMax + 1);
          if (optionValue != 0) {
            updateNightLight(night);
          }
          DisplayNumber(LightNight, "20000", false);
          break;
      }
      break;
    case 3:
      editing = 2;
      updateDisplay = true;
      break;
  }
}

void BrightnessSubMenu(byte optionValue) {
  switch (editing) {
    case 0:
      Display("BRGHT", "00002", true);
      break;
    case 1:
      subOption = changeOption(subOption, optionValue, BrightnessSubMenuItems);
      switch (subOption) {
        case BrightnessUseAdjustmentSubMenuItem:
          Display(" DIM ", "00000", true);
          break;
        case BrightnessDaySubMenuItem:
          Display(" DAY ", "00000", true);
          break;
        case BrightnessNightSubMenuItem:
          Display("NIGHT", "00000", true);
          break;
      }
      break;
    case 2:
      switch (subOption) {
        case BrightnessUseAdjustmentSubMenuItem:
          UseBrightnessAdjustment = changeOption(UseBrightnessAdjustment, optionValue, 2);
          DisplayBool(UseBrightnessAdjustment, "20000", false);
          break;
        case BrightnessDaySubMenuItem:
          DayBrightness = changeOptionMin(DayBrightness, optionValue, 0, BrightnessLevels);
          DisplayNumber(DayBrightness, "20000", false);
          break;
        case BrightnessNightSubMenuItem:
          NightBrightness = changeOptionMin(NightBrightness, optionValue, 0, BrightnessLevels);
          DisplayNumber(NightBrightness, "20000", false);
          break;
      }
    case 3:
      editing = 2;
      break;
  }
}

void Hour24SubMenu(byte optionValue) {
  if (editing == 0) {
    char temp[5] = " __H ";
    temp[1] = ToCurrentBase(24, 1);
    temp[2] = ToCurrentBase(24, 0);
    Display(temp, "00002", true);
  }
  else if (editing == 1) {
    subOption = changeOption(subOption, optionValue, Menu24HSubMenuItems);
    if (Enable24HModeSubMenuItem == subOption) {
      char temp2[5] = "__H M";

      temp2[0] = ToCurrentBase(24, 1);
      temp2[1] = ToCurrentBase(24, 0);
      Display(temp2, "00000", true);
    }
    else if (Menu24HSpinnerSubMenuItem == subOption) {
      Display("    _", "00000", true);
    }
  }
  else if (editing == 2) {
    if (subOption == Enable24HModeSubMenuItem) {
      Mode24H = changeOption(Mode24H, optionValue, 2);
      DisplayBool(Mode24H, "20000", false);
    }
    else if (Menu24HSpinnerSubMenuItem == subOption) {
      UseSpinner = changeOption(UseSpinner, optionValue, 2);
      if (UseSpinner) {
        Display("SPINR", "20000", false);
      }
      else {
        Display(" SEC ", "20000", false);
      }
    }
  }
  else if (editing == 3) {
    editing = 2;
    updateDisplay = true;
  }
}
