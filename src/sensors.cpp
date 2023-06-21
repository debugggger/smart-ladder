#include "sensors.h"

bool pressFlag = false;
unsigned long pressTime = 0;
unsigned long releaseTime = 0;

bool pirFlagLow = false;
unsigned long pirTimeLow = 0;

bool pirFlagUp = false;
unsigned long pirTimeUp = 0;


enum BUTTON_PRESS{
    empty,
    shortPress,
    longPress
} buttonPress;

bool brightDir = 0;

void scanButton(struct LADDER &ladder){
    if (digitalRead(BUTTON_PIN)){
        if (pressFlag == false){
            pressTime = millis();
            pressFlag = true;
        }
        if (millis() - pressTime >= 500){
            // buttonPress = BUTTON_PRESS::longPress;
            //  Serial.println("long button");

            if (ladder.stripMode == 1){
                if (brightDir == 0){
                    ladder.effectsBright = ladder.effectsBright - 1;
                    if (ladder.effectsBright < 10)
                        ladder.effectsBright = 10;
                    FastLED.setBrightness(ladder.effectsBright);
                    FastLED.show();
                }
                if (brightDir == 1){
                    ladder.effectsBright = ladder.effectsBright + 1;
                    if (ladder.effectsBright > 255)
                        ladder.effectsBright = 255;
                    FastLED.setBrightness(ladder.effectsBright);
                    FastLED.show();
                }
            }

            if (ladder.stripMode == 2 || ladder.stripMode == 3){
                if (brightDir == 0){
                    ladder.speedRainbow -= 1;
                    if (ladder.speedRainbow <= 0)
                        ladder.speedRainbow = 1;
                }
                if (brightDir == 1){
                    ladder.speedRainbow += 1;
                    if (ladder.speedRainbow >= 51)
                        ladder.speedRainbow = 50;
                }
            }
        }
    }

    if (!digitalRead(BUTTON_PIN) && pressFlag == true){
        if (millis() - pressTime < 500 && millis() - pressTime > 50){
             buttonPress = BUTTON_PRESS::shortPress;
             Serial.println("short button");
        }
        brightDir = !brightDir;
        
        pressFlag = false;
        if (buttonPress != BUTTON_PRESS::empty)
            switchMode(ladder);
    }
}

void switchMode(struct LADDER &ladder){
    if (buttonPress == BUTTON_PRESS::longPress){
        
        
    }

    if (buttonPress == BUTTON_PRESS::shortPress){
        ladder.stripMode++;
        if (ladder.stripMode > MAX_MODE_NUM)
            ladder.stripMode = 0;
    }
    buttonPress = BUTTON_PRESS::empty;
}

bool scanPir(struct LADDER &ladder){
    ladder.pirV = analogRead(LDR_PIN);
    Serial.print("pirV = ");
    Serial.println(ladder.pirV);
    if (ladder.pirV <= MLBTW){
        ladder.bright = ladder.pirV/4;
        if (ladder.bright > 255)
            ladder.bright = 255;
        
        if (ladder.bright <= 10)
            ladder.bright = 10;
        
        return true;
    }
    else {
        ladder.stripMode = 0;
        return false;
    }

}

void scanMove(struct LADDER &ladder){
    
    if (digitalRead(PIN_PIR1) == LOW){
        if (pirFlagLow == false){
            pirTimeLow = millis();
            pirFlagLow = true;
            Serial.println("low pir");
        }
    }

    if (pirFlagLow == true && millis()-pirTimeLow >= 10 ){
        ladder.upperSensor = 0;
        ladder.lowerSensor = 1;
        pirFlagLow = false;
    }

    if (digitalRead(PIN_PIR2) == LOW){
        if (pirFlagUp == false){
            pirTimeUp = millis();
            pirFlagUp = true;
            Serial.println("up pir");
        }
    }
    if (pirFlagUp == true && millis()-pirTimeUp >= 10){
        ladder.lowerSensor = 0;
        ladder.upperSensor = 1;
        pirFlagUp = false;
    }
}
