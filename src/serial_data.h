#include <iostream>

enum types :uint8_t{
  INT,
  FLOAT,
  CHAR,
  SHORT,
};

union DataSend{
  struct 
  {
    union{
      int data_int;
      float data_float;
      char data_char[4];
      short data_short[2];
    };
    uint8_t flag;
    uint8_t type;
    uint8_t padding[2];
  };
  uint8_t bin[8];
};


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
