#define AlarmSetButton 0
#define TimeSetButton 1
#define PlusButton 2
#define MinusButton 3

#define Buttons 4

#define TwoButtonThreshhold 500
#define debounceTime 100

#define ButtonUpdateInterval 5

//Hold parameters
#define ButtonStartHold 500
#define ButtonHoldTriggerRate 15

unsigned long ButtonReadyTimes[Buttons];
unsigned long ButtonStartTimes[Buttons];

byte ButtonsState;
byte ButtonsTriggered;

void ManageButtons() {
  byte ButtonsStateLast = ButtonsState;
  ButtonsState = a5GetButtons();

  for (int i = 0; i < Buttons; i++) {
    if ((long)(ButtonReadyTimes[i] - milliseconds) < 0) {
      if (!bitRead(ButtonsState, i)) {
        if (bitRead(ButtonsTriggered, i)) {
          bitClear(ButtonsTriggered, i);
          ButtonReadyTimes[i] = milliseconds + debounceTime;
        }
      }
      else if (!bitRead(ButtonsStateLast, i) && bitRead(ButtonsState, i)) {
        ButtonStartTimes[i] = milliseconds;
      }
    }
  }
}

bool GetButton(byte button) {
  if ((long)(ButtonReadyTimes[button] - milliseconds) < 0) {
    if (bitRead(ButtonsState, button) && !bitRead(ButtonsTriggered, button)) {
      bitSet(ButtonsTriggered, button);
      ButtonReadyTimes[button] = milliseconds + debounceTime;
      MakeSound();
      return true;
    }
  }
  return false;
}

bool GetButtonPair(byte button1, byte button2) {
  if ((long)(ButtonReadyTimes[button1] - milliseconds) < 0 || (long)(ButtonReadyTimes[button2] - milliseconds) < 0) {
    if (bitRead(ButtonsState, button1) && bitRead(ButtonsState, button2) && !(bitRead(ButtonsTriggered, button1) && bitRead(ButtonsTriggered, button2)) && (absoluteValue(ButtonStartTimes[button1] - ButtonStartTimes[button2]) <= TwoButtonThreshhold)) {
      if (!bitRead(ButtonsTriggered, button1)) {
        bitSet(ButtonsTriggered, button1);
        ButtonReadyTimes[button1] = milliseconds + debounceTime;
      }
      if (!bitRead(ButtonsTriggered, button2)) {
        bitSet(ButtonsTriggered, button2);
        ButtonReadyTimes[button2] = milliseconds + debounceTime;
      }
      MakeSound();
      return true;
    }
  }
  return false;
}

bool GetHoldButton(byte button) {
  if ((long)(ButtonReadyTimes[button] - milliseconds) < 0) {
    if ((long)(ButtonStartTimes[button] + ButtonStartHold - milliseconds) < 0 && bitRead(ButtonsState, button)) {
      ButtonStartTimes[button] += ButtonHoldTriggerRate; // Set up start time so we get here in ButtonHoldTriggerRate milliseconds
      return true;
    }
    else if (bitRead(ButtonsState, button) && !bitRead(ButtonsTriggered, button)) { // Copied from GetButton()
      bitSet(ButtonsTriggered, button);
      ButtonReadyTimes[button] = milliseconds + debounceTime;
      MakeSound();
      return true;
    }
  }
  return false;
}

#define OneTriggerHoldTime 300

bool GetHoldButton_oneTrigger(byte button) {
  if ((long)(ButtonReadyTimes[button] - milliseconds) < 0 && (long)(ButtonStartTimes[button] + OneTriggerHoldTime - milliseconds) < 0 && bitRead(ButtonsState, button)) {
    MakeSound();
    return true;
  }
  return false; // No need for else statement
}

void MakeSound() {
  if (EnableSound) {
    a5tone(SoundFreqency, SoundDuration);
  }
}
