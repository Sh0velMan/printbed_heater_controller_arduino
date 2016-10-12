/***************************************************************************
Name: 3D Print Bed Heater Controller for Arduino IDE
Date: 10/12/2016
Author: Matt Gustafson (sh0velman)
Purpose: Controls the operation of a heated print bed for a 3D printer. This
        is important for high quality prints and safety when using a
        non-integrated bed heater that isn't controlled actively by the
        printer itself. See github for more.
Platforms Tested: Adafruit Feather Huzzah ESP8266
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h> // we don't use SPI but it's a dependency for the Adafruit Sensor library
#include <Adafruit_Sensor.h> // Adafruit's spiffy sensor library - a dependency for all of their sensor breakout boards.
#include <Adafruit_BMP280.h> //  Library for Adafruit's Bosch BMP280 breakout board, a super accurate and reliable digital temperature/pressure sensor.
#include <Adafruit_LEDBackpack.h> // Library for Adafruit's 'LED Backpacks' which let you run many LEDs (or a 7 segment display as in this case) with just the I2C bus.
#include <Adafruit_GFX.h> // A dependency of the LED Backpack library, not explicitly used in our case.

int ctl = 12; // 'Control' pin, the pin which will control the flow of energy through our target device.
int swtu = 13;  // Bump our target temperature up 1 degree per loop (up to 4 degrees per second in this case)
int swtd = 14;   // Bump our target temperature down 1 degree per loop.
int pwmmax = PWMRANGE; // Uncomment the version of this directly below to set an arbitrary 'ceiling' on the PWM signal we produce.
// int pwmmax = 128;
int pwmtarget;
float temp;  // Variable to hold the last sampled bed temperature
float targettemp; // Variable to hold the current target temperature
float maxtemp = 115.00; // The maximum temperature (Celcius) we will allow the user to set via the buttons
float mintemp = 10.00; // The minimum temperature we will allow the user to set via the buttons
float defaulttargettemp = 10.00; // Our default target temperature. Set low on purpose so that the heater doesn't slam on the second we come online.
boolean adjustmode; // We'll set this 'true' when the user adjusts the temperature so that we can show the setting chosen on our display.
unsigned long adjusttime; // The point in time when the user changed the temperature so that we can hide the setting chosen after a few seconds (adjustdelay)
int adjustdelay = 4000; // 4 second delay before we go back to showing the current temperature of the bed after changing the targettemp
Adafruit_BMP280 bmp; // variable to hold the instance of our sensor, configured by default to use I2C for communication.
Adafruit_7segment disp = Adafruit_7segment(); // variable to hold the instance of our 7 segment display

void setup() {
  disp.begin(0x70);
  Serial.begin(9600);
  Serial.println(F("."));
  Serial.println(F(".."));
  Serial.println(F("..."));
  Serial.println(F("...."));
  Serial.println(F("....."));
  Serial.println(F("......"));
  Serial.println(F("Starting up... Letting system settle..."));
  if (!bmp.begin()) {
    Serial.println(F("FATAL ERROR: Could not find a valid BMP280 sensor, check wiring!"));
    Serial.println(F("Halting!"));
    displayError();
    delay(2000);
    while (1);
  }

  disp.print(88.88);
  disp.writeDisplay();

  targettemp = defaulttargettemp; // default target temp
  pinMode (ctl, OUTPUT);  // make pin 'ctl' an output
  adjustmode = false;
  adjusttime = millis();
  delay(4000);
  Serial.println(F("Completed... Running now!"));
}

void loop() {
  temp = bmp.readTemperature();
  if (adjustmode = true)
  {
      if (millis() > (adjusttime + adjustdelay))
        {
          adjustmode = false;
          disp.print(temp);
          disp.writeDisplay();
        } else {
          disp.print(targettemp);
          disp.writeDisplay();
          }
  } else {
          disp.print(temp);
          disp.writeDisplay();
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
    } else  if (digitalRead(swtd)== 1 ) // user is pressing the DOWN button
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
    } else if (temp > (targettemp - 1.0))// Measured temperature is close to target, cut the power back to 25% to try and level in without overshoot.
    {
      pwmtarget = pwmmax * 0.25;
    } else if (temp > (targettemp - 2.0))// Measured temperature is closing to target, cut the power back to 50%.
    {
      pwmtarget = pwmmax * 0.5;
    }  else if (temp > (targettemp - 5.0))// Measured temperature is within 5 degrees of target, cut the power back to 75% to let any gradients that have built up start to equalize.
    {
      pwmtarget = pwmmax * 0.75;
    } else { // Measured temperature is more than 5 degrees below target, run the power at max.
      pwmtarget = pwmmax;
    }
  analogWrite(ctl, pwmtarget); // Write the PWM target to the pin.
  delay (250);
}

void displayError() // function to show 'Err' on the display if something goes wrong. Can only be called after the disp.begin() in the setup block.
{
  disp.writeDigitRaw(0,121); // "E"
  disp.writeDigitRaw(1,80); // "r"
  disp.writeDigitRaw(3,80); // "r"
  disp.writeDisplay(); // Write it out to the display.
}
