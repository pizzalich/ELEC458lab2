//########################################################################
//# MILESTONE : 2
//# PROGRAM : 2
//# PROJECT : Lab2
//# GROUP : X
//# NAME 1 : Marc, Bonwick V00707226
//# NAME 2 : Mario, Dellaviola, V00817406
//# DESC : This program does Nightrider using the internal oscillators
//# DATA
//# REVISED 28-01-2019
//########################################################################

#include <stdlib.h>        // the header of the general purpose standard library of C programming language
#include <avr/io.h>        // the header of i/o port
#include <avr/interrupt.h> // Delay functions for AT90USBKey

//################## MAIN ROUTINE ##################

int main(int argc, char *argv[])
{
    //START setup functions
    DDRC = 0b11111111;   // Sets all pins on Port C to output
    PORTC = 0b00000001;  // Set Ititial position for Nightrider LED pattern on far right
    TCCR1B |= _BV(CS10); // Initalize the timer control register, clock scaling CS01
    flag = 1;            //Flag nightrider for dirrection 1===Left

    //Start main rountine
    //Do nightRider and delay
    while (1)
    {
        nightRider(flag);
        mTimer(150);
    }
}

//ms timer function using the internal oscillators, Count is the desired #ms
void mTimer(int count)
{
    int i = 0;                    //loop counter
    TCCR1B |= _BV(WGM12);         // Set to Waveform Generator Mode 12 (Clear on Timer Compare)
    OCR1A = 0x03E8;               // Set output compare register for 1000 cycles(1ms)
    TCNT1 = 0x0000;               // Set initalal counter timer time to 0
    TIMSK1 = TIMSK1 | 0b00000010; // Enable output compare interrupt enable
    TIFR1 |= _BV(OCF1A);          // Clear interrupt flag and start timer

    //TODO: switch this to a real interrupt
    while (i < count)
    {
        //When the interrups flag is triggered
        if ((TIFR1 & 0x02) == 0x02)
        {
            TIFR1 | +_BV(OCF1A); //Clear the flag
            i++;                 //increment the loop
        }
    }
    return
}

//Function to perform the nessessary nightrider shifts
void nightRider(volatile char flag)
{
    if (flag)
    {
        //For PORTC <= 0b00001111 the increase in non linear, deal with this using ifs
        if (PORTC == 0b00000001)
            PORTC = 0b00000011;
        else if (PORTC == 0b00000011)
            PORTC = 0b00000111;
        else if (PORTC == 0b00000111)
            PORTC = 0b00001111;
        //If PORTC > 0b00001111 Bitshift the registers left
        else
            PORTC = PORTC << 1;
        //If PORTC	=== 0b10000000 toggle the flang and go back to the right
        if (PORTC == 0b10000000)
            flag = 0;
        //if flag is not toggled move right
    }
    else
    {
        //For PORTC <= 0b1111000 the increase in non linear, deal with this using ifs
        if (PORTC == 0b10000000)
            PORTC = 0b11000000;
        else if (PORTC == 0b11000000)
            PORTC = 0b11100000;
        else if (PORTC == 0b11100000)
            PORTC = 0b11110000;
        //If PORTC < 0b1111000 Bitshift the registers right
        else
            PORTC = PORTC >> 1;
        //If PORTC	=== 0b00000001 toggle the flang and go back to the left
        if (PORTC == 0b00000001)
            flag = 1;
    }
    return;
}
