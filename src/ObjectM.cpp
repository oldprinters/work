#include "ObjectM.h"

//------------------------------------------------------------------------------
int16_t ObjectM::setParam(uint8_t* byteArray, int &i){
  bool sign{0};
  x = (byteArray[i++] + 256 * byteArray[i++]);
  if (x && 0x8000){
    sign  =  1;
    x &= 0x7fff;
  }
  y = (byteArray[i++] + 256 * byteArray[i++]);
  if (y && 0x8000){
    sign  =  1;
    y &= 0x7fff;
  } else sign = 0;
  s = byteArray[i++] + 256 * byteArray[i++];
  if (s && 0x8000){
    sign  =  1;
    s &= 0x7fff;
  } else sign = 0;
  gate = byteArray[i++] + 256 * byteArray[i++];
  l = sqrt(pow(x, 2) + pow(y, 2));
  // Serial.print(l);
  // Serial.print(" \t");
  // Serial.println(gate);
  return l;
}
