#include<Arduino.h>
#include "Timer.h"
#include "ObjectM.h"
#include "PointLed.h"

/*
Датчик присутствия
*/
ObjectM arrObj[3];
Timer tRadar(100);

//-----------------------------------------------------
int16_t dist{0};
int16_t distNew{0};

bool led1_On{true};
bool led2_On{true};
const int PinOutLd{18};
const int PinOutPres{19};
const int PinOutDis{5};
Timer t1(750);
Timer tLed1(3000);
Timer tLed2(3000);
bool ledStat{true};
uint16_t dt{250};
enum class Motion : uint8_t {
  None,
  Far,
  Near,
} motion;
//------------------------------------------------------------------------------------------------------------
//TODO: добавить кнопку для включения максимального света, повторное нажатие отключает максимальный свет.
//выход из зоны также отключает максимальный свет.
//------------------------------------------------------------------------------------------------------------
const uint8_t ledR{27};
const uint8_t ledG{26};
const uint8_t ledB{25};
//-------------------------------------------------------------------------------------------------------
PointLed led1(ledR, 0, arrObj, 10, 1450, 1000);
PointLed led2(ledG, 1, arrObj, 1300, 3200, 100);
// PointLed led3(ledB, 2, arrObj, 1900, 3200);
// PointLed* arrLed[3]{&led1, &led2, &led3};
PointLed* arrLed[]{&led1, &led2};
//*********************************************************
void setup(void)
{
  pinMode(PinOutPres, OUTPUT);
  pinMode(PinOutDis, OUTPUT);
  pinMode(PinOutLd, OUTPUT);
  Serial.begin(115200); //Feedback over Serial Monitor
  Serial.println(F("=========================== ptr =="));
  // radar.debug(Serial); //Uncomment to show debug information from the library on the Serial Monitor. By default this does not show sensor reads as they are very frequent.
  Serial2.begin (256000, SERIAL_8N1, 16, 17); //UART for monitoring the radar
  delay(500);
}
//------------------------------------------------------------------------------------------------------------
void outByte(uint8_t n){
  if(n < 10)
    Serial.print("0");
  Serial.print(n, HEX);
}
//------------------------------------------------------------------------------------------------------------
void outHexArray(uint8_t *byteArray, int size = 26){
  for (int i = 0; i < size; i++) {
    outByte(byteArray[i]);
    if(i % 2 > 0)
      Serial.print(" ");
  }
  Serial.println();
}
//------------------------------------------------------------------------------------------------------------
int getRadar(){
  int i = 0;
  while(Serial2.available()){
    if (Serial2.read()==0xAA && Serial2.read()==0xFF) {
      uint8_t byteArray[28];
      int nBytes = Serial2.readBytes(byteArray,28);
      i = 2;
      // outHexArray(byteArray, nBytes- 2);
      uint16_t m{0};
      for(auto& obj: arrObj){
        obj.setParam(byteArray, i);
        // arrAlarm[m++] -> setDuration(obj.setParam(byteArray, i));
      }
    }
  }
  return i;
}
//----------------------------------------------------------
void controlLed(Motion motion) {
  switch (motion) {
    case Motion::None:
      digitalWrite(PinOutLd, LOW);
      digitalWrite(PinOutPres, LOW);
      break;
    case Motion::Near:
      digitalWrite(PinOutLd, HIGH);
      digitalWrite(PinOutPres, LOW);
      break;
    case Motion::Far:
      digitalWrite(PinOutLd, LOW);
      digitalWrite(PinOutPres, HIGH);
      break;
  }
}
//********************************************************
void loop()
{
  if(tRadar.getTimer()){
    tRadar.setTimer();
    getRadar();
    // statMove = false;
    // for(auto& aa: arrLed){
    //   statMove |= !aa -> setDist(lux);
    // }
  }
}