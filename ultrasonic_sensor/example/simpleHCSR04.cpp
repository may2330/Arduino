/*
 * The MIT License (MIT)

Copyright (c) 2015 Jetsonhacks

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/// simpleHCSR04.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <jetsonGPIO.h>
#include <hcsr04.h>

// segment source
#include "/home/js/JHLEDBackpack/ultrasonic_sensor/JHHC-SR04/segment_sensor/src/JHLEDBackpack.cpp"

using namespace std;

int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    /* set the terminal to raw mode */
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    /* read a character from the stdin stream without blocking */
    /*   returns EOF (-1) if no character is available */
    character = fgetc(stdin);

    /* restore the original terminal attributes */
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}



int main(int argc, char *argv[]){
    // segment display object 
    HT16K33 *displayMatrix = new HT16K33() ;
    // segment open
    displayMatrix->openHT16K33();
    displayMatrix->begin() ;
 //   displayMatrix->clear() ;

    // Use gpio165 as trigger, gpio166 as echo
    HCSR04 *hcsr04 = new HCSR04();
    // Make the HC-SR04 available in user space
    hcsr04->exportGPIO() ;
    // Then set the direction of the pins
    hcsr04->setDirection() ;

    while(getkey() != 27){
        // unsigned int duration = ping() ;
        // unsigned int duration = ping_median(5) ;
        unsigned int duration = hcsr04->pingMedian(5) ;
        if (duration == NO_ECHO) {
          cout << "No echo" << endl ;

          // ----
          // try to print a number thats too long
          // shows dashes & blink
          displayMatrix->print(10000, DEC);
          displayMatrix->writeDisplay();

/*
          // nono
          displayMatrix->writeDigitNum(0,16,0);
          displayMatrix->writeDigitNum(1,17,0);
          displayMatrix->writeDigitNum(3,16,0);
          displayMatrix->writeDigitNum(4,17,0);
          displayMatrix->writeDisplay();
*/

        } else {
            // print out distance in inches and centimeters
            cout << "Duration: " << duration << " Distance (in): " << duration/148 << " Distance (cm): " << duration/58.0 << endl ;
	
            // *********** 0.596
            // print a floating point number
            displayMatrix->printFloat(duration/58.0,3,DEC);
            displayMatrix->writeDisplay();

        }

	sleep(1);
        usleep(10000); // sleep 10 milliseconds
    }
    cout << "HC-SR04 example finished." << endl;

    // segment close
    displayMatrix->end() ;
    displayMatrix->closeHT16K33();


    hcsr04->unexportGPIO() ;

    return 0;
}

