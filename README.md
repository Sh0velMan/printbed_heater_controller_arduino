#Printbed Heater Controller for 3D Printers (Arduino)

Summary
----

This sketch is a fully featured controller for external heated print beds intended to be used in 3D Printers which do not come with an integrated print bed heater solution. The project, developed using the Arduino IDE, utilizes Adafruit's BMP280 temperature/humidity sensor breakout board for highly accurate temperature measurement feedback as well as the 7-segment I2C LED Backpack and associated Adafruit libraries. This allows highly accurate temperature control with no analog sensor noise or jitter and a bright, highly readable display to show current temperature as well as confirm setting changes during operation.

####Also...
This could be used to control any other system which requires temperature feedback to turn a high-current (relatively speaking) device on and off, with simple modifications to the logic loop. For example, reversing the "Greater-than" symbols in the logic chain will change the function from "warming something up" to "cooling something down", when paired with a fan, coolant pump or thermoelectric cooler (or some combination of such).

Required Hardware:
----

1. **1 x** Arduino-compatible microcontroller with:
  1. I2C Capability (for sensor and display).
  2. _2 x_ GPIO Pins for use with momentary switches.
  3. _1 x_ PWN-capable pin to control power output to the heating device.
2. **1 x** **logic-level** N-channel MOSFET capable of handling the current requirements of your heating device.
3. **2 x** Momentary switches (buttons)
  * These will trigger increment/decrement of target temperature when the GPIO pins are pulled high.
4. **1 x** Adafruit BMP280 Breakout Board (or similar)
  * https://www.adafruit.com/product/2651
  * https://www.sparkfun.com/products/13676 _Not tested, but I don't see why it wouldn't work, barring an address conflict or other issue I can't predict._
  * It is highly recommended to coat or cover the "business side" of the board with a thermally conductive but electrically non-conductive material which will decrease the likelihood of electrical short, physical damage to the sensor as well as provide some smoothing on the sensor response. These sensors react _extremely_ quickly to changes in temperature caused by basically anything. The slight puff of 'wind' created by moving your hand past the sensor will show a measurable drop in temperature for a second or two, for example. A coating on the components in question will lessen that effect greatly. However, care should be taken to use only "just enough" of whatever material you choose to do the job at hand. Using too much will introduce _crippling_ sensor lag and compromise the efficacy of the entire system.
5. **1 x** Adafruit LED Backpack 7-segment display
  * Dealer's choice on color. Here are my favorites:
    * Green: https://www.adafruit.com/product/880
    * White: https://www.adafruit.com/product/1002
    * Blue: https://www.adafruit.com/product/881
    * Yellow: https://www.adafruit.com/product/879
6. A heating device.
  * This can be a resistive element, thermoelectric cooler (TEC/TED/Peltier Cooler - flip it over or reverse polarity and it's a heater).
7. A power supply of sufficient voltage and current supply for your heating device.
  * Many setups use 12 or 24 VDC power supplies. What you need depends entirely upon your heating device. Take care to design around a power supply voltage which your power MOSFET can tolerate (Drain-Source voltage, or Vds), after the voltage drop across your heating device's natural impedance. With TEC/TED's that impedance can be relatively low, so this may be a concern depending on the voltages and FETs being used.

    While you are not required to _power_ your microcontroller from the same source, a common ground _is required_ in order for this design to function.

Extras
----
* LEDs for indicators
  * Across the control (**ctl**) pin or MOSFET gate, lighting up whenever the heater is actively heating, changing with intensity as the heater duty cycle changes.
  * Across the **Vin** or **3/5v** pin on the microcontroller, as a "power" indicator,
* Enclosure(s) for various components

License
----

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
