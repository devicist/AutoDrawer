/*
 *
 * Controls the motion of a stepper motor controlled linear rail
 * with end stops attached on each end. 
 * On startup the motor will calibrate, fully retracting the gantry plate 
 * to the retracted end stop. Then it will wait for a user command over Serial.
 * 
 * USAGE
 * 
 * Wait until calibration is complete then:
 * Send 1 to fully extend the plate. Send 0 to fully retract the plate. 
 * Send values as ASCII characters. 
 * CR and NL are ignored.
 * Errant values are ignored.
 * 
 * Tested on an Arduino Uno.
 *
 * Linear Rail: https://vi.aliexpress.com/item/33049810658.html?spm=a2g0o.order_list.order_list_main.120.498a1802Y4LbKi&gatewayAdapt=glo2vnm
 * Stepper Driver: https://fuselab.gr/product/btt-tmc2160-stepper-motor-driver/
 * End Stops: https://www.amazon.ca/gp/product/B06XTB7WMK/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1
 * 
 * contact: Nicholas Stedman - nick@devicist.com
 */



#include <AccelStepper.h>
#include <movingAvg.h>

// uncomment to print out various debug messages to console
// #define DEBUG
// #define DEBUG_CALIBRATE
// #define DEBUG_ENDSTOP

// define as needed
const long maxDistanceInSteps = 33000;
const float maxVel = 4000;
const float defaultVel = 3900;
const float calibrationVel = 1500;
const float defaultAccel = 500;

// hardware
const int enablePin1 = 6;            // Enable
const int enableVoltagePin1 = 5;     // Enable Voltage
const int directionPin1 = 8;         // Direction
const int directionVoltagePin1 = 7;  // Direction Voltage
const int stepPin1 = 10;             // Step
const int stepVoltagePin1 = 9;       // Step Voltage

const int retractedEndStopPin1 = 12;
const int extendedEndStopPin1 = 11;

AccelStepper stepper1 = AccelStepper(stepper1.DRIVER, stepPin1, directionPin1);


void setup() {
  Serial.begin(9600);
#ifdef DEBUG
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.println(F("Begin Setup"));
  Serial.println(F("+++"));
  Serial.flush();
#endif

  // set the pins
  initStepperPins();
  initEndStops();

  delay(3000);  // allow driver to power up

  // setup stepper drivers
  setupSteppers();
  enableSteppers();

  // find retracted position
  calibrate();

  // uncomment to discover the maximum steps steppers can travel
  // delay(1000);
  // moveToAndReportFullExtension();
  // delay(1000);

#ifdef DEBUG
  Serial.println(F("Setup Complete"));
  Serial.println();
  Serial.flush();
#endif
}


void loop() {
  long speed = defaultVel;
  int accel = defaultAccel;

  int drawerControlVal = checkForCommand();

  if (drawerControlVal == 0) {
    moveTo(0);
  } else if (drawerControlVal == 1) {
    moveTo(100);
  }

  // step the steppers if permitted
  if (isEitherEndStopHit() == false) {
    run(speed, accel);
  }
}


int checkForCommand() {
  static int controlVal = 0;

  if (Serial.available()) {
    String inputString = Serial.readString();
    inputString.trim();
#ifdef DEBUG
    Serial.print(inputString + " - ");
#endif
    if (inputString.equals("0")) {
      controlVal = 0;
#ifdef DEBUG
      Serial.println("close");
#endif
    } else if (inputString.equals("1")) {
      controlVal = 1;
#ifdef DEBUG
      Serial.println("open");
#endif
    } else {
#ifdef DEBUG
      Serial.println("invalid input");
#endif
    }
  }
  return controlVal;
}