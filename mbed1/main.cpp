#include "mbed.h"
#include "PinDetect.h"
#include "SDFileSystem.h"
#include "uLCD_4DGL.h"
#include "wave_player.h"
#include "Servo.h"
#include "stdio.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include "rtos.h"
#include "Mutex.h"
#include <string>
#include <vector>

#define MENU 0
#define ALARMSET 1
#define ALARMREP 11
#define ALARMSEE 2
#define RETURN 21
#define UNLOCK 3


#define KEYBOARD 4
#define TIMESET 5
#define SOUND 6
#define COUNTDOWN 61
#define COLORSET 7
#define PICTURE 8

#define BEEP 1
#define BIRDS 2
#define MIC 3

#define VL53L0_I2C_SDA   p28
#define VL53L0_I2C_SCL   p27

typedef struct Alarm {
    int hours;
    int minutes;
    bool am;
    int sound;
    int color;
    bool on;
};

int arrow[80] = {0x000000, 0x000000, 0x000000, 0xFFFFFF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                    0x000000, 0x000000, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                    0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
                    0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
                    0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
                    0x000000, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
                    0x000000, 0x000000, 0xFFFFFF, 0xFFFFFF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                    0x000000, 0x000000, 0x000000, 0xFFFFFF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000};

int blueArrow[80] = {0x000000, 0x000000, 0x000000, 0x0000FF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                    0x000000, 0x000000, 0x0000FF, 0x0000FF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                    0x000000, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
                    0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
                    0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
                    0x000000, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
                    0x000000, 0x000000, 0x0000FF, 0x0000FF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                    0x000000, 0x000000, 0x000000, 0x0000FF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000};              

Alarm *alarms[10];
volatile int hour = 12;
volatile int minute = 0;
volatile bool am = true;
volatile int numAlarms;
volatile int keySelect = -1;
volatile int LCDMode = MENU;
volatile int MenuSelect = MENU;
volatile int SoundSelect = BEEP;
volatile int ColorSelect = 0x000000;
volatile int pw[4] = {1, 2, 3, 4};
volatile bool haspw = false;
volatile int enteredpw[4];
volatile int numEnter = 0;
volatile int correctpw = 0;
volatile int currAlarm;
volatile bool hr_set = true;
volatile int dispAlarm = 0;
volatile int currDisp = 0;
volatile bool replaceAlarm = false;
volatile bool day = true;
volatile bool nightDrawn = false;
FILE *writefile;
char audiopath[15];
volatile char high;
volatile char low;
volatile unsigned int val;
volatile unsigned int playI;
volatile int reccount = 0;
volatile bool alarmOn = false;
volatile int activeAlarm;
bool canLock = false;

volatile bool lock = false;
volatile bool physicalLock;
volatile int picCount;

Mutex sdLock;
Mutex mux;

uLCD_4DGL uLCD(p9,p10,p11);

InterruptIn RPG_A(p24);//encoder A and B pins/bits use interrupts
InterruptIn RPG_B(p15);
DigitalIn RPG_PB(p16); //encode pushbutton switch "SW" on PCB
SDFileSystem sd(p5, p6, p7, p8, "sd");

PwmOut red(p21);
PwmOut green(p22);
PwmOut blue(p23);

//ervo myservo(p24);

DigitalOut ledPB(LED1);
AnalogIn lightSensor(p19);

AnalogOut speaker(p18);
AnalogIn mic(p20);

DigitalOut tx0(p30);
DigitalOut tx1(p29);
DigitalOut rx0(p26);
DigitalOut rx1(p25);

DigitalOut shdn(p17);


wave_player waver(&speaker);


Serial pc(USBTX,USBRX);

RawSerial  dev(p28,p27);
RawSerial  timedisplay(p13,p14);

Serial rpc(p13, p14);

DigitalOut lockFlag(p12);

Ticker lockFlagCount;


volatile int old_enc = 0;
volatile int new_enc = 0;
volatile int enc_count = 0;
volatile int old_count = 0;
volatile int old_mode = 0;
volatile bool pushed = false;
volatile int screenDown = 0;
volatile bool refresh = false;

const int lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

void Enc_change_ISR(void)
{
    new_enc = RPG_A<<1 | RPG_B;//current encoder bits
    //check truth table for -1,0 or +1 added to count
    enc_count = enc_count + lookup_table[old_enc<<2 | new_enc];
    old_enc = new_enc;
}

//debounced RPG pushbutton switch callback
void PB_callback(void) {
    int i;
    while (1) {
        //printf("%d", RPG_PB);
        if (RPG_PB & !pushed) {
            if (alarmOn) {
                alarmOn = false;
                alarms[activeAlarm]->on = false;
                lockFlag = 1;
                canLock = false;
            } else if (LCDMode == MENU && MenuSelect == UNLOCK) {
                keySelect = -1;
                LCDMode = KEYBOARD;
                enc_count = 0;
            } else if (LCDMode == MENU && MenuSelect == ALARMSET) {
                if (numAlarms == 10) {
                    LCDMode = ALARMREP;
                    replaceAlarm = true;
                } else {
                    LCDMode = TIMESET;
                    currAlarm = numAlarms;
                    enc_count = 0;
                }

            } else if (LCDMode == MENU && MenuSelect == ALARMSEE) {
                LCDMode = ALARMSEE;
                enc_count = 0;

            } else if (LCDMode == KEYBOARD) {
                if (keySelect == 11) {
                    LCDMode = MENU;
                    MenuSelect = MENU;
                    enc_count = 0;
                } else if (keySelect != -1) {
                    enteredpw[numEnter] = keySelect;
                    numEnter++;
                    if (numEnter == 4) {
                        for (i = 0; i < 4; i++) {
                            if (enteredpw[i] != pw[i]) {
                                break;
                            }
                        }
                        if (i == 4) {
                            correctpw = 1;
                        } else {
                            correctpw = -1;
                        }
                    }

                }
            } else if (LCDMode == TIMESET) {
                if (hr_set) {
                    hr_set = false;
                    enc_count = 0;
                } else {
                    LCDMode = SOUND;
                    SoundSelect = -1;
                    enc_count = 0;
                    hr_set = true;
                }
            } else if (LCDMode == SOUND && SoundSelect != -1) {
                if (SoundSelect == MIC) {
                    LCDMode = COUNTDOWN;
                    alarms[currAlarm]->sound = SoundSelect;
                    enc_count = 0;
                } else {
                    LCDMode = COLORSET;
                    alarms[currAlarm]->sound = SoundSelect;
                    enc_count = 0;
                }
            } else if (LCDMode == COLORSET) {
                if (ColorSelect < 2) {
                    ColorSelect++;
                    enc_count = 0;
                } else {
                    enc_count = 0;
                    alarms[currAlarm]->on = true;
                    LCDMode = MENU;
                    MenuSelect = MENU;
                    if (replaceAlarm) {
                        replaceAlarm = false;
                    } else {
                        numAlarms++;
                    }
                    ColorSelect = 0;
                }
            } else if (LCDMode == RETURN) {
                LCDMode = MENU;
                MenuSelect = MENU;
                enc_count = 0;
                dispAlarm = 0;
            } else if (LCDMode == ALARMSEE) {
                if (replaceAlarm) {
                    LCDMode = TIMESET;
                    currAlarm = dispAlarm;
                    dispAlarm = 0;
                } else {
                    alarms[dispAlarm]->on = !alarms[dispAlarm]->on;
                }
            } else if (LCDMode == ALARMREP) {
                LCDMode = ALARMSEE;
                enc_count = 0;
            } else if (LCDMode == PICTURE) {
                LCDMode = MENU;
                MenuSelect = MENU;
                refresh = true;
                nightDrawn = false;
            }
            pushed = true;
            screenDown = 0;
        } else if (!RPG_PB) {
            pushed = false;
            if (screenDown < 200) {
                screenDown++;
            }
        }
        Thread::wait(100);
    }
}

void screenRest() {
    int count = 0;
    while (1) {
        if (lightSensor * 10.0 > 0.3) {
            count++;
            if (count == 3) {
                day = true;
                count = 0;
            } else if (count < 0) {
                count = 0;
            }
        }
        
        if (lightSensor * 10.0  < 0.25) {
            count--;
            if (count == -3) {
                day = false;
                count = 0;
            } else if (count > 0) {
                count = 0;
            }
        }

        if (screenDown == 200 && LCDMode == MENU && !alarmOn) {
            refresh = true;
            LCDMode = PICTURE;
            screenDown = 201;
        }
        pc.printf("Light level:%8.6f\n", lightSensor * 100.0);
        //pc.printf("%d\n", count);
        Thread::wait(5000);
    }
}


void checkAlarm() {
    float r, g, b;
    bool flag = true;
    float * beepOut = (float *) malloc(sizeof(int) * 128);
    for(int k=0; k<128; k++) {
        beepOut[k]=((1.0 + sin((float(k)/128.0*6.28318530717959)))/2.0);
        // scale the sine wave from 0.0 to 1.0 - as needed for AnalogOut arg 
    }
    int i = 0;
    int total = 0;
    char waveStr[15];
    char lhigh;
    char llow;
    
    while (1) {
        for (int i = 0; i < numAlarms; i++) {
            mux.lock();
            if (alarms[i]->on && alarms[i]->am == am && alarms[i]->hours == hour && alarms[i]->minutes == minute) {
                ledPB = 1;
                alarmOn = true;
                activeAlarm = i;
            }
            mux.unlock();
            if (alarmOn) {
                red = (float) (((alarms[activeAlarm]->color & 0x0FF0000) >> 16));
                green = (float) (((alarms[activeAlarm]->color & 0x000FF00) >> 8));
                blue = (float) (((alarms[activeAlarm]->color & 0x00000FF)));

                if (alarms[activeAlarm]->sound == BEEP) {
                while (alarmOn) {
                    speaker = beepOut[i];
                    i++;
                    total++;
                    if (i > 127) i = 0;
                    if (total > 128000) {
                        total = 0;
                        Thread::wait(1000.0);
                    } else {
                        Thread::wait(1.0/(50.0*128));
                    }
                }
            }  else if  (alarms[activeAlarm]->sound == BIRDS) {
                FILE *wave_file;
                while(alarmOn) {
                    sdLock.lock();
                    wave_file=fopen("/sd/sounds/birds.wav","r");
                    waver.play(wave_file);
                    fclose(wave_file);
                    sdLock.unlock();
                }
            } else {
                FILE *wave_file;
                while (alarmOn) {
                    sdLock.lock();
                    sprintf(waveStr, "/sd/sounds/custom%d.txt", activeAlarm);
                    wave_file=fopen(waveStr,"r");
                    while (!feof(wave_file)) {
                        lhigh = getc(wave_file);
                        llow = getc(wave_file);
                        i =  (lhigh << 8 | llow);
                        speaker.write_u16(i);
                        Thread::wait(1.0/270.0);
                    }
                    fclose(wave_file);
                    sdLock.unlock();
                    Thread::wait(1000.0);
                }
            }

            } else {
                red = 0.0;
                blue = 0.0;
                green = 0.0;
                total = 0;
            }
            

        }
        Thread::wait(1000.0);
    }
}


void pr() {
    char * hr = (char *) malloc(sizeof(char) * 2);
    char * mn = (char *) malloc(sizeof(char) * 2);
    while(1) {
        mux.lock();
        if (rpc.readable()) {
            pc.printf("1");
            hr[0] = rpc.getc();
            pc.printf("2");
            hr[1] = rpc.getc();
            pc.printf("3");
            mn[0] = rpc.getc();
            pc.printf("4");
            mn[1] = rpc.getc();
            hour = atoi(hr);
            if (hour >= 12) {
               am = false;
            } else {
               am = true;
            }
            hour = hour % 12;
            if (hour == 0) {
                hour = 12;
            }
            minute = atoi(mn);
            rpc.getc();
            pc.printf("%d:%d", hour, minute);
            if (am) {
                printf("am");
            }
            lockFlag = 0;
            }
        mux.unlock();
        pc.printf("out");
        Thread::wait(1000.0);
    }
}



int main() {


    RPG_PB.mode(PullDown);
    RPG_A.mode(PullUp);
    RPG_B.mode(PullUp);
// generate an interrupt on any change in either encoder bit (A or B)
    RPG_A.rise(&Enc_change_ISR);
    RPG_A.fall(&Enc_change_ISR);
    RPG_B.rise(&Enc_change_ISR);
    RPG_B.fall(&Enc_change_ISR);
    uLCD.cls();

    char r; 
    char b;
    char g;
    vector<string> filenames;
    DIR *dp;
    struct dirent *dirp;
    do {
        dp = opendir("/sd/pictures");
    } while (dp == NULL);
    while((dirp = readdir(dp)) != NULL) {
        filenames.push_back(string(dirp->d_name));
    }

    closedir(dp); 

    vector<string>::iterator it = filenames.begin();
    char path[50];

    FILE *pfile = fopen("/sd/pw/pw.txt", "r");
    pw[0] = (int) (fgetc(pfile) - '0');
    pw[1] = (int) (fgetc(pfile) - '0');
    pw[2] = (int) (fgetc(pfile) - '0');
    pw[3] = (int) (fgetc(pfile) - '0');
    fclose(pfile);

    lockFlag = 0;

    //myservo.calibrate();

  
    Thread thread2(PB_callback);

    Thread thread3(screenRest);


    Thread thread4(checkAlarm);
    Thread thread5(pr);



    pc.printf("Starting");
    for (int i = 0; i < 10; i++) {
        alarms[i] = (Alarm *) malloc(sizeof(Alarm));
        alarms[i]->hours = 12;
        alarms[i]->minutes = 0;
        alarms[i]->am = true;
        alarms[i]->sound = BEEP;
        alarms[i]->color = 0x000000;
        alarms[i]->on = false;
    }
    while(1) {
        mux.lock();
        if (LCDMode == MENU) {
            if (old_mode != MENU) {
                uLCD.cls();
                old_mode = MENU;
            }
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.locate(3, 0);
            uLCD.color(WHITE);
            uLCD.printf("MENU");
            uLCD.filled_rectangle(0,22,127,25, WHITE);
            uLCD.filled_rectangle(0,55,127,58, WHITE);
            uLCD.filled_rectangle(0,87,127,90, WHITE);
            uLCD.filled_rectangle(0,118,127,121, WHITE);
            uLCD.locate(0,2);

            if (enc_count / 15 == 1) {
                MenuSelect = ALARMSET;
                uLCD.color(BLUE);
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("Set Alarm");

            uLCD.locate(0,4);
            if (enc_count / 15 == 2) {
                MenuSelect = ALARMSEE;
                uLCD.color(BLUE);
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("See Alarm");
            
            uLCD.locate(0,6);
            if (enc_count / 15 == 3) {
                MenuSelect = UNLOCK;
                uLCD.color(BLUE);
            } else {
                uLCD.color(WHITE);
            }            
            uLCD.printf("Unlock");

            if (enc_count > 59) {
               enc_count = 0;
            } else if (enc_count < 0) {
                enc_count = 59;
            }
        } else if (LCDMode == KEYBOARD) {
            if (old_mode != KEYBOARD) {
                uLCD.cls();
                old_mode = KEYBOARD;
            }
            uLCD.text_height(2);
            uLCD.text_width(2);

            uLCD.locate(1,0);
            if (enc_count / 15 == 1) {
                uLCD.color(BLUE);
                keySelect = 1;
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("1");

            uLCD.locate(4,0);
            if (enc_count / 15 == 2) {
                uLCD.color(BLUE);
                keySelect = 2;
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("2");

            uLCD.locate(7,0);
            if (enc_count / 15 == 3) {
                uLCD.color(BLUE);
                keySelect = 3;
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("3");

            uLCD.locate(1,2);
            if (enc_count / 15 == 4) {
                uLCD.color(BLUE);
                keySelect = 4;
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("4");

            uLCD.locate(4,2);
            if (enc_count / 15 == 5) {
                uLCD.color(BLUE);
                keySelect = 5;
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("5");

            uLCD.locate(7,2);
            if (enc_count / 15 == 6) {
                uLCD.color(BLUE);
                keySelect = 6;
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("6");

            uLCD.locate(1,4);
            if (enc_count / 15 == 7) {
                uLCD.color(BLUE);
                keySelect = 7;
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("7");

            uLCD.locate(4,4);
            if (enc_count / 15 == 8) {
                uLCD.color(BLUE);
                keySelect = 8;
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("8");

            uLCD.locate(7,4);
            if (enc_count / 15 == 9) {
                uLCD.color(BLUE);
                keySelect = 9;
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("9");

            uLCD.locate(4,6);
            if (enc_count / 15 == 10) {
                uLCD.color(BLUE);
                keySelect = 0;
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("0");

            if (enc_count / 15 == 11) {
                uLCD.BLIT(14, 100, 10, 8, blueArrow);
                keySelect = 11;
            } else {
                uLCD.BLIT(14, 100, 10, 8, arrow);
            }

            if (enc_count / 15 == 0) keySelect = -1;

            switch (numEnter) {
                case 1:
                    uLCD.filled_circle(92, 103, 3, WHITE);
                    break;
                case 2:
                    uLCD.filled_circle(100, 103, 3, WHITE);
                    break;
                case 3:
                    uLCD.filled_circle(108, 103, 3, WHITE);
                    break;
                case 4:
                    if (correctpw == 1) {
                        for (int i = 0; i < 3; i++) {
                            lock = false;
                            uLCD.filled_circle(92, 103, 3, GREEN);
                            uLCD.filled_circle(100, 103, 3, GREEN);
                            uLCD.filled_circle(108, 103, 3, GREEN);
                            uLCD.filled_circle(116, 103, 3, GREEN);
                            Thread::wait(500);
                            uLCD.filled_rectangle(86, 98, 127, 127, BLACK);
                            lockFlag = 1;
                            canLock = false;
                            Thread::wait(500);
                        }
                        numEnter = 0;
                        LCDMode = MENU;
                        MenuSelect = MENU;
                        enc_count = 0;
                        uLCD.cls();
                    } else if (correctpw == -1) {
                        for (int i = 0; i < 3; i++) {
                            uLCD.filled_circle(92, 103, 3, RED);
                            uLCD.filled_circle(100, 103, 3, RED);
                            uLCD.filled_circle(108, 103, 3, RED);
                            uLCD.filled_circle(116, 103, 3, RED);
                            Thread::wait(500);
                            uLCD.filled_rectangle(86, 98, 127, 127, BLACK);
                            Thread::wait(500);
                        }
                        numEnter = 0;
                        keySelect = 1;
                        enc_count = 0;
                    }
                    break;
                default:
                    break;
            }

            if (enc_count > 179) {
                enc_count = 0;
            }

            if (enc_count < 0) {
                enc_count = 179;
            }

        } else if (LCDMode == TIMESET) {
            if (old_mode != TIMESET) {
                uLCD.cls();
                uLCD.text_width(2);
                uLCD.text_height(2);
                uLCD.locate(3, 0);
                uLCD.color(WHITE);
                uLCD.printf("Time");
                old_mode = TIMESET;
            }
            uLCD.text_width(3);
            uLCD.text_height(3);
            uLCD.locate(0,2);
            if (alarms[currAlarm]->minutes/10 == 0) {
                uLCD.printf("%2d:0%1d", alarms[currAlarm]->hours, alarms[currAlarm]->minutes);
            } else {
                uLCD.printf("%2d:%2d", alarms[currAlarm]->hours, alarms[currAlarm]->minutes);
            }

            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.locate(4,6);
            if  (alarms[currAlarm]->am) {
                uLCD.printf("AM");
            } else {
                uLCD.printf("PM");
            }
            if (hr_set) {
                if (enc_count > 8) {
                    alarms[currAlarm]->hours++;
                    if (alarms[currAlarm]->hours == 12) {
                        alarms[currAlarm]->am = !alarms[currAlarm]->am;
                    }
                    enc_count = 0;
                } else if (enc_count < -8) {
                    alarms[currAlarm]->hours--;
                    if (alarms[currAlarm]->hours == 11) {
                        alarms[currAlarm]->am = !alarms[currAlarm]->am;
                    }
                    enc_count = 0;
                }
            } else {
                if (enc_count > 8) {
                    alarms[currAlarm]->minutes++;
                    enc_count = 0;
                } else if (enc_count < -8) {
                    alarms[currAlarm]->minutes--;
                    enc_count = 0;
                }
            }

            if (alarms[currAlarm]->minutes == 60) {
                alarms[currAlarm]->minutes = 0;
            } else if (alarms[currAlarm]->minutes == -1) {
                alarms[currAlarm]->minutes = 59;
            }

            if (alarms[currAlarm]->hours == 13) {
                alarms[currAlarm]->hours = 1;
            } else if (alarms[currAlarm]->hours == 0) {
                alarms[currAlarm]->hours = 12;
            }

        } else if (LCDMode == SOUND) {
            if (old_mode != SOUND) {
                uLCD.cls();
                uLCD.text_width(2);
                uLCD.text_height(2);
                uLCD.locate(2, 0);
                uLCD.printf("SOUND");
                old_mode = SOUND;
            }
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.filled_rectangle(0,22,127,25, WHITE);
            uLCD.filled_rectangle(0,55,127,58, WHITE);
            uLCD.filled_rectangle(0,87,127,90, WHITE);
            uLCD.filled_rectangle(0,118,127,121, WHITE);

            uLCD.locate(0,2);
            if (enc_count / 15 == 1) {
                SoundSelect = BEEP;
                uLCD.color(BLUE);
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("Beep");

            uLCD.locate(0,4);
            if (enc_count / 15 == 2) {
                SoundSelect = BIRDS;
                uLCD.color(BLUE);
            } else {
                uLCD.color(WHITE);
            }
            uLCD.printf("Birds");
            
            uLCD.locate(0,6);
            if (enc_count / 15 == 3) {
                SoundSelect = MIC;
                uLCD.color(BLUE);
            } else {
                uLCD.color(WHITE);
            }            
            uLCD.printf("Custom");

            if (enc_count > 59) {
               enc_count = 0;
            } else if (enc_count < 0) {
                enc_count = 59;
            }
        } else if (LCDMode == COLORSET) {
            if (old_mode != COLORSET) {
                uLCD.cls();
                uLCD.text_width(2);
                uLCD.text_height(2);
                uLCD.locate(2, 0);
                uLCD.color(WHITE);
                uLCD.printf("COLOR");
                old_mode = COLORSET;
            }

            if (enc_count > 255*4) {
               enc_count = 255*4;
            } else if (enc_count < 0) {
                enc_count = 0;
            }
            if (ColorSelect == 0) {
                alarms[currAlarm]->color = enc_count/4;
            } 
            if (ColorSelect == 1) {
                alarms[currAlarm]->color = (alarms[currAlarm]->color & 0x0000FF) | ((enc_count/4)<<8);
            }
            if (ColorSelect == 2) {
                alarms[currAlarm]->color = (alarms[currAlarm]->color & 0x00FFFF) | (((enc_count/4)<<16) & 0xFF0000);
            }
            
            uLCD.filled_rectangle(10, 20, 117, 117, alarms[currAlarm]->color);
        } else if (LCDMode == ALARMSEE || LCDMode == RETURN) {

            if (enc_count > (numAlarms + 1) * 16) {
               enc_count = (numAlarms + 1) * 16;
            } else if (enc_count < 0) {
                enc_count = 0;
            }
            dispAlarm = (enc_count - 1) / 16;

            if (dispAlarm == numAlarms) {
                if (old_mode != RETURN) {
                    uLCD.cls();
                    LCDMode = RETURN;
                    old_mode = RETURN;
                }
                uLCD.text_width(2);
                uLCD.text_height(2);
                uLCD.color(WHITE);
                uLCD.locate(1,2);
                uLCD.printf("BACK TO");
                uLCD.locate(0, 3);
                uLCD.printf("MAIN MENU");

            } else {
                if (old_mode != ALARMSEE) {
                    uLCD.cls();
                    LCDMode = ALARMSEE;
                    old_mode = ALARMSEE;
                }
                if (dispAlarm != currDisp) {
                    uLCD.cls();
                    currDisp = dispAlarm;
                }
                uLCD.text_width(2);
                uLCD.text_height(2);
                uLCD.color(WHITE);
                uLCD.locate(1, 0);
                uLCD.printf("ALARM#%d", dispAlarm + 1);
                uLCD.locate(0, 2);
                if (alarms[dispAlarm]->minutes/10 == 0) {
                    uLCD.printf("%2d:0%1d", alarms[dispAlarm]->hours, alarms[dispAlarm]->minutes);
                } else {
                    uLCD.printf("%2d:%2d", alarms[dispAlarm]->hours, alarms[dispAlarm]->minutes);
                }
                uLCD.locate(6,2);
                if (alarms[dispAlarm]->am) {
                    uLCD.printf("AM");
                } else {
                    uLCD.printf("PM");
                }

                uLCD.locate(0,3);
                if (alarms[dispAlarm]->sound == BEEP) {
                    uLCD.printf("BEEP");
                } else if (alarms[dispAlarm]->sound == BIRDS) {
                    uLCD.printf("BIRDS");
                } else {
                    uLCD.printf("CUSTOM");
                }

                uLCD.locate(0,4);
                uLCD.color(alarms[dispAlarm]->color);
                uLCD.printf("COLOR");

                uLCD.locate(0, 5);
                if (alarms[dispAlarm]->on) {
                    uLCD.color(GREEN);
                    uLCD.filled_rectangle(26, 80, 127, 127, BLACK);
                    uLCD.printf("ON");
                } else {
                    uLCD.color(RED);
                    uLCD.printf("OFF");
                }
            } 

        } else if (LCDMode == ALARMREP) {
                if (old_mode != ALARMREP) {
                    uLCD.cls();
                    old_mode = ALARMREP;
                }
                uLCD.text_width(2);
                uLCD.text_height(2);
                uLCD.locate(0, 1);
                uLCD.printf("OVER 10");
                uLCD.locate(0, 2);
                uLCD.printf("ALARMS!");
                uLCD.locate(0, 4);
                uLCD.printf("CHOOSE AN");
                uLCD.locate(0, 5);
                uLCD.printf("ALARM TO");
                uLCD.locate(0, 6);
                uLCD.printf("REPLACE");

        } else if (LCDMode == PICTURE) {
            if (old_mode != PICTURE) {
                uLCD.cls();
                old_mode = PICTURE;
                picCount = 1201;
            }
            if (day) {
                if (picCount > 1200) {
                    picCount = 0;
                    uLCD.text_height(2);
                    uLCD.text_width(2);
                    uLCD.color(WHITE);
                    uLCD.locate(7,7);
                    if (am) {
                        uLCD.printf("AM");
                    } else {
                        uLCD.printf("PM");
                    }

                    int pic[128];
                    int shortPic[98];
                    sprintf(path, "/sd/pictures/%s", (*it).c_str());
                    sdLock.lock();
                    FILE *fp = fopen(path, "rb");
                    for (int i = 0; i < 138; i++) {
                        fgetc(fp);
                    }
                    for (int i = 127; i >= 0; i--) {
                        for (int j = 0; j < 128; j++) {
                            b = fgetc(fp);
                            g = fgetc(fp);
                            r = fgetc(fp);
                            if (i > 109) {
                                if (j < 98) {
                                    shortPic[j] = (int) (b | g<<8 | r << 16 );
                                }
                            } else {
                                pic[j] = (int) (b | g<<8 | r << 16 );
                            }
                        }
                        if (i > 109) {
                            uLCD.BLIT(0,i, 98, 1, shortPic);
                        } else {
                            uLCD.BLIT(0,i, 128, 1, pic);
                        }
                        if (LCDMode != PICTURE) {
                            break;
                        }
                        
                        //Thread::wait(50);
                    }
                    fclose(fp);
                    sdLock.unlock();
                    it++;
                    (*it).c_str();
                    it++;
                    if (it >= filenames.end()) {
                        it = filenames.begin();
                    }
                    refresh = false;
                    nightDrawn = false;
                }
                picCount++;

            } else {
                if (!nightDrawn) {
                    uLCD.filled_rectangle(0, 0, 127, 127, 0x120629);
                    uLCD.filled_rectangle(98, 110, 127, 127, BLACK);
                    uLCD.filled_circle(63, 63, 40, 0x7abfbf);
                    uLCD.filled_circle(45, 45, 40, 0x120629);
                    nightDrawn = true;
                    refresh = true;
                }
                
            }
            uLCD.text_height(2);
            uLCD.text_width(2);
            uLCD.color(WHITE);
            uLCD.locate(7,7);
            if (am) {
                uLCD.printf("AM");
            } else {
                uLCD.printf("PM");
            }
        } else if (LCDMode == COUNTDOWN) {
            old_mode = COUNTDOWN;
            uLCD.cls();
            uLCD.text_height(5);
            uLCD.text_width(5);
            uLCD.color(WHITE);
            sprintf(audiopath, "/sd/sounds/custom%d.txt", currAlarm);
            sdLock.lock();
            writefile = fopen(audiopath, "w");
            reccount = 0;
            uLCD.locate(1,1);
            uLCD.printf("3");
            ledPB = 1;

            while (reccount < 60000) {
                val = mic.read_u16();
                high = (val & 0x0FF00) >> 8;
                low = (val & 0x000FF);
                putc(high, writefile);
                putc(low, writefile);
                reccount++;
                if (reccount == 20000) {
                    uLCD.filled_rectangle(20, 20, 108, 108, BLACK);
                    uLCD.locate(1,1);
                    uLCD.printf("2");
                } else if (reccount == 40000) {
                    uLCD.filled_rectangle(20, 20, 108, 108, BLACK);
                    uLCD.locate(1,1);
                    uLCD.printf("1");
                }
                Thread::wait(1.0/20.0);
            }

            fclose(writefile);
            sdLock.unlock();
            ledPB = 0;
            LCDMode = COLORSET;
            
        }
        mux.unlock();
    //pc.printf("%d\n", LCDMode);
    Thread::wait(50);
    }
}
#include "mbed.h"
#include "PinDetect.h"
//See http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html
//for a detailed explanation of the RPG encoder counting algorithm
//uses Sparkfun RPG with RGB led on breakout board (#15141,11722,10597)
//place RPG PCB across a breadboard power bus strip for easier pin hookup!
InterruptIn RPG_A(p14,PullUp);//encoder A and B pins/bits use interrupts
InterruptIn RPG_B(p15,PullUp);
PinDetect RPG_PB(p16); //encode pushbutton switch "SW" on PCB
//PWM setup for RGB LED in enocder
PwmOut red(p21);//"R" pin
PwmOut blue(p22);//"G" pin
PwmOut green(p23);//"B" pin
//Note: also tie RPG PCB "C" pin to ground, "+" pin to 3.3
//mbed status leds
DigitalOut ledPB(LED1);
DigitalOut red_adjust_mode(LED2);
DigitalOut green_adjust_mode(LED3);
DigitalOut blue_adjust_mode(LED4);
//Serial pc(USBTX,USBRX);
volatile int old_enc = 0;
volatile int new_enc = 0;
volatile int enc_count = 0;
//Instead of a slow 16 case statement use a faster table lookup of truth table
//index table with (previous encoder AB value<<2 | current current encoder AB value) 
//to find -1(CCW),0,or 1(CW) change in count each time a bit changes state
//Always want Interrupt routines to run fast!
//const puts data in read only Flash instead of RAM
const int lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
//Encoder bit change interrupt service routine
//called whenever one of the two A,B encoder bits change state
void Enc_change_ISR(void)
{
    new_enc = RPG_A<<1 | RPG_B;//current encoder bits
    //check truth table for -1,0 or +1 added to count
    enc_count = enc_count + lookup_table[old_enc<<2 | new_enc];
    old_enc = new_enc;
}
//debounced RPG pushbutton switch callback
void PB_callback(void)
{
    ledPB= !ledPB;
}
int main()
{
//turn off built-in RPG encoder RGB led
    red = 1.0;//PWM value 1.0 is "off", 0.0 is full "on"
    green = 1.0;
    blue = 1.0;
//current color adjust set to red
    red_adjust_mode = 1;
//debounce RPG center pushbutton
    RPG_PB.mode(PullDown);
    RPG_PB.attach_deasserted(&PB_callback);
    RPG_PB.setSampleFrequency();
// generate an interrupt on any change in either encoder bit (A or B)
    RPG_A.rise(&Enc_change_ISR);
    RPG_A.fall(&Enc_change_ISR);
    RPG_B.rise(&Enc_change_ISR);
    RPG_B.fall(&Enc_change_ISR);
    while (true) {
        // Scale/limit count to 0..100
        if (enc_count>100) enc_count = 100;
        if (enc_count<0) enc_count = 0;
        red = 1.0 - enc_count/100.0;
//        pc.printf("%D\n\r",enc_count);
    }
}
