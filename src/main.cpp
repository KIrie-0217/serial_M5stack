#include <M5Core2.h>


#define LGFX_M5STACK_CORE2
#include <LovyanGFX.hpp>
#include "./serial_data.h"

static LGFX lcd;


uint8_t cobs[sizeof(DataSend) + 2];

void setup(){
    M5.begin(true,true,true,true);
    Serial.begin(115200);

    lcd.init();
    lcd.setFont(&fonts::lgfxJapanGothic_28);
    lcd.setBrightness(128);
    lcd.setCursor(0,128);
    lcd.println("hello M5");
}

void loop(){

    DataSend send;

    M5.update();

    float batVol = M5.Axp.GetBatVoltage();
    float batCur = M5.Axp.GetBatCurrent();

    lcd.setCursor(0,156);
    lcd.printf("Battery: %1.3f\n Current: %.3f\n",batVol,batCur);

    if ( M5.BtnA.wasPressed()){
      send.data_short[0] = 120;
      send.data_short[1] = 220;
      send.flag = 1;
      send.type = types::SHORT;
      cobs_encode(&send, cobs);
      for( int i = 0; i < sizeof(cobs); i++){ Serial.write( cobs[i] ); }
    }

    if ( M5.BtnB.wasPressed()){
      send.data_int = 120;
      send.flag = 1;
      send.type = types::INT;
      cobs_encode(&send, cobs);
      for( int i = 0; i < sizeof(cobs); i++){ Serial.write( cobs[i] ); }
    }

    if ( M5.BtnC.wasPressed()){
      send.data_float = 12000.001F;
      send.flag = 1;
      send.type = types::FLOAT;
      cobs_encode(&send, cobs);
      for( int i = 0; i < sizeof(cobs); i++){ Serial.write( cobs[i]); }
    }

    if (batCur < 0){
        delay(500);
        lcd.setBrightness(0);
        lcd.sleep();
        delay(500);
        lcd.wakeup();
        lcd.setBrightness(128);
    }
}