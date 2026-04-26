#ifndef EID_CORE_H
#define EID_CORE_H
#include <stdint.h>
#include "stm32f4xx_hal.h"

#define SCREEN_W 296
#define SCREEN_H 128

extern uint8_t EID_Buffer[4736];
typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *portCS;
    uint16_t      pinCS;
    GPIO_TypeDef *portDC;
    uint16_t      pinDC;
    GPIO_TypeDef *portRST;
    uint16_t      pinRST;
    GPIO_TypeDef *portBUSY;
    uint16_t      pinBUSY;
} EID_Interface;

void EID_Init(EID_Interface *eid);
void EID_Clear(EID_Interface *eid);
void EID_SendCmd(EID_Interface *eid, uint8_t command);
void EID_SendData(EID_Interface *eid, uint8_t data);
void EID_WaitOnBusy(EID_Interface *eid);
void EID_CursorBegin(EID_Interface *eid);

#endif
