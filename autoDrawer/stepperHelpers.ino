void initStepperPins() {
  pinMode(enableVoltagePin1, OUTPUT);
  digitalWrite(enableVoltagePin1, HIGH);
  pinMode(stepVoltagePin1, OUTPUT);
  digitalWrite(stepVoltagePin1, HIGH);
  pinMode(directionVoltagePin1, OUTPUT);
  digitalWrite(directionVoltagePin1, HIGH);
}


void setupSteppers() {
  stepper.setPinsInverted(false, false, true);
  stepper.setMaxSpeed(maxVel);
  stepper.setAcceleration(defaultAccel);
  stepper.setEnablePin(enablePin1);
}


int isMoving() {
  static long lastPosition;
  long position = stepper.currentPosition();
  int isStepperMoving = true;

#ifdef DEBUG
  Serial.print(position);
  Serial.print(" : ");
  Serial.print(lastPosition);
  Serial.print(", ");
#endif

  if (position == lastPosition) {
    isStepperMoving = false;
  }

  lastPosition = position;
  return isStepperMoving;
}


// step the steppers
void run(int stepSpeed, int accel) {
  stepper.run();
}


void enableSteppers() {
  stepper.enableOutputs();
}


void disableSteppers() {
  stepper.disableOutputs();
}
