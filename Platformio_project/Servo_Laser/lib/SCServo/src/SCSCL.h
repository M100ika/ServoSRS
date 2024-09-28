
#ifndef _SCSCL_H
#define _SCSCL_H

#include <Arduino.h>

#define u8  uint8_t
#define s16 int16_t

class SCSCL {
public:
    SCSCL();  // Конструктор
    void begin(Stream &serial);  // Инициализация с указанием последовательного порта
    int PWMMode(u8 ID);  // Переключение в режим PWM
    int WritePWM(u8 ID, s16 pwmOut);  // Запись PWM значений

private:
    Stream* pSerial;  // Указатель на последовательный порт для связи с сервоприводом

    void Host2SCS(u8* DataL, u8* DataH, u16 Data);
    int genWrite(u8 ID, u8 MemAddr, u8* nDat, u8 nLen);
};

#endif 