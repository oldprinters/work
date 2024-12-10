//PointLed.h
#ifndef POINTLED_H
#define POINTLED_H
#include "Arduino.h"
#include "OneLed.h"
#include "ObjectM.h"
#include "Timer.h"

class PointLed : public OneLed {
        ObjectM* arrObj;
        int16_t distL{0};  //ближняя дистанция
        int16_t distH{1000};  //дальняя
        Timer* timer;
        Timer* timerExt;
        bool maxLevelAuto{false};
        bool maxLevelOn{false};
        float thresholdLight{1000};     //порог освещённости включения подсветки
    public:
        PointLed(uint8_t pin, uint8_t ch, ObjectM* arrO, int16_t l, int16_t h, float tLight);
        ~PointLed(){delete timer;delete timerExt;}
        bool setDist(float& l);
        void setMaxLevelAuto(){maxLevelAuto = true;}
        void setMaxLevelOn(){maxLevelOn = true;}
        void setMaxLevelOff(){maxLevelOn = false;}
        void extOn(){timerExt->setTimer();}       //включение подсветки по внешней команде
        bool cycle();
};
#endif
