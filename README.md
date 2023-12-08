# Smart-Alarm-Clock
## Chauncey Carpenter and Max Beaulieu
## Project Idea
For our design project, we are made a smart alarm clock, an embedded system that features a wide selection of input and output devices, starting first with a four digit seven-segment display that shows the current time to the user. The time updates automatically pulls the local time from a WiFi module to prevent any power fluctuations from changing the time. The clock is configured by a user using an RPG. We included a built in speaker that is used to play sounds, some of which are coded into an uSD card that the Mbed can read. We also included a microphone in our device that allows users to record their own wake up messages to be played when an alarm sounds. In addition to the sound, a high power LED on the top of the clock illuminates a room once the alarm goes off. For these additional settings, the alarm clock uses LCD display that shows text to the user and allows them to toggle settings. Users can make their own custom alarm grouping for their own desired combination of LED brightness, color, sound, and alarm time. When the LCD is not actively being used to change settings on the device, it serves as a picture display, since many people put their alarm clocks on a desk or bedside table. A light sensor is used to dim this picture and turn the LCD off when the lights in a room are turned off so that the display does not distract any users while they are sleeping. Another feature that makea our smart alarm clock unique is a built-in space to store a phone overnight or until an alarm goes off to allow users to help prevent themselves from being distracted by their electronics. A pushbutton inside the clock will allow the system to know whether a device is in the casing, at which point it will lock using a servo. A manually entered password using the RPG and LCD will allow users to bypass this lock in case of emergency. Finally, we built a custom casing assembly to house our alarm clock in a way that looks clean and professional.
## Parts List
2x [MBED LPC1768](https://os.mbed.com/platforms/mbed-LPC1768/)
[Rotary Pulse Generator](https://www.sparkfun.com/products/15141) and [Breakout](https://www.sparkfun.com/products/11722)
[Serial Miniature LCD Module]()
[4-Digit 7-Segment Serial Display](https://www.sparkfun.com/products/11441?gclid=CjwKCAiAmsurBhBvEiwA6e-WPEP8WmDgiUsFf63b7yVcwcge2y5nDk6p3JabCTzRN64d8s_ZsfzsixoCqOMQAvD_BwE)
[MEMS Microphone Breakout](https://www.sparkfun.com/products/11377)
[Phototransistor](https://www.sparkfun.com/products/22923)
[PCB Mount Speaker](https://www.sparkfun.com/products/11089)
[Triple Output High Power RGB LED](https://www.sparkfun.com/products/15200)
[microSD Breakout](https://www.sparkfun.com/products/544)
[16 GB uSD Card](https://www.amazon.com/Sandisk-Ultra-Micro-UHS-I-Adapter/dp/B073K14CVB/ref=asc_df_B073K14CVB/?tag=&linkCode=df0&hvadid=309779531175&hvpos=&hvnetw=g&hvrand=9381312203880574580&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1015254&hvtargid=pla-374265929034&mcid=c807c3f631b537b8933a7e4180dbdb3e&ref=&adgrpid=62412137260&gclid=CjwKCAiAmsurBhBvEiwA6e-WPIWyWIFLqb7D1wKH_4Ky3mwcNt26lNEenP0GCpwaWnLJHyinrtvyeBoCzJ4QAvD_BwE&th=1)
[Class D Mono Audio Amp Breakout](https://www.sparkfun.com/products/11044)
[OpAmp Breakout](https://www.sparkfun.com/products/9816)
[HUZZAH ESP8266 Breakout](https://www.adafruit.com/product/2471?gad_source=1&gclid=CjwKCAiAmsurBhBvEiwA6e-WPHXie__MHFwu7-7Y7vulDpLyjev2cK0OwEWCRKstSdSvLrDsJB6mPhoCDqkQAvD_BwE)
[HS-422 Deluxe Standard Servo](https://hitecrcd.com/products/servos/analog/sport-2/hs-422/product)
[SPST NO Button](https://www.sparkfun.com/products/9190)
[5V 2A DC Power Supply](https://www.amazon.com/iMBAPrice-Adapter-Listed-Supply-5-Feet/dp/B00GUO5WUI/ref=asc_df_B00GUO5WUI/?tag=hyprod-20&linkCode=df0&hvadid=194024095585&hvpos=&hvnetw=g&hvrand=5106399020110171904&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9010931&hvtargid=pla-312399822621&psc=1&mcid=00950fc9047636c080a34b44093546ff&gclid=CjwKCAiAmsurBhBvEiwA6e-WPG9c9hOsudIAbyRjluknYXdhm4PiftuPU-BUM3OMwRJSzD9DJgyDlRoCOlUQAvD_BwE)
[5V 4AA Battery Pack](https://www.sparkfun.com/products/9835)
[2x Barrel Jack Adapter](https://www.sparkfun.com/products/10811)
[3x 10K Ohm Resistors](https://www.sparkfun.com/products/14491)
[3x 330 Ohm Resistors](https://www.sparkfun.com/products/14490)
## Instructions
## Hardware Setups
