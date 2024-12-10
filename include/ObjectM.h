//ObjectM
#ifndef ObjectM_H
#define ObjectM_H
#include <Arduino.h>

class ObjectM {
    int16_t x{0};
    int16_t y{0};
    int16_t s{0};   //speed
    int16_t l{0};   //distance
    int16_t gate{0};
    public:
        ObjectM(){};
        int16_t setParam(uint8_t* byteArray, int &i);
        int16_t getDist(){return l;}
};
#endif
