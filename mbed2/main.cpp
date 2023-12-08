#include "Thread.h"
#include "cmsis_os.h"
#include "mbed.h"
#include "rtos.h"
#include "stdio.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <Servo.h>

DigitalOut shdn(p26);

RawSerial  pc(USBTX, USBRX);
RawSerial  dev(p9,p10);
RawSerial  timedisplay(p28,p27);
Serial mbed2(p13, p14);
DigitalOut led1(LED1);
DigitalOut led4(LED4);
Mutex devlock;
static char allcomms[2048];
static int marker = 0;
static bool open = true;
static int status;

// servo
Servo myservo(p21);
DigitalIn lockPin(p30);
DigitalIn btn(p24);
static bool btnlock = false;

// time display function
void displaytime(char* hour, char* minute) {
    mbed2.putc(hour[0]);
    Thread::wait(80);
    mbed2.putc(hour[1]);
    Thread::wait(80);
    mbed2.putc(minute[0]);
    Thread::wait(80);
    mbed2.putc(minute[1]);
    Thread::wait(80);
    mbed2.putc('!');

    pc.printf("sent via serial\n");

    timedisplay.putc(0x76);
    // loop through char in hour and use dev.putc to send to serial
    for (int i = 0; i < 2; i++) {
        timedisplay.putc(hour[i]);
    }

    // send colon
    timedisplay.putc(0x77);
    
    timedisplay.putc(0x10);

    // loop through char in minute and use dev.putc to send to serial
    for (int i = 0; i < 2; i++) {
        timedisplay.putc(minute[i]);
    }
}

// convertGMTtoEST function
std::string convertGMTtoEST(const std::string& gmtHour) {
    // Assuming gmtHour is in the format "HH" (24-hour format)
    int hour = std::atoi(gmtHour.c_str()); // Convert string to integer

    // EST is 5 hours behind GMT
    int estHour = hour - 5;

    // Handle day wrap-around
    if (estHour < 0) {
        estHour += 24;
    }

    // Convert back to string and format to 2 digits if necessary
    std::stringstream ss;
    ss << estHour;
    std::string estHourStr = ss.str();

    if (estHour < 10) {
        estHourStr = "0" + estHourStr;
    }

    return estHourStr;
}

void dev_recv()
{
    led1 = !led1;
    
    while(dev.readable()) {
        devlock.lock();
        char c = dev.getc();
        devlock.unlock();
        pc.putc(c);
        allcomms[marker] = c;
        marker++;
        Thread::wait(80);
    }

}

void send_message(char str[])
{
    int i = 0;

    while (str[i] != '\0') {
        // pc.putc(str[i]);
        devlock.lock();
        dev.putc(str[i]);
        devlock.unlock();
        Thread::wait(80);
        i++;
    }

}

// function to retrieve time after setup
void gettime() {
    while (1){
        for (int i = 0; i < 2048; i++) {
            allcomms[i] = '\0';
        }

        marker = 0;

        char str4[] = "sk = net.createConnection(net.TCP, 0)\n";
        send_message(str4);
        char str5[] = "sk:on('receive', function(sck, c)\nresponse = response .. c\nend)\n";
        send_message(str5);
        char strvar[] = "response = ''\n";
        send_message(strvar);
        char str6[] = "sk:connect(80, 'google.com')\n";
        send_message(str6);
        char str7[] = "sk:send('GET / HTTP/1.1\\r\\n' ..\n'Host: google.com\\r\\n' ..\n'Connection: close\\r\\n' ..\n'Accept: */*\\r\\n\\r\\n')\n";
        send_message(str7);
        char str8[] = "print(response)\n";
        send_message(str8);
        Thread::wait(1000);


        std:string final(allcomms);

        // create an ss object
        std::stringstream ss(final);

        // loop through the ss lines and find the one that starts with "Date:"
        std::string line;
        while (std::getline(ss, line)) {
            if (line.substr(0, 5) == "Date:") {
                break;
            }
        }

        if (line.size() < 20) {
            // retry
            gettime();
            break;
        }


        std::string value = line.substr(23, 8);
        Thread::wait(200);
        char* strfinal = const_cast<char*>(value.c_str());
        
        // from value get first 2 characters
        std::string hour = value.substr(0, 2);

        // from value get index 3 and 4
        std::string minute = value.substr(3, 2);

        // get seconds from index 6 and 7
        std::string second = value.substr(6, 2);  

        hour = convertGMTtoEST(hour);

        char* hourfinal = const_cast<char*>(hour.c_str());
        char* minutefinal = const_cast<char*>(minute.c_str());
        char* secondfinal = const_cast<char*>(second.c_str());

        displaytime(hourfinal, minutefinal);
        Thread::wait(5000);
    }
}

void setup() {
    char str2[] = "wifi.sta.config('Allo', 'colinisshort')\n";
    send_message(str2);
    Thread::wait(5000);
    char str3[] = "print(wifi.sta.getip())\n";
    send_message(str3);
}


void lockCheck() {
    while (1) {
        if (open) {
            myservo.position(-20);
        } else {
            myservo.position(100);
        }
        Thread::wait(100);
    }
}


int main()
{
    pc.baud(9600);
    dev.baud(9600);
    myservo.calibrate();
    btn.mode(PullUp);

    dev.attach(&dev_recv, Serial::RxIrq);

    // connect to the network
    setup();
    // get the current time, display, and send to other mbed
    Thread threadTime(gettime);
    // make sure the lock should be locked / unlocked
    Thread threadLock(lockCheck);

    
    while(1) {
        if (lockPin) {
            open = true;
        }

        if (!btn && !btnlock) {
            open = false;
            btnlock = true;
        } else if (btn && !btnlock) {
            open = true;
        } else if (!btn && btnlock) {
            open = open;
        } else if (btn && btnlock) {
            btnlock = false;
            open = true;
        }

        Thread::wait(1000);
    }
}
