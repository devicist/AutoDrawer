// use a moving average to mitigate false switch readings
const int numToAverage = 15;

movingAvg homeVal1(numToAverage);
movingAvg retractedVal1(numToAverage);
movingAvg extendedVal1(numToAverage);

enum endStopNames {
  homeES,
  retractedES,
  extendedES,
  numEndStops
};

void initEndStops() {
  pinMode(homePin, INPUT_PULLUP);  
  pinMode(extendedEndStopPin, INPUT_PULLUP);
  pinMode(retractedEndStopPin, INPUT_PULLUP);

  homeVal1.begin();
  retractedVal1.begin();
  extendedVal1.begin();

  updateEndStopReading();
};

// initialize the endstop values
void updateEndStopReading() {
  for (int i = 0; i < numToAverage; i++) {
    homeVal1.reading(!digitalRead(homePin));
    retractedVal1.reading(!digitalRead(retractedEndStopPin));
    extendedVal1.reading(!digitalRead(extendedEndStopPin));
  }
  return;
};

// update and get endStop value
int isEndStopHit(int endStop) {
  switch (endStop) {
    case homeES: return homeVal1.reading(!digitalRead(homePin));
    case retractedES: return retractedVal1.reading(!digitalRead(retractedEndStopPin));
    case extendedES: return extendedVal1.reading(!digitalRead(extendedEndStopPin));
  }
}


// check all endStops at once
bool isEmergencyEndStopHit() {
  bool isHit = false;

  if (isEndStopHit(retractedES)) {
    isHit = true;
#ifdef DEBUG_ENDSTOP
    Serial.println(F("Retracted End Stop Hit"));
#endif
  }

  if (isEndStopHit(extendedES)) {
    isHit = true;
#ifdef DEBUG_ENDSTOP
    Serial.println(F("Extended End Stop Hit"));
#endif
  }

  return isHit;
}
