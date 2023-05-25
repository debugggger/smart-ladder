#include "common.h"

#include "effects.h"
#include "sensors.h"

struct LADDER ladder;

int startTime = 0;
int lastSaveTime = 0;
uint32_t sec = 0; 

bool u1 = 0;
bool l1 = 0;

byte EEMEM effectsBrightAdr;
byte EEMEM speedRainbowAdr;

void setup() {
  startTime = millis()/1000ul;
  FastLED.addLeds<WS2811, PIN_LED, GRB>(ladder.leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2811, PIN_LED_RAIL, GRB>(ladder.leds_rail, LEDS_RAIL).setCorrection(TypicalLEDStrip);
  
  pinMode(PIN_PIR1, INPUT);
  pinMode(PIN_PIR2, INPUT);

  ladder.effectsBright = eeprom_read_byte(&effectsBrightAdr);
  ladder.speedRainbow = eeprom_read_byte(&speedRainbowAdr);

  ladder.bright = 5;
  FastLED.setBrightness(ladder.bright);
  ladder.stripMode = 0;

  Serial.begin(9600);  
}

void yield(){
  scanButton(ladder);
  if (ladder.stripMode == 0 || ladder.stripMode == 1){
    scanMove(ladder);
  }
  else{
    ladder.upperSensor = 0;
    ladder.lowerSensor = 0;

    for (int i = 0; i < LEDS_RAIL; i++) //выключение перил
        ladder.leds_rail[i] = CHSV(0, 0, 0);
    FastLED.show();
  }
}

void loop() {

  sec = millis() / 1000ul;   
  if ((ladder.stripMode == 0) && (sec-startTime > 3600)){
    Serial.println("reboot");
    delay(100);
    asm volatile("jmp 0x00");
  }

  if (sec-lastSaveTime == 60){
      lastSaveTime = millis() / 1000ul; 
      Serial.println("save settings");
      eeprom_update_byte(&effectsBrightAdr, ladder.effectsBright);
      eeprom_update_byte(&speedRainbowAdr, ladder.speedRainbow);
      delay(1000);
  }

  yield();
  //Serial.println(ladder.stripMode);

  defaultEffect(ladder);

  if (ladder.stripMode == 0) {
      if (scanPir(ladder))
          ladderEffect(ladder);
  }

  if (ladder.stripMode == 1) {
      ladderEffect(ladder);
  }


  if (sec == 5 && u1 == 0){
      ladder.upperSensor = 1;
      u1 = 1;
  }
  if (sec == 10 && l1 == 0){
      ladder.lowerSensor = 1;
      l1 = 1;
      u1 = 0;
  }

  if (sec == 15 && u1 == 0){
      ladder.lowerSensor = 1;
      u1 = 1;
      l1 = 0;
  }
  if (sec == 20 && l1 == 0){
      ladder.upperSensor = 1;
      l1 = 1;
      u1 = 0;
  }

}