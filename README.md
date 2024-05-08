# AutoDrawer

Firmware to open and close a drawer with an actuator

Controls the motion of a stepper motor controlled linear rail
with end stops attached on each end.
On startup the motor will calibrate, fully retracting the gantry plate
to the retracted end stop. Then it will wait for a user command over Serial.

### USAGE

Wait until calibration is complete then:  
Send 1 to fully extend the plate. Send 0 to fully retract the plate.  
Send values as ASCII characters.  
CR and NL are ignored.  
Errant values are ignored.

Tested on an Arduino Uno.

Linear Rail: https://vi.aliexpress.com/item/33049810658.html?spm=a2g0o.order_list.order_list_main.120.498a1802Y4LbKi&gatewayAdapt=glo2vnm  
Stepper Driver: https://fuselab.gr/product/btt-tmc2160-stepper-motor-driver/  
End Stops: https://www.amazon.ca/gp/product/B06XTB7WMK/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1

contact: Nicholas Stedman - nick@devicist.com
