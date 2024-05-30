// move to a position based on a percentage of range
void moveTo(int dest) {
  if (dest <= 0) dest = 0;
  if (dest > 100) dest = 100;
  long destInSteps = (long)(maxDistanceInSteps * (dest / 100.0));
  if (destInSteps > maxDistanceInSteps) destInSteps = maxDistanceInSteps; 
  stepper.setAcceleration(defaultAccel);
  stepper.setSpeed(defaultVel);
  stepper.moveTo(destInSteps);
}

// stop the stepper as fast as possible
void quickStop() {
  stepper.setAcceleration(10000);
  stepper.setSpeed(0);
  stepper.stop();  
  stepper.runSpeedToPosition();
  stepper.moveTo(stepper.currentPosition());
}


// Calibration - Fully retract, and set position to 0.
void calibrate() {
#ifdef DEBUG_CALIBRATE
  Serial.println(F("Calibrate, retract..."));
#endif

  retractToHome();
  stepper.setCurrentPosition(0);

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Retracted, Set Position: 0"));
#endif

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Calibration Complete"));
  Serial.println(F("+++"));
  Serial.flush();
#endif
}


// retract until home endStop is hit
void retractToHome() {
  stepper.setSpeed(-calibrationVel);

  while (!isTriggerHit(homeES)) {
    while (isEmergencyEndStopHit())
      ;
    stepper.runSpeed();
  }
}


// back off home endStop after it is hit.
void backOffHome() {
  stepper.setSpeed(calibrationVel);

  while (isTriggerHit(homeES)) {
    while (isEmergencyEndStopHit())
      ;
    stepper.runSpeed();
  }
}


// move to and report position at full extension
void moveToAndReportFullExtension() {
  extendToEndStop();

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Extended, Back Off..."));
#endif

  backOffExtendedEndStop();

#ifdef DEBUG_CALIBRATE
  Serial.println(F("Extended Position: "));
  Serial.println(stepper.currentPosition());
#endif
}


// extend until endStop is hit
void extendToEndStop() {
  stepper.setSpeed(calibrationVel);

  while (!isTriggerHit(extendedES)) {
    if (!isTriggerHit(extendedES)) stepper.runSpeed();
  }
}


// back off endStop after it is hit.
void backOffExtendedEndStop() {
  stepper.setSpeed(-calibrationVel);

  while (isTriggerHit(extendedES)) {
    if (isTriggerHit(extendedES)) stepper.runSpeed();
  }
}
