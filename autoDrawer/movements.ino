// move to a position based on a percentage of range
void moveTo(int dest) {
  if (dest <= 0) dest = 0;
  if (dest > 100) dest = 100;
  long destInSteps = (long)(maxDistanceInSteps * (dest / 100.0));
  if (destInSteps > maxDistanceInSteps) destInSteps = maxDistanceInSteps;
  stepper1.moveTo(destInSteps);
}


// Calibration - Fully retract, and set position to 0.
void calibrate() {
#ifdef DEBUG_CALIBRATE
  Serial.println(F("Calibrate, retract..."));
#endif

  retractToEndStops();

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Retracted, Back Off..."));
#endif

  backOffRetractedEndStops();
  stepper1.setCurrentPosition(0);

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Set Position: 0"));
#endif

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Calibration Complete"));
  Serial.println(F("+++"));
  Serial.flush();
#endif
}


// move to and report position at full extension 
void moveToAndReportFullExtension() {
  extendToEndStops();

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Extended, Back Off..."));
#endif

  backOffExtendedEndStops();

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Extended Position: "));
  Serial.println(stepper1.currentPosition());
#endif
}


// retract until endStop is hit
void retractToEndStops() {
  stepper1.setSpeed(-calibrationVel);

  while (!isEndStopHit(retractedES1)) {
    if (!isEndStopHit(retractedES1)) stepper1.runSpeed();
  }
}


// back off endStop after it is hit.
void backOffRetractedEndStops() {
  stepper1.setSpeed(calibrationVel);

  while (isEndStopHit(retractedES1)) {
    if (isEndStopHit(retractedES1)) stepper1.runSpeed();
  }
}


// extend until endStop is hit
void extendToEndStops() {
  stepper1.setSpeed(calibrationVel);

  while (!isEndStopHit(extendedES1)) {
    if (!isEndStopHit(extendedES1)) stepper1.runSpeed();
  }
}


// back off endStop after it is hit.
void backOffExtendedEndStops() {
  stepper1.setSpeed(-calibrationVel);

  while (isEndStopHit(extendedES1)) {
    if (isEndStopHit(extendedES1)) stepper1.runSpeed();
  }
}
