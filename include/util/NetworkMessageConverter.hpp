#ifndef NETWORKMESSAGECONVERTER_H
#define NETWORKMESSAGECONVERTER_H

#include <cstdint>

struct NetworkMessageConverter{
    static void convertFrom16To8(int8_t* dest, int16_t dataAll);
    static uint16_t convertFrom8To16(int8_t dataFirst, int8_t dataSecond);
    static uint32_t convertFrom8To32(uint8_t dataFirst, uint8_t dataSecond, uint8_t dataThird, uint8_t dataFourth);
};
#endif