#include <M5Core2.h>


#define LGFX_M5STACK_CORE2
#include <LovyanGFX.hpp>

static LGFX lcd;

union DataSend{
  struct 
  {
    int data;
    uint8_t flag;
    uint8_t padding[3];
  };
  uint8_t bin[8];
};

uint8_t cobs[sizeof(DataSend) + 2];

void cobs_encode(DataSend* source,uint8_t *encoded){
  uint8_t count =0;
  int marker = 0;

  for(int i=0; i< sizeof(*source) +1; i++){
    if( source->bin[i] != 0x00){
      encoded[i+1] = source->bin[i];
      count ++;
    }else{
      count ++;
      encoded[marker] = count;
      marker = i + 1;
      count = 0;
      encoded[marker] = count;
    }
  }
  encoded[marker] = count;
  encoded[sizeof(*source)+1]  = 0;
}

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
      Serial.println("hello");
    }

    if ( M5.BtnB.wasPressed()){
      send.data = 120;
      send.flag = 1;
      cobs_encode(&send, cobs);
      for( int i = 0; i < sizeof(cobs); i++){ Serial.write( cobs[i] ); }
    }

    if ( M5.BtnC.wasPressed()){
      send.data = 12000;
      send.flag = 1;
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