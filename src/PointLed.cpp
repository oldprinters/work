//PointLed.cpp
#include "PointLed.h"

//---------------------------------------------------------------------------
PointLed::PointLed(uint8_t pin, uint8_t ch, ObjectM* arrO, int16_t l, int16_t h, float tLight = 1000):OneLed(pin, ch), arrObj(arrO), distL(l), distH(h), thresholdLight(tLight){
    timer = new Timer(5000);
    timerExt = new Timer(5000);
};

//---------------------------------------------------------------------------
bool PointLed::setDist(float& l){
    bool stat{false};
    for(int i=0; i < 3; ++i){
        int16_t l = arrObj[i].getDist();
        stat |= (distH >= l) && (distL < l);
    }
    if(stat && (l < thresholdLight) ) {
        timer->setTimer();
    }
    return timer->getTimer();
}
//--------------------------------------------------------------------------------------
bool PointLed::cycle(){
    if(!timer->getTimer() || maxLevelOn){
        OneLed::setStat(StatLed::ON);
        if ( maxLevelAuto  || maxLevelOn) {
            OneLed::setMaxLevel();
        } else {
            OneLed::setMediumLevel();
        }
    } else if(!timerExt->getTimer()){
        OneLed::setLowLevel();
    } else {
        OneLed::setOff();   //выключаем
        maxLevelAuto = false;
    }
    return OneLed::cycle(); //возвращает true, если достигнута заданная яркость
}
//--------------------------------------------------------------------------------------
// bool PointLed(){
// }       //включение подсветки по внешней команде
