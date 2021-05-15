#define EEPROMTimerDuration 30000 // 30 Seconds

void startEEPROMSaveTimer() {
  UpdateEEPROMTimer = milliseconds + EEPROMTimerDuration;
  EEPROMUpdatePending = true;
}

void writeEEPROM() {
  EEPROMUpdatePending = false;

  int j = 0;
  int j_bools = 0;
  byte booleans = 0;

  writeEEPROMSetting(j++, Font,                    true );
  bitWrite(booleans, j_bools++, UseBrightnessAdjustment);
  writeEEPROMSetting(j++, NightStart,              true );
  writeEEPROMSetting(j++, NightEnd,                true );
  writeEEPROMSetting(j++, NightBrightness,         true );
  writeEEPROMSetting(j++, DayBrightness,           true );
  bitWrite(booleans, j_bools++, Mode24H);
  writeEEPROMSetting(j++, LightDay,                true );
  writeEEPROMSetting(j++, LightNight,              true );
  writeEEPROMSetting(j++, AlarmEnabled,            true );
  ChangeAlarm(-AlarmSnoozed * SnoozeMinutes);
  writeEEPROMSetting(j++, AlarmMinute,             true );
  writeEEPROMSetting(j++, AlarmHour,               true );
  ChangeAlarm(-AlarmSnoozed * SnoozeMinutes);
  bitWrite(booleans, j_bools++, UseSpinner);
  writeEEPROMSetting(j++, Base,                    true);
  bitWrite(booleans, j_bools++, EnableSound);
  writeEEPROMSetting(j++, booleans,                true);
}

void readEEPROM() {
  int j = 0;
  Font =                    readEEPROMSetting(j++, Fonts - 1,     DefaultFont,                        true );
  NightStart =              readEEPROMSetting(j++, 23,            DefaultNightStart,                  true );
  NightEnd =                readEEPROMSetting(j++, 23,            DefaultNightEnd,                    true );
  NightBrightness =         readEEPROMSetting(j++, a5_MaxBright,  DefaultNightBrightness,             true );
  DayBrightness =           readEEPROMSetting(j++, a5_MaxBright,  DefaultDayBrightness,               true );
  LightDay =                readEEPROMSetting(j++, NightLightMax, DefaultLightDay,                    true );
  LightNight =              readEEPROMSetting(j++, NightLightMax, DefaultLightNight,                  true );
  AlarmEnabled =            readEEPROMSetting(j++, 63,            0,                                  true );
  AlarmMinute =             readEEPROMSetting(j++, 59,            0,                                  true );
  AlarmHour =               readEEPROMSetting(j++, 23,            0,                                  true );
  AlarmSnoozed = 0;
  Base          =           readEEPROMSetting(j++, 37,            DefaultBase,                        true );
  byte booleans =           readEEPROMSetting(j++, 15,            4,                                  true );

  j = 0;
  UseBrightnessAdjustment = bitRead(booleans, j++);
  Mode24H                 = bitRead(booleans, j++);
  UseSpinner              = bitRead(booleans, j++);
  EnableSound             = bitRead(booleans, j++);
}

void writeEEPROMSetting(int slot, byte value, bool subtract) {
  if (subtract) {
    value += 100;
  }

  for (int i = 0; i < 3; i++) {
    if (EEPROM.read(slot) != value) {
      a5writeEEPROM(slot, value);
    }
    else {
      return;
    }
  }
  Serial.println("EEPROM failed.");
}

byte readEEPROMSetting(int slot, int maximumValue, byte defaultValue, bool subtract) {
  byte value = EEPROM.read(slot);
  if ((value > maximumValue + (100 * subtract)) || (value < 100 && subtract)) {
    return defaultValue;
  }
  else {
    return (value - (100 * subtract));
  }
}
