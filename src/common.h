#ifndef COMMON_H 
#define COMMON_H

#include <Arduino.h>
#include <avr/eeprom.h>
#include "FastLED.h"

#define PIN_PIR1 4                //пин нижнего датчика движения - d4
#define PIN_PIR2 5                //пин верхнего датчика движения - d5
#define PIN_LED 10                //пин ленты на лестнице - d10
#define PIN_LED_RAIL 11           //пин ленты на перилах - d11
#define BUTTON_PIN 3              //пин кнопки - d3
#define LDR_PIN 0                 //пин датчика света - a0

#define MAX_MODE_NUM 3
#define MLBTW 275

#define AMOUNT 19
#define NUM_STEPS 12
#define NUM_LEDS 228              
#define LEDS_RAIL 60  

struct LADDER{
    byte stripMode = 0;
    byte bright = 0;
    byte effectsBright = 10;
    byte speedRainbow = 20;
    int pirV = 0;
    bool lowerSensor = 0;
    bool upperSensor = 0;
    
    CRGB leds[NUM_LEDS]; 
    CRGB leds_rail[LEDS_RAIL];
};

#endif
