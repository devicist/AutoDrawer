// use a moving average to mitigate false switch readings
const int numToAverage = 15;

movingAvg retractedVal1(numToAverage);
movingAvg extendedVal1(numToAverage);

enum endStopNames {
  retractedES1,
  extendedES1,
  numEndStops
};

void initEndStops() {
  pinMode(extendedEndStopPin1, INPUT_PULLUP);
  pinMode(retractedEndStopPin1, INPUT_PULLUP);

  retractedVal1.begin();
  extendedVal1.begin();

  updateEndStopReading();
};

// initialize the endstop values
void updateEndStopReading() {
  for (int i = 0; i < numToAverage; i++) {
    retractedVal1.reading(!digitalRead(retractedEndStopPin1));
    extendedVal1.reading(!digitalRead(extendedEndStopPin1));
  }
  return;
};

// update and get endStop value
int isEndStopHit(int endStop) {
  switch (endStop) {
    case retractedES1: return retractedVal1.reading(!digitalRead(retractedEndStopPin1));
    case extendedES1: return extendedVal1.reading(!digitalRead(extendedEndStopPin1));
  }
}


// check all endStops at once
bool isEitherEndStopHit() {
  bool isHit = false;

  if (isEndStopHit(retractedES1)) {
    isHit = true;
#ifdef DEBUG_ENDSTOP
    Serial.println(F("Retracted End Stop 1 Hit"));
#endif
  }

  if (isEndStopHit(extendedES1)) {
    isHit = true;
#ifdef DEBUG_ENDSTOP
    Serial.println(F("Extended End Stop 1 Hit"));
#endif
  }

  return isHit;
}
