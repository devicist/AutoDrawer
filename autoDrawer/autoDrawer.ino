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
 * The Arduino reports its current state:
 * 0 when fully closed
 * 1 when fully open
 * 2 when moving
 * The state it reported once, and is true until next reported state
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
const long maxDistanceInSteps = 23000;  // End to end is 34000 steps
const float maxVel = 4000;
const float defaultVel = 2000;
const float calibrationVel = 1500;
const float defaultAccel = 500;

// hardware
const int enablePin1 = 6;            // Enable
const int enableVoltagePin1 = 5;     // Enable Voltage
const int directionPin1 = 8;         // Direction
const int directionVoltagePin1 = 7;  // Direction Voltage
const int stepPin1 = 10;             // Step
const int stepVoltagePin1 = 9;       // Step Voltage

const int overridePin = 4;
const int homePin = 3;
const int extendedEndStopPin = 11;
const int retractedEndStopPin = 12;

enum triggerNames {
  overrideButton,
  homeES,
  retractedES,
  extendedES,
  numTriggers
};

AccelStepper stepper = AccelStepper(stepper.DRIVER, stepPin1, directionPin1);


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
  initTriggers();

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
  static int drawerControlVal;
  static int lastControlVal;
  static int lastDrawerState;

  drawerControlVal = checkForCommand(drawerControlVal);
  drawerControlVal = checkForManualOverride(drawerControlVal);

  if (drawerControlVal != lastControlVal) {
    if (stepper.isRunning() == true) {
      quickStop();
#ifdef DEBUG
      Serial.println("stopping");
#endif
    } else {
      if (drawerControlVal == 0) {
        moveTo(0);
#ifdef DEBUG
        Serial.println("closing");
#endif
      } else if (drawerControlVal == 1) {
        moveTo(100);
#ifdef DEBUG
        Serial.println("opening");
#endif
      }
      lastControlVal = drawerControlVal;
    }
  }

  // step the steppers if permitted
  if (isEmergencyEndStopHit() == false) {
    run(speed, accel);
  }

  // print the current state of the drawer
  int drawerState = checkDrawerState();
  if (drawerState != lastDrawerState) {
#ifdef DEBUG
      Serial.print("current state: ");
#endif    
    Serial.println(drawerState);
    lastDrawerState = drawerState;
  }
}


int checkForCommand(int currentControlVal) {
  int newControlVal = currentControlVal;

  if (Serial.available()) {
    String inputString = Serial.readString();
    inputString.trim();
#ifdef DEBUG
    Serial.print("received: ");
    Serial.print(inputString + " - ");
#endif
    if (inputString.equals("0")) {
      newControlVal = 0;
#ifdef DEBUG
      Serial.println("close");
#endif
    } else if (inputString.equals("1")) {
      newControlVal = 1;
#ifdef DEBUG
      Serial.println("open");
#endif
    } else {
#ifdef DEBUG
      Serial.println("invalid input");
#endif
    }
  }

  return newControlVal;
}


int checkForManualOverride(int currentControlVal) {
  static int lastTriggerState;
  static int triggeredTimeStamp;
  const int debounceTime = 100;
  int newControlVal = currentControlVal;

  if (millis() - triggeredTimeStamp > debounceTime) {
    int isHit = isTriggerHit(overrideButton);
    if (isHit != lastTriggerState) {
      if (isHit == true) {
#ifdef DEBUG
        Serial.println("override triggered - change state");
#endif
        newControlVal = !currentControlVal;
        triggeredTimeStamp = millis();
      }
    }
    lastTriggerState = isHit;
  }

  return newControlVal;
}