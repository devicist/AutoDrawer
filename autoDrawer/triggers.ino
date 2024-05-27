// use a moving average to mitigate false switch readings
const int numToAverage = 15;

movingAvg overrideVal(numToAverage);
movingAvg homeVal(numToAverage);
movingAvg retractedVal(numToAverage);
movingAvg extendedVal(numToAverage);


void initTriggers() {
  pinMode(overridePin, INPUT_PULLUP);
  pinMode(homePin, INPUT_PULLUP);
  pinMode(extendedEndStopPin, INPUT_PULLUP);
  pinMode(retractedEndStopPin, INPUT_PULLUP);

  overrideVal.begin();
  homeVal.begin();
  retractedVal.begin();
  extendedVal.begin();

  initTriggerReadings();
};


// initialize the trigger values
void initTriggerReadings() {
  for (int i = 0; i < numToAverage; i++) {
    overrideVal.reading(!digitalRead(overridePin));
    homeVal.reading(!digitalRead(homePin));
    retractedVal.reading(!digitalRead(retractedEndStopPin));
    extendedVal.reading(!digitalRead(extendedEndStopPin));
  }
  return;
};


// update and get individual trigger value
int isTriggerHit(int trigger) {
  switch (trigger) {
    case overrideButton: return overrideVal.reading(!digitalRead(overridePin));
    case homeES: return homeVal.reading(!digitalRead(homePin));
    case retractedES: return retractedVal.reading(!digitalRead(retractedEndStopPin));
    case extendedES: return extendedVal.reading(!digitalRead(extendedEndStopPin));
  }
}


// check all emergency endStops at once
bool isEmergencyEndStopHit() {
  bool isHit = false;

  if (isTriggerHit(retractedES)) {
    isHit = true;
#ifdef DEBUG_ENDSTOP
    Serial.println(F("Retracted End Stop Hit"));
#endif
  }

  if (isTriggerHit(extendedES)) {
    isHit = true;
#ifdef DEBUG_ENDSTOP
    Serial.println(F("Extended End Stop Hit"));
#endif
  }

  return isHit;
}
