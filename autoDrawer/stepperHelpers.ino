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


// report state of motor: moving, open, or closed
int checkDrawerState() {
  int drawerState = 2;
  int pos = stepper.currentPosition();
  if(pos == maxDistanceInSteps){
    drawerState = 1;
  }
  else if(pos == 0) {
    drawerState = 0;
  }
  return drawerState;
}


// NOT USED - report if motor is moving or not.
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
