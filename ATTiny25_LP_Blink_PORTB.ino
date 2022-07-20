/*
 
Ultra Low Power Led Flasher with AtTiny25 @ 128 kHz
Matt Harrison
m-harrison.org
2022-07-20

http://m-harrison.org

Derived from work by Luca Soltoggio
https://arduinoelettronica.com/
 
*/
 
#include <avr/sleep.h>
#include <avr/wdt.h>
 
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
 
volatile bool f_wdt = 1;
 
void setup(){
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
  DDRB = 0b00010000;
}
 
void loop(){
  if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
  f_wdt=0;       // reset flag
  
  setup_watchdog(1);      // Set watchdog duration to 32ms
  PORTB = 0b00010000;     // PB4 High
  system_sleep();         // Go to sleep
  PORTB = 0b00000000;     // PB4 Low
  setup_watchdog(3);      // Set watchdog duration to 128ms
  system_sleep();         // Go to sleep
  setup_watchdog(1);      // Set watchdog duration to 32ms
  PORTB = 0b00010000;     // PB4 High
  system_sleep();         // Go to sleep
  PORTB = 0b00000000;     // PB4 Low
  setup_watchdog(8);      // Set watchdog duration to 4 seconds
  DDRB = 0b00000000;      // Set PB4 to input
  system_sleep();         // Go to sleep
  DDRB = 0b00010000;      // Set PB4 to output
  }
}
 
// set system into the sleep state
// system wakes up when wtchdog is timed out
void system_sleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System sleeps here
  sleep_disable();                     // System continues execution here when watchdog timed out
}
 
// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int16_t ii) {
 
  uint8_t bb;
  int16_t ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;
   
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}
 
// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  f_wdt=1;  // set global flag
}
