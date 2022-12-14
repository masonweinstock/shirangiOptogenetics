/* 
 *  Written by Mason Weinstock for 2-transistor rig
 * Last updated: 12/12/22
 * 
 * To use script, change parameters below, plug in arduino and select 'Sketch->Upload.' This file is read-only such that
 * users don't overwrite default parameters. It is recommended users save parameters in an excperiment by selecting,
 * 'Save-As,' and creating a new file in the folder with the user's optogenetics videos.
 * 
 * See bottom of page for software AND hardware instructions 
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// To change code/cycle parameters, edit the below values
        
        int numBouts;                 // users ignore
        #define ambient 1            // pin to IR backlights
        #define active 3             // [MUST BE ODD #] pin to red/green + indicator lights

        unsigned long lightOn = 15;   // time (sec) the active lights will be on each cycle
        unsigned long lightOff = 45;  // time (sec) the active lights will be off each cycle
        unsigned long prepTime = 60;  // time (sec) after boot-up before signal to record
        int numFlashes = 3;          // # of flashes indicating signal to begin recording
        int recordingWarning = 5;    // time (sec) after flashing signal for user to press record

// Operation mode: ramping or normal
        bool ramping = true;         // set to true if you want intensity to ramp
        int activeIntensity = 255;    // active light intensity for *NON-RAMPING* mode
        int activeIntensities[] = {    // active light intensities for *RAMPING* mode: default ramps +10%/cycle
            25, 50, 75, 100, 125, 150, 175, 200, 225, 255  
            };  
                                  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
    //convert human input -> arduino language //
    lightOn = lightOn * 1000;
    lightOff = lightOff * 1000;
    prepTime = prepTime * 1000;
    recordingWarning = recordingWarning * 1000;

    //deduce number of bouts desired by user
    numBouts = sizeof(activeIntensities)/sizeof(int);

    // setting  pins to output mode
    pinMode(ambient, OUTPUT);
    pinMode(active, OUTPUT);

   //turn on backlight, wait prepTime (sec)     
    digitalWrite(ambient, HIGH);
    delay(prepTime);

  // flash the backlights as
  // signal to press 'record'
    for(int i = 0; i < numFlashes; i++) {
        digitalWrite(ambient, LOW);              
        delay(500);                               
        digitalWrite(ambient, HIGH);               
        delay(500);            
    }
    // delay specified seconds, then begin main cycle
    delay(recordingWarning);
    loop();
}

void loop() {
  
      // ramping mode
      if(ramping==true) {

        // ramp intensity $numBouts times, with intensities of params
        for(int i = 0; i < numBouts; i++) {
          // turn active lights off for specified duration
          digitalWrite(active, LOW);
          delay(lightOff);
  
          // turn active lights on for specified duration
          analogWrite(active, activeIntensities[i]);
          delay(lightOn);
        }   
        
        // after ramping is done, turn lights off, end program
        digitalWrite(active, LOW);
        digitalWrite(ambient, LOW);
        exit(0);
      }

      // normal mode
      else {
        // turn active lights off for specified duration
        digitalWrite(active, LOW);
        delay(lightOff);

        // turn active lights on for specified duration
        analogWrite(active, activeIntensity);
        delay(lightOn);
      }
}

/* 
 *  Software
 *    Program flow
 *      After arduino is powered, IR backlights are powered for $prepTime seconds so the user can focus
 *      the camera, align chambers, attach indicator strip, etc. After this period, the backlights will
 *      flash $numFlashes times, signalling the user to press record on SpinView/DigitalViewer software.
 *      The user will then have $recordingWarning seconds to press record before the program's main loop
 *      will begin. The main loop begins with $lightOff seconds of only-IR exposure, before the first pulse
 *      of active lights for $lightOn seconds.
 *  
 *  Hardware
 *   Notes
 *     The power rails on this breadboard are not set up as usual. The (+) rails *will*
 *     act as a typical (+) rails (carrying current), but the (-) rails serves only to connect
 *     the various (-) ends of the lightstrips in parallel. They only connect to true ground (-)
 *     when the transistor is powered.
 * 
 *     The power rails on this breadboard are not set up as usual. The (+) rails *will*
 *     as a typical (+) rails (carrying current), but the (-) rails serves only to connect
 *     various (-) ends of the lightstrips in parallel. They only connect to true ground (-)
 *     the transistor is powered.
 *     
 *   Instructions:
 *     1. *Through a 1kOHM resistor*, connect pin 1 (base) of *each* transistor to its own arduino digi. pin, for switch control.
 *     2. Connect the power supply's (+) terminal to the closer of the two (+) rails. This rail is now powered.
 *     3. Connect the power supply's (-) terminal to an arbitrary row of 5 squares, which will be our grounding squares.
 *     4. Connect another square in this row to the arduino's digital GND pin, grounding it.
 *     5. Connect another square in this row to pin 3 (emitter) of transistor 1. Now these transistors are grounded... when the transistor is 'open.'
 *     6. Connect one of the transistor's middle pins (collector) its own (+) rail. E.g., connect transistor 1 (IR lights) to the closest (-) rail.
 *     7. Connect IR (+)-leads to this close (+) rail, and the (-) lead to the '(-) rail.' Remember that current only flows if transistor permits.
 *     8. Connect the two (+) rails with a wire, such that the power supply's (+) lead now powers both (+) rails.
 *     9. Repeat steps (5-7) for the other transistor, using the other (-) rail. Now the transistors and lightstrips will function independently of one another!
 *     9b. Step 8 must be repeated (wiring together the (+) rails), as must step 5 (middle pin to further (-) rail), whenever switching active light breadboard rails.
 */
