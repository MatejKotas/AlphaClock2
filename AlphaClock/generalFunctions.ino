// Screen

void Display(char *wordToPrint, char *decimals, bool fade)
{
  a5clearOSB(); // Loading OSB is additive, must clear buffer before writing to it.

  a5loadOSB_Ascii(wordToPrint, BrightnessLevel[brightness]);
  a5loadOSB_DP(decimals, BrightnessLevel[brightness]);

  if (fade)
  {
    Fading = true;

    a5BeginFadeToOSB();
  }
  else
  {
    a5loadVidBuf_fromOSB();
  }
}

void Fade() {
  if (Fading && FadeUpdateTimer - milliseconds > MillisHalfOverflow)
  {
    FadeUpdateTimer = milliseconds + 10;

    a5LoadNextFadeStage();
    a5loadVidBuf_fromOSB();

    if (a5_FadeStage == -1)
    {
      Fading = false;
    }
  }
}

int absoluteValue(int number) {
  if (number < 0) {
    number *= -1;
  }
  return number;
}

void DisplayBool(bool on, char *printDP, bool fade) {
  if (on) {
    Display(" ON  ", printDP, fade);
  }
  else {
    Display(" OFF ", printDP, fade);
  }
}

void DisplayNumber(int num, char* printDP, bool fade) {
  char string[5] = "     ";
  
  for (int i = 4; i >= 0; i--) {
    uint8_t c = num % Base;
    num /= Base;
    if (c >= 10) {
      string[i] = c + '7'; // same as num += 'A' - 10
    }
    else {
      string[i] = c + '0';
    }
  }
  for (int i = 0; i < 4; i++) {
    if (string[i] == '0') {
      string[i] = ' ';
    }
    else {
      break;
    }
  }

  Display(string, printDP, fade);
}

void DisplayHour(int h, char* printDP, bool fade) {
  char temp[5] = "     ";

  if (Mode24H) {
    temp[1] = ToCurrentBase(h, 1);
    temp[2] = ToCurrentBase(h, 0);

    if (temp[1] == '0') {
      temp[1] = ' ';
    }

  }
  else {
    byte mod12 = h % 12;
    if (mod12 == 0) {
      mod12 = 12; // 0 AM or 0 PM dosn't make sense
    }

    temp[0] = ToCurrentBase(mod12, 1);
    temp[1] = ToCurrentBase(mod12, 0);

    temp[4] = 'M';
    if (h >= 12) {
      temp[3] = 'P';
    }
    else {
      temp[3] = 'A';
    }
    if (temp[0] == '0') {
      temp[0] = ' ';
    }
  }

  Display(temp, printDP, fade);
}

bool isNight() {
  byte h = hour();
  return (h >= NightStart || h < NightEnd);
}

void updateNightLight(bool nightTemp) {
  byte level;
  if (nightTemp) {
    level = LightNight;
  }
  else {
    level = LightDay;
  }
  if (level == 0) {
    a5nightLight(0);
  }
  else if (level == 1) {
    a5nightLight(1);
  }
  else {
    level = (int)NightLightMultiplier * (int)level;
    level = level * level / 255;

    a5nightLight(level);
  }
}

void updateBrightness() {
  if (brightness == 0) {
    brightness = BrightnessLevels - 1;;
  }
  else if (brightness == BrightnessLevels) {
    brightness = 1;
  }
  a5_brightLevel = BrightnessLevel[brightness];
  a5_brightMode = BrightnessMode[brightness];

  Display("     ", "00000", false); // Prevent bright flash that sometimes occurs

  updateDisplay = true;
}

int changeOption(int optionLocal, int optionValue, int totalOptions) {
  return changeOptionMin(optionLocal, optionValue, 0, totalOptions);
}

int changeOptionMin(int optionLocal, int optionValue, int minOption, int totalOptions) {
  if (optionValue == 1) {
    if (optionLocal < totalOptions - 1) {
      optionLocal++;
    }
    else {
      optionLocal = minOption;
    }
  }
  else if (optionValue != 0) {
    if (optionLocal > minOption) {
      optionLocal--;
    }
    else {
      optionLocal = totalOptions - 1;
    }
  }

  return optionLocal;
}

char ToCurrentBase(int num, byte placeValue) {
  if (placeValue) { // second from right
    num /= Base;
  }
  else {
    num %= Base;
  }

  if (num >= 10) {
    num += '7'; // same as num += 'A' - 10
  }
  else {
    num += '0';
  }

  return num;
}
