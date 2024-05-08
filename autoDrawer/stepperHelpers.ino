void initStepperPins() {
  pinMode(enableVoltagePin1, OUTPUT);
  digitalWrite(enableVoltagePin1, HIGH);
  pinMode(stepVoltagePin1, OUTPUT);
  digitalWrite(stepVoltagePin1, HIGH);
  pinMode(directionVoltagePin1, OUTPUT);
  digitalWrite(directionVoltagePin1, HIGH);  
}


void setupSteppers() {
  stepper1.setPinsInverted(false, false, true);
  stepper1.setMaxSpeed(maxVel);
  stepper1.setAcceleration(defaultAccel);
  stepper1.setEnablePin(enablePin1);
}


// step the steppers
void run(int stepSpeed, int accel) {
  stepper1.run();
}


void enableSteppers() {
  stepper1.enableOutputs();
}


void disableSteppers() {
  stepper1.disableOutputs();
}
