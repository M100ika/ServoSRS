#include "SCSCL.h"

// Конструктор
SCSCL::SCSCL() {
    pSerial = nullptr;
}

// Инициализация с указанием последовательного порта
void SCSCL::begin(Stream &serial) {
    pSerial = &serial;
}

// Метод для преобразования данных в формат, подходящий для сервопривода
void SCSCL::Host2SCS(u8* DataL, u8* DataH, u16 Data) {
    *DataL = Data & 0xFF;
    *DataH = (Data >> 8) & 0xFF;
}

// Основной метод записи данных в серво
int SCSCL::genWrite(u8 ID, u8 MemAddr, u8* nDat, u8 nLen) {
    if (pSerial == nullptr) return -1;

    u8 msgLen = 2 + nLen + 2;
    u8 checksum = 0;
    u8 bBuf[msgLen + 4];
    bBuf[0] = 0xFF;
    bBuf[1] = 0xFF;
    bBuf[2] = ID;
    bBuf[3] = msgLen;
    bBuf[4] = 0x03;
    bBuf[5] = MemAddr;

    for (u8 i = 0; i < nLen; i++) {
        bBuf[6 + i] = nDat[i];
    }

    for (u8 i = 2; i < msgLen + 2; i++) {
        checksum += bBuf[i];
    }

    bBuf[msgLen + 2] = ~checksum;
    pSerial->write(bBuf, msgLen + 3);

    return 1;
}

// Переключение в PWM режим
int SCSCL::PWMMode(u8 ID) {
    u8 bBuf[4] = {0, 0, 0, 0};
    return genWrite(ID, 0x08, bBuf, 4);  // Указание адреса и данных для PWM режима
}

// Запись значений PWM
int SCSCL::WritePWM(u8 ID, s16 pwmOut) {
    if (pwmOut < 0) {
        pwmOut = -pwmOut;
        pwmOut |= (1 << 10);
    }
    u8 bBuf[2];
    Host2SCS(bBuf, bBuf + 1, pwmOut);
    return genWrite(ID, 0x2A, bBuf, 2);  // Указание адреса и данных для записи PWM
}
