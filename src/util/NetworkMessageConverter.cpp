#include "../../include/util/NetworkMessageConverter.hpp"

uint16_t NetworkMessageConverter::convertFrom8To16(int8_t dataFirst, int8_t dataSecond) {
    uint16_t dataBoth = 0x0000;

    dataBoth = dataFirst;
    dataBoth = dataBoth << 8;
    dataBoth |= dataSecond;
    return dataBoth;
}

void NetworkMessageConverter::convertFrom16To8(int8_t* dest, int16_t dataAll) {
    *(dest) = (dataAll >> 8) & 0x00FF;
    dest[1] = dataAll & 0x00FF;
}

uint32_t NetworkMessageConverter::convertFrom8To32(uint8_t dataFirst, uint8_t dataSecond, uint8_t dataThird, uint8_t dataFourth){
    uint32_t dataResult = 0x0000;

    dataResult = dataFirst;
    dataResult <<= 8;
    dataResult |= dataSecond; 
    dataResult <<= 8;
    dataResult |= dataThird; 
    dataResult <<= 8;
    dataResult |= dataFourth; 

    return dataResult;
}
