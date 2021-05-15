byte option;
byte subOption;
byte editing;

void MenuTransition() {
  option = 0;
  subOption = 0;
  editing = 0;
  updateDisplay = true;

  EEPROMUpdatePending = false;
}

byte Menu() {
  if (GetButton(AlarmSetButton)) {
    editing--;
    updateDisplay = true;
  }
  if (GetButtonPair(MinusButton, PlusButton) || editing == 255) {
    TimeTransition();
    startEEPROMSaveTimer();
  }

  if (GetButton(TimeSetButton)) {
    editing++;
    updateDisplay = true;
  }

  byte optionValue = 0;

  if (GetHoldButton(PlusButton)) {
    optionValue++;
    updateDisplay = true;
  }
  if (GetHoldButton(MinusButton)) {
    optionValue--;
    updateDisplay = true;
  }

  return optionValue;
}
