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
const uint8_t ledR{32};
// const uint8_t ledG{26};
// const uint8_t ledB{25};
//-------------------------------------------------------------------------------------------------------
PointLed led1(ledR, 0, arrObj, 10, 2450, 1000);
// PointLed led2(ledG, 1, arrObj, 1300, 3200, 100);
// PointLed led3(ledB, 2, arrObj, 1900, 3200);
// PointLed* arrLed[3]{&led1, &led2, &led3};
PointLed* arrLed[]{&led1};
Timer tButt_1(1000);
const int drDelay{250};
const int pauseDelay{2000};
int16_t ft_1_pause{0};
Timer tDrebezg_1(drDelay);
Timer tPause_1(pauseDelay);
//------------------------------------------------------------
bool statMove{false};
const int16_t pinBut1{34};
volatile int16_t buttonStatus_1{0}, ft_1{0}, ft_1_dr{0}, nClickBut_1{0};
//*********************************************
void IRAM_ATTR button_interr_1(){ //IRAM_ATTR
  detachInterrupt(pinBut1);
  ft_1_dr = 2;
  if(!ft_1){
    ft_1 = 3;
    nClickBut_1 = 1;
  } else if(ft_1 == 2){
    nClickBut_1++;
  }
}
//*********************************************************
void setup(void)
{
  pinMode(pinBut1, INPUT);
  Serial.begin(115200); //Feedback over Serial Monitor
  Serial.println(F("=========================== ptr =="));
  // radar.debug(Serial); //Uncomment to show debug information from the library on the Serial Monitor. By default this does not show sensor reads as they are very frequent.
  Serial2.begin (256000, SERIAL_8N1, 16, 17); //UART for monitoring the radar
  delay(500);
  attachInterrupt(digitalPinToInterrupt(pinBut1), button_interr_1, FALLING);
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
//********************************************************
void loop()
{
  if(tRadar.getTimer()){
    tRadar.setTimer();
    getRadar();
    float tmp = 0;
    for(auto& aa: arrLed){
      aa -> setDist(tmp);
    }
  }
//-------------------------------------------------------------------------------- BUTTONS
  if(ft_1 == 3){
    tButt_1.setTimer();
    ft_1 = 2;
  }
  if( ft_1_dr == 2 ){
    tDrebezg_1.setTimer();
    ft_1_dr = 1;
  }
  if( ( ft_1_dr == 1 ) && tDrebezg_1.getTimer() ){
    ft_1_dr = 0;
    attachInterrupt(digitalPinToInterrupt(pinBut1), button_interr_1, FALLING);
  }
  if( ( ft_1_pause == 1 ) && tPause_1.getTimer() ){
    ft_1_pause = 0;
    attachInterrupt(digitalPinToInterrupt(pinBut1), button_interr_1, FALLING);
  }

  if(ft_1 == 2 && tButt_1.getTimer()){
    int16_t i = digitalRead(pinBut1);
    // Serial.print(i == 0? "Long ": "Short ");
    // Serial.print("nCount = ");
    // Serial.println(nClickBut_1);
    ft_1 = 0;
    ft_1_pause = 1;
    tPause_1.setTimer();
    detachInterrupt(pinBut1);
    arrLed[0]->toogleMaxLevel();
    // light_1.clickBut(0, i, nClickBut_1);
  }
  arrLed[0]->cycle();
  for(auto& aa: arrLed){
    aa -> cycle();
  }
}