#include "effects.h"

byte counter;
byte counter2;

bool waitUpper = 0;
bool waitLower = 0;

int sensorCount = 0;
long int workTime = 0;


void defaultEffect(struct LADDER &ladder){

    switch(ladder.stripMode){
        case 0:
            for (int i = 0; i < NUM_LEDS; i++)
                ladder.leds[i] = CHSV(0, 0, 0); 
            FastLED.show();
            break;
        case 1:
            for (int i = 0; i < NUM_LEDS; i++)
                ladder.leds[i] = CRGB::White;
            FastLED.show();
            break;
        case 2:
            FastLED.setBrightness(ladder.effectsBright);
            FastLED.show();
            for (int i = 0; i < NUM_LEDS; i++ )
                ladder.leds[i] = CHSV(counter + i * 2, 255, 255);
            counter++;        
            FastLED.show();
            delay(ladder.speedRainbow);
            break;
        case 3:
            FastLED.setBrightness(ladder.effectsBright);
            FastLED.show();
            for (int i = 0; i < NUM_LEDS; i++ )
                ladder.leds[i] = CHSV(counter2, 255, 255);
            counter2++;        
            FastLED.show();
            delay(ladder.speedRainbow);
            break;
        default:
            ladder.stripMode = 0;
            break;
    }
}

void ladderEffect(struct LADDER &ladder){
    if (waitLower == 1 || waitUpper == 1){
        if (millis() - workTime > MAX_WORK_TIME && ladder.stripMode == 1){
            sensorCount = 0;
            waitLower = 0;
            waitUpper = 0;
            ladder.upperSensor = 0;
            ladder.lowerSensor = 0;
            workTime = 0;
            ladder.stripMode = 0;
            for (int current_step = 0; current_step < NUM_STEPS; current_step++){
                for (int i = current_step*AMOUNT; i < AMOUNT*(current_step + 1); i++)
                    ladder.leds[i] = CHSV(0, 0, 0);
            }
            for (int i = 0; i < LEDS_RAIL; i++)
                ladder.leds_rail[i] = CHSV(0, 0, 0);
            FastLED.show();
            Serial.println("switch to 0 mode by time");
        }
    }

    if (ladder.upperSensor == 0 && ladder.lowerSensor == 0)
        return;

    if (ladder.upperSensor == 1 && ladder.stripMode == 0){
        FastLED.setBrightness(ladder.bright);
        workTime = millis();
        
        for (int i = 0; i < LEDS_RAIL; i++)
            ladder.leds_rail[i] = CRGB::White;
        FastLED.show();

        for (int current_step = NUM_STEPS; current_step > 0; current_step--){
            for (int i = AMOUNT*current_step; i > AMOUNT*(current_step - 1); i--)
                ladder.leds[i] = CRGB::White;
            FastLED.show();
            delay(100);
        }
        waitLower = 1;
        Serial.println("upper");
        ladder.stripMode = 1;
        ladder.upperSensor = 0;
        sensorCount++;
    }
    if (ladder.lowerSensor == 1 && ladder.stripMode == 0){
        FastLED.setBrightness(ladder.bright);
        workTime = millis();

        for (int current_step = 0; current_step < NUM_STEPS; current_step++){
            for (int i = current_step*AMOUNT; i < AMOUNT*(current_step + 1); i++)
                ladder.leds[i] = CRGB::White;
            FastLED.show();
            delay(100);
        }
        for (int i = 0; i < LEDS_RAIL; i++) 
            ladder.leds_rail[i] = CRGB::White;
        FastLED.show();
        waitUpper = 1;
        Serial.println("lower");
        ladder.stripMode = 1;
        ladder.lowerSensor = 0;
        sensorCount++;
    }

    if (ladder.upperSensor == 1 && waitUpper == 1 && ladder.stripMode == 1 && millis() - workTime > 3000){
        for (int i = 0; i < LEDS_RAIL; i++)
            ladder.leds_rail[i] = CHSV(0, 0, 0);
        FastLED.show();
        for (int current_step = 0; current_step < NUM_STEPS; current_step++){
            for (int i = current_step*AMOUNT; i < AMOUNT*(current_step + 1); i++)
                ladder.leds[i] = CHSV(0, 0, 0); 
            FastLED.show();
            delay(100);
        }

        Serial.println("upper");
        ladder.stripMode = 0;
        ladder.upperSensor = 0;
        sensorCount++;
    }
    if (ladder.lowerSensor == 1 && waitLower == 1 && ladder.stripMode == 1 && millis() - workTime > 3000){
        for (int current_step = NUM_STEPS; current_step > 0; current_step--){
            for (int i = AMOUNT*current_step; i > AMOUNT*(current_step - 1); i--)
                ladder.leds[i] = CHSV(0, 0, 0); 
            FastLED.show();
            delay(100);
        }
        for (int i = 0; i < LEDS_RAIL; i++)
            ladder.leds_rail[i] = CHSV(0, 0, 0);
        
        FastLED.show();
        Serial.println("lower");
        ladder.stripMode = 0;
        ladder.lowerSensor = 0;
        sensorCount++;
    }
    // if (sensorCount == 2){
    //     sensorCount = 0;
    //     waitLower = 0;
    //     waitUpper = 0;
    //     workTime = 0;
    // }

}

