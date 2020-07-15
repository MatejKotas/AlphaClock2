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

  byte alphaButton = 1;
  for (int i = 0; i < Buttons; i++) {
    if (milliseconds >= ButtonReadyTimes[i]) {
      if (!(ButtonsState & alphaButton)) {
        if (ButtonsTriggered & alphaButton) {
          ButtonsTriggered ^= alphaButton;
          ButtonReadyTimes[i] = milliseconds + debounceTime;
        }
      }
      else if ((!(ButtonsStateLast & alphaButton)) && (ButtonsState & alphaButton)) {
        ButtonStartTimes[i] = milliseconds;
      }
    }
    alphaButton <<= 1;
  }
}

bool GetButton(byte button) {
  if (milliseconds >= ButtonReadyTimes[button]) {
    byte alphaButton = 1 << button;
    if ((ButtonsState & alphaButton) && !(ButtonsTriggered & alphaButton)) {
      ButtonsTriggered ^= alphaButton;
      ButtonReadyTimes[button] = milliseconds + debounceTime;
      return true;
    }
  }
  return false;
}

bool GetButtonPair(byte button1, byte button2) {
  if (milliseconds >= ButtonReadyTimes[button1] || milliseconds >= ButtonReadyTimes[button2]) {
    byte alphaButton1 = 1 << button1;
    byte alphaButton2 = 1 << button2;

    if ((ButtonsState & alphaButton1) && (ButtonsState & alphaButton2) && !((ButtonsTriggered & alphaButton1) && (ButtonsTriggered & alphaButton2)) && (absoluteValue(ButtonStartTimes[button1] - ButtonStartTimes[button2]) <= TwoButtonThreshhold)) {
      if (!(ButtonsTriggered & alphaButton1)) {
        ButtonsTriggered ^= alphaButton1;
        ButtonReadyTimes[button1] = milliseconds + debounceTime;
      }
      if (!(ButtonsTriggered & alphaButton2)) {
        ButtonsTriggered ^= alphaButton2;
        ButtonReadyTimes[button2] = milliseconds + debounceTime;
      }

      return true;
    }
  }
  return false;
}

bool GetHoldButton(byte button) {
  if (milliseconds >= ButtonReadyTimes[button]) {
    byte alphaButton = 1 << button;
    if (milliseconds >= ButtonStartTimes[button] + ButtonStartHold && (ButtonsState & alphaButton)) {
      ButtonStartTimes[button] += ButtonHoldTriggerRate; // Set up start time so we get here in ButtonHoldTriggerRate milliseconds
      return true;
    }
    else if ((ButtonsState & alphaButton) && !(ButtonsTriggered & alphaButton)) { // Copied from GetButton()
      ButtonsTriggered ^= alphaButton;
      ButtonReadyTimes[button] = milliseconds + debounceTime;
      return true;
    }
  }
  return false;
}

#define OneTriggerHoldTime 300

bool GetHoldButton_oneTrigger(byte button) {
  return (ButtonReadyTimes[button] <= milliseconds && ButtonStartTimes[button] + OneTriggerHoldTime <= milliseconds && bitRead(ButtonsState, button));
}
