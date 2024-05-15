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

  retractToHome();
  stepper1.setCurrentPosition(0);

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Retracted, Set Position: 0"));
#endif

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Calibration Complete"));
  Serial.println(F("+++"));
  Serial.flush();
#endif
}


// retract until endStop is hit
void retractToHome() {
  stepper1.setSpeed(-calibrationVel);

  while (!isEndStopHit(homeES)) {
    while (isEmergencyEndStopHit())
      ;
    stepper1.runSpeed();
  }
}


// back off endStop after it is hit.
void backOffHome() {
  stepper1.setSpeed(calibrationVel);

  while (isEndStopHit(homeES)) {
    while (isEmergencyEndStopHit())
      ;
    stepper1.runSpeed();
  }
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


// extend until endStop is hit
void extendToEndStops() {
  stepper1.setSpeed(calibrationVel);

  while (!isEndStopHit(extendedES)) {
    if (!isEndStopHit(extendedES)) stepper1.runSpeed();
  }
}


// back off endStop after it is hit.
void backOffExtendedEndStops() {
  stepper1.setSpeed(-calibrationVel);

  while (isEndStopHit(extendedES)) {
    if (isEndStopHit(extendedES)) stepper1.runSpeed();
  }
}
