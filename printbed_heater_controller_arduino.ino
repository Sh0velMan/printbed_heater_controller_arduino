/**********************************************
   Name: 3D Print Bed Heater Controller for Arduino IDE
   Version: 0.2
   Date: 10/18/2016
   Author: Matt Gustafson (sh0velman)

   Purpose: Controls the operation of a heated print bed for a 3D printer. This
        is important for high quality prints and safety when using a
        non-integrated bed heater that isn't controlled actively by the
        printer itself. See github for more.

   Platforms Tested: Adafruit Feather Huzzah ESP8266, Arduino UNO R3

   License:

       Copyright 2016 Matt Gustafson (sh0velman)

       Licensed under the Apache License, Version 2.0 (the "License");
       you may not use this file except in compliance with the License.
       You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

       Unless required by applicable law or agreed to in writing, software
       distributed under the License is distributed on an "AS IS" BASIS,
       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
       See the License for the specific language governing permissions and
       limitations under the License.

       All libraries utilized within this work are property of their respective
       copyright owners are subject to the license terms outlined therein.

 ***************************************************************************/

#include <Wire.h>
#include <SPI.h> // we don't use SPI but it's a dependency for the Adafruit Sensor library
#include <Adafruit_Sensor.h> // Adafruit's spiffy sensor library - a dependency for all of their sensor breakout boards.
#include <Adafruit_BMP280.h> //  Library for Adafruit's Bosch BMP280 breakout board, a super accurate and reliable digital temperature/pressure sensor.
#include <Adafruit_LEDBackpack.h> // Library for Adafruit's 'LED Backpacks' which let you run many LEDs (or a 7 segment display as in this case) with just the I2C bus.
#include <Adafruit_GFX.h> // A dependency of the LED Backpack library, not explicitly used in our case.


int ctl = 10; // 'Control' pin, the pin which will control the flow of energy through our target device. -- Arduino UNO config!
int swtu = 11;  // Bump our target temperature up 1 degree per loop (up to 4 degrees per second in this case) -- Arduino UNO config!
int swtd = 12;   // Bump our target temperature down 1 degree per loop. -- Arduino UNO config!
int tmpPin = 1;
//int pwmmax = PWMRANGE; // Uncomment the version of this directly below to set an arbitrary 'ceiling' on the PWM signal we produce. -- apparently not all boards have this defined by default, ex: Uno.
int pwmmax = 255; // defining the 8 bit max here because the Uno doesn't have PWMRANGE defined by default.
int pwmtarget;
int loopcount;
float temp;  // Variable to hold the last sampled bed temperature
float targettemp; // Variable to hold the current target temperature
float maxtemp = 115.00; // The maximum temperature (Celcius) we will allow the user to set via the buttons
float mintemp = 10.00; // The minimum temperature we will allow the user to set via the buttons
float defaulttargettemp = 10.00; // Our default target temperature. Set low on purpose so that the heater doesn't slam on the second we come online.
boolean adjustmode; // We'll set this 'true' when the user adjusts the temperature so that we can show the setting chosen on our display.
unsigned long adjusttime; // The point in time when the user changed the temperature so that we can hide the setting chosen after a few seconds (adjustdelay)
int adjustdelay = 4000; // 4 second delay before we go back to showing the current temperature of the bed after changing the targettemp
Adafruit_BMP280 sensor; // variable to hold the instance of our sensor, configured by default to use I2C for communication.
Adafruit_7segment disp = Adafruit_7segment(); // variable to hold the instance of our 7 segment display

void setup() {
        disp.begin(0x70); // display is on I2C address 0x70 in default config, can be changed by soldering jumpers.
        Serial.begin(9600);
        Serial.println(F("Starting up... Letting system settle..."));

        if (!sensor.begin()) {
                Serial.println(F("FATAL ERROR: Could not find a valid BMP280 sensor, check wiring!"));
                Serial.println(F("Halting!"));
                displayError();
                delay(2000);
                while (1) ;
        }



// this code just displays a fun animation on the display during startup. Can be removed if you're the dour sort.
        disp.writeDigitRaw(0,B00000001);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(1,B00000001);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(1,B00000011);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(3,B00010000);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(3,B00011000);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(4,B00001000);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(4,B00001100);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(4,B00001110);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(4,B00001111);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(3,B00011001);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(3,B00111001);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(1,B00000111);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(1,B00001111);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(0,B00001001);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(0,B00011001);
        disp.writeDisplay();
        delay(150);
        disp.writeDigitRaw(0,B00111001);
        disp.writeDisplay();
        delay(250);
        disp.print(8888);
        disp.writeDisplay();
// end our silly antics

        targettemp = defaulttargettemp; // default target temp
        pinMode (ctl, OUTPUT); // make pin 'ctl' an output
        adjustmode = false;
        adjusttime = millis();
        delay(2000);
        disp.clear();
        disp.setBrightness(7);
        disp.writeDisplay();
        loopcount = 0;
        Serial.println(F("Completed... Running now!"));
}

void loop() {
        temp = sensor.readTemperature();
        if (adjustmode == true)
        {
                if (millis() > (adjusttime + adjustdelay))
                {
                        adjustmode = false;
                        disp.setBrightness(7);
                        disp.print(temp);
                        disp.writeDisplay();

                        loopcount = 0;
                } else {
                        disp.setBrightness(15);
                        disp.print(targettemp);
                        disp.writeDisplay();
                }
        } else {


                if (loopcount >= 2)
                {
                        disp.print(temp);
                        disp.writeDisplay();
                        loopcount = 0;
                } else {
                        loopcount+=1;
                }
        }
        if (digitalRead(swtu)== 1 ) // user is pressing the UP button
        {
                adjusttime = millis(); // set our time of adjustment
                adjustmode = true; // set the adjustment mode to true so we can show the new setting on the display
                if (targettemp < maxtemp) // if we aren't already at the max temp
                {
                        targettemp += 1.0; // increment the value by 1.0
                } else { // if we're already at the max temp, set it to the max again to be safe and move on
                        targettemp = maxtemp;
                }
        } else if (digitalRead(swtd)== 1 ) // user is pressing the DOWN button
        {
                adjusttime = millis();
                adjustmode = true;
                if (targettemp > mintemp) // if we aren't already at the minimum temp
                {
                        targettemp -= 1.0; // decrement the value by 1.0
                } else { // if we're already at the minimum, set it to the minimum again to be safe and move on
                        targettemp = mintemp;
                }
        }
        if (temp < 0) // To catch a failed sensor and prevent the heater from coming on full blast indefinitely, we check if the value is less than 0. If you are trying to 3D print in a sub-(water)freezing space, this code would have to be adjusted.
        {
                Serial.print(F("ERROR: Measured Temp = "));
                Serial.print(temp);
                Serial.println(" *C");
                Serial.println(F("Failed sensor?"));
                displayError();
                pwmtarget = 0; // We will hold the pwmtarget at zero until we get a valid (valid-looking anyway) temperature reading to work off of.
        } else if (temp >= targettemp) // Measured temperature is at or above target, turn off the output.
        {
                pwmtarget = 0;
        } else if (temp > (targettemp - 0.5))// Measured temperature is close to target, cut the power back to 40% to try and level in without overshoot.
        {
                pwmtarget = pwmmax * 0.4;
        } else if (temp > (targettemp - 1.0))// Measured temperature is closing to target, cut the power back to 75%.
        {
                pwmtarget = pwmmax * 0.75;
        }  else if (temp > (targettemp - 5.0))// Measured temperature is within 5 degrees of target, cut the power back to 75% to let any gradients that have built up start to equalize.
        {
                pwmtarget = pwmmax * 1; // just leaving this here in case another level of adjustment is needed in the future.
        } else { // Measured temperature is more than 5 degrees below target, run the power at max.
                pwmtarget = pwmmax;
        }
        analogWrite(ctl, pwmtarget); // Write the PWM target to the pin.
        delay(500);
}

void displayError() // function to show 'Err' on the display if something goes wrong. Can only be called after the disp.begin() in the setup block.
{
        disp.writeDigitRaw(0,121); // "E"
        disp.writeDigitRaw(1,80); // "r"
        disp.writeDigitRaw(3,80); // "r"
        disp.writeDigitRaw(4,0); // " "
        disp.writeDisplay(); // Write it out to the display.
}
