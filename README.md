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
2x [Barrel Jack Adapter](https://www.sparkfun.com/products/10811)  
3x [10K Ohm Resistors](https://www.sparkfun.com/products/14491)  
3x [330 Ohm Resistors](https://www.sparkfun.com/products/14490)
## Instructions
In order to use our device as we created, two MBED LPC1768 boards are required, one running the software in the mbed1 folder and one running the code in the mbed2 folder. The software needs little configuration as is, but there are a few things that may need to be adjusted depending on your location. Our board is configured to display US-EST time, and uses the function convertGMTtoEST() on line 64 in mbed2/main.cpp in order to alter the time returned by the gettime() thread to be usable. If you are outside of the US-EST time zone, you will need to offset this hour returned by the difference between your time zone and US-EST.   
Another thing up to the user are the pictures, sounds, and passwords used by the Smart Alarm Clock. These are all stored on a single uSD card in different folders. The /sd/pictures folder contains .bmp files. The user can add any pictures they want displayed in this folder, with a little setup before. All pictures must be resized to 128x128 pixels and saved as .bmp files to be displayed properly. Most online picture editors work for this. Inside of the sounds folder are the sounds that are played when an alarm sounds. The only sound that the user should worry about setting up on the uSD card is the birds.wav file in this case, as it is the only sound saved sound not recorded by the user. Any wav file can be added to the folder, and to play it the user should change the fopen call in mbed1/main.cpp line 368 to be "sd/pictures/[your new wave file name].wav". the labels on the LCD be changed as well on mbed1/main.cpp line 816 so that it does not display birds if you update your .wav file to a different sound. Finally, to change the password that unlocks the phone drawer, the /sd/pw/pw.txt file can be changed to a new password. Simply open the text file and type in the four numbers you want for the password and the mbed will update that password on reboot. 
The user should also configure the code in mbed2/main.cpp function setup() to connect to the internet through their phone hotspot by changing the code starting at line 187 to include their network name and password.
After the setup is done, the Smart Alarm Clock is ready to be used. Upon starting, the two mbeds will boot up and start initializing hardware and network connections. Once these are initialized, the 4-digit 7-segment display will show time and the LCD will start on the menu screen. The menu screen has three options, Set Alarm, See Alarm, and Unlock. Turning the rotary pulse generator clockwise will scroll down, and counter-clockwise will scroll up. The selected option will be highlighted in blue.  

<img src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/0b416f12-419b-4ac8-821f-00237a7517fd">

If the user clicks down on the rotary pulse generator when selecting Set Alarm, they will be prompted to set an alarm. The first step is to set the time of the alarm, beginning with the hours. The user can cycle through 1-12 hours alternatiung between AM and PM. After clicking on the RPG after setting the desire hour, the user can the switch the minute selected by again spinning the RPG. Clicking again will bring the suer to the sounds configuration page.  

<img src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/039704f6-387c-41f2-906f-59ed674ddecc">

For the sounds configuration page, there are again three options that can be selected the same way. The BEEP option plays a simple tone that beeps for two seconds and waits for one second on repeat. The BIRDS option plays a .wav file of birds chirping. The CUSTOM option prompts the user to speak or sing into the microphone on the top of the box with a countdown for how much recording time is remaining. This recorded audio is what will be played if CUSTOM is selected. After a sound is selected, the user can choose what color they want displayted by the high power RGB LED on top of the alarm clock.  

<img src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/eef901f7-c0fc-43be-b4c6-719ffbe76a86">

To set the color, start turing the RPG clockwise. A blue box will start appearing on the screen, which indicates the blue level desired from the LED. Once the user has decided they want that level, they can click the RPG to switch to modifying green, and the square will update to show what the color will look like with the added green. This is repeated for red, and once complete means that the alarm is set, and will deafult to being on. 

<img src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/f2b4a4c6-52ad-4968-a443-ecc1e16d7fe7">

If the user decides to click on See Alarm from the main menu, they will be brought a screen where they can scroll through all alarms swet using the RPG, with the final option being a return to main menu. These screens show all of the chosen values for each alarm, and clicking the RPG pushbutton while on an alarm in this screen will toggle the alarm on and off.  

<img src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/5ec8b06a-31c7-4e53-bfaa-e4c599411df6">

The final option from the main menu is unlock, which is used when the user had their phone locked in the drawer in the box. This is to be used in case of emergency, but the user can scroll through digits 0-9 using the RPG and click the button to enter their four digit password, and a correct entry will unlock the drawer in teh back using a servo. A back arrow can be used to return to the main menu. The other way to unlock the drawer in the back is to click the RPG pushbutton when an alarm is sounding to turn the alarm  off, which will also unlock ther servo. 

<img src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/fc8fe0e4-fcc4-4cdc-ae8e-6ec7ec18ed85">

When the user is on the main menu page and has not pressed the button in 20 seconds, the LCD will enter a rest mode. A light sensor on the top measures the ambient light level, and if there is significant light the LCD displays the pictures from the uSD card, switching the picture every 60 seconds. If the light level is low, the LCD displays a dim picture of a moon. 

<img src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/90875318/8e6294f1-8559-48ea-b4fd-188b11d456b4" width="400" height="300">

The drawer on the back of the Smart Alarm Clock is the perfect size for the a cell phone and has a groove cut out that allows a pushbutton mounted in  the device to determine if there is a phone in the case. If there is phone in the case, the button will be pressed, signaling the servo to lock the mechanism. 
## Code
The device works by running threads on both mbeds to ensure that all tasks can run concurrently. MBED 1 runs five threads, while MBED 2 runs four threads. A breakdown of how frequently each thread is run can be seen below.  

**MBED 1**

<img width="335" alt="Screen Shot 2023-12-08 at 12 39 14 PM" src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/007ca7da-11da-4f10-8333-8bc1da9084e0">

**MBED 2**

<img width="357" alt="Screen Shot 2023-12-08 at 12 38 55 PM" src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/11c2f192-563c-4d64-ba38-c9cf73359d57">

## Hardware Setups


<img width="546" alt="Screen Shot 2023-12-08 at 1 25 24 PM" src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/89199bab-02c8-4378-8463-f369caba0421">


### MBED1
**Rotary Pulse Generator**

| RPG  | LPC1768  |
| ---- | ---------|
| A    | p24      |
| B    | p15      |
| C    | gnd      |
| SW   | p16      |
| +    | Vout     |

**Serial Miniature LCD Module**

| uLCD  | LPC1768  |
| ----- | ---------|
| 5V    | VU       |
| gnd   | gnd      |
| RX    | p9       |
| TX    | p10      |
| RESET | p11      |

**MEMS Microphone Breakout and OpAmp Breakout**

| Microphone  | OpAmp    | LPC1768  |
| ----------- | ---------| ---------|
| Vin         |          | VU       |
| gnd         | gnd      | gnd      |
| AC          | In       |          |
|             | Vcc      | Vout     |
|             | Out      | p20      |

The AC signal from the microphone should be poth pulled up and pulled down using a 10K ohm resistors as shown below.

<img width="187" alt="Screen Shot 2023-12-08 at 12 47 00 PM" src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/e021806f-337e-488f-ab57-30c2e9a27c0e">

**Phototransistor**

| Phototransistor  | LPC1768  |
| ---------------- | ---------|
| +                | Vout     |
| -                | p19      |  

The negative end of the phototransistor should be pulled down using a 10K ohm resistor as shwon below.  

<img width="175" alt="Screen Shot 2023-12-08 at 12 46 08 PM" src="https://github.com/ccarpenter38/Smart-Alarm-Clock/assets/143563695/ded4e6f0-fd33-4230-b9b6-083067942778">

**PCB Mount Speaker and Mono Audio Amp Breakout**

| Speaker  | Audio Amp  | LPC1768  |
| -------- | -----------| ---------|
| +        | Out+       |          |
| -        | Out-       |          |
|          | In-/Pwr-   | gnd      |
|          | In+        | p18      |
|          | Pwr+       | Vout     |

**Triple Output High Power RGB LED**

| LED                   | LPC1768  |
| --------------------- | ---------|
| Red+                  | p21      |
| Green+                | p22      |
| Blue+                 | p23      |
| Read-/Green-/Blue-    | gnd      |

Typcally an RGB LED would require series resistances for all LEDS, however because this is a high powered LED, the operating volatages for Blue and Green can take in a 3.3V input directly from the MBED. The Red LED require an operating voltage of 2.2-2.6V so it require a series restsor, and in this porject we used 3 330 Ohm restsors in parallel for an effective resistance of 110 Ohms, which dropped the voltage enough.

**microSD Breakout**

| uSD Card Breakout  | LPC1768  |
| ------------------ | ---------|
| CS                 | p8       |
| DI                 | p5       |
| VCC                | Vout     |
| SCK                | p7       |
| gnd                | gnd      |
| DO                 | p6       |

**5V 4AA Battery Pack**

| Battery Pack       | LPC1768  |
| ------------------ | ---------|
| +                  | Vin      |
| -                  | gnd      |

**MBED 2 Connection**

| LPC1768 1 | LPC1768 2 |
| --------- | --------- |
| p14       | p13       |
| p12       | p30       |

### MBED2
**HUZZAH ESP8266 Breakout**

| HUZZAH ESP8266  | LPC1768  |  External Power Supply |
| --------------- | -------- | ---------------------- |
| V+              |          | +5 V >=500 mA          |
| RX              | p9       |                        |
| TX              | p10      |                        |
| gnd             | gnd      | -                      |

**4-Digit 7-Segment Serial Display**

| 4-Digit Display    | LPC1768  |
| ------------------ | ---------|
| Vcc                | Vout     |
| gnd                | gnd      |
| RX                 | p28      |

**HS-422 Deluxe Standard Servo**

| Servo  | LPC1768  |  External Power Supply |
| ------ | -------- | ---------------------- |
| Vin    |          | +5 V >=500 mA          |
| gnd    | gnd      | -                      |
| PWM    | p21      |                        |

**SPST NO Button**

| SPST PB   | LPC1768 |
| --------- | ------- |
| +         | p24     |
| -         | gnd     |

Note: This pushbutton is pulled up in software so no external resistor is required.

**5V 2A DC Power Supply**

| Power Supply       | LPC1768  |
| ------------------ | ---------|
| +                  | Vin      |
| -                  | gnd      |

Note this power supply is also used to power the MBED 2 devices using an external power supply.

**MBED 1 Connection**

| LPC1768 1 | LPC1768 2 |
| --------- | --------- |
| p14       | p13       |
| p12       | p30       |


