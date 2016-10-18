# 3D Print Bed Heater Controller for Arduino IDE
This sketch is a fully featured 3D Printer print bed heater controller for the Arduino IDE utilizing Adafruit's BMP280 breakout board for sensing temperature and the 7-seg LED Backpack and associated Adafruit libraries.

This could also be used to control any other system which requires temperature feedback to turn a high-current (relatively speaking) device on and off, with simple modifications to the logic loop.

*The minimum required hardware:*
1. Arduino-compatible microcontroller with:
  a. I2C Capability (for sensor and display).
  b. 2 x GPIO Pins for use with momentary switches.
  c. 1 x PWN-capable pin to control power output to the heating device.
2. A logic-level N-channel MOSFET capable of handling the current requirements of your heating device.
3. 2 x Momentary switches (buttons)
  * These will trigger increment/decrement of target temperature when the GPIO pins are pulled high.
4. Adafruit BMP280 Breakout Board (or similar)
  * https://www.adafruit.com/product/2651
  * https://www.sparkfun.com/products/13676 _Not tested, but I don't see why it wouldn't work, barring an address conflict or other issue I can't predict._
  * Highly recommended to coat or cover the "business side" of the board with a thermally conductive but electrically non-conductive material which will decrease the likelihood of electrical short, physical damage to the sensor as well as provide some smoothing on the sensor response. These sensors react _extremely_ quickly to changes in temperature caused by basically anything. The slight puff of 'wind' created by moving your hand past the sensor will show a measureable drop in temperature for a second or two, for example. A coating on the components in question will lessen that effect greatly. Take care to use only enough  of whatever material you choose to do the job at hand. Using too much will introduce crippling sensor lag and compromise the entire system.
5. Adafruit LED Backpack 7-segment display
  * Dealer's choice on color. Here's my favorites:
    * Green: https://www.adafruit.com/product/880
    * White: https://www.adafruit.com/product/1002
    * Blue: https://www.adafruit.com/product/881
    * Yellow: https://www.adafruit.com/product/879
6. A heating device.
  * This can be a resistive element, thermoelectric cooler (flip it over or reverse polarity and it's a heater).
7. A power supply.
  * Many setups use 12 or 24 VDC power supplies. What you need depends entirely upon your heating device. Take care to design around a power supply voltage which your power MOSFET can tolerate (Drain-Source voltage, or Vds). While you are not required to _power_ your microcontroller from the same source, a common ground _is required_ in order for this design to function.

*Extras:*
* LEDs for indicators
* Enclosure(s) for various components

>Copyright 2016 Matt Gustafson (sh0velman)
>
>Licensed under the Apache License, Version 2.0 (the "License");
>you may not use this file except in compliance with the License.
>You may obtain a copy of the License at
>
>  http://www.apache.org/licenses/LICENSE-2.0
>
>Unless required by applicable law or agreed to in writing, software
>distributed under the License is distributed on an "AS IS" BASIS,
>WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
>See the License for the specific language governing permissions and
>limitations under the License.
>
>All libraries utilized within this work are property of their respective
>copyright owners are subject to the license terms outlined therein.
