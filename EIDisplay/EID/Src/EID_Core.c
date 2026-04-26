#include <EID_Core.h>
#include "stm32f4xx_hal.h"
uint8_t EID_Buffer[4736];
void EID_WaitOnBusy(EID_Interface *eid){
	    while(HAL_GPIO_ReadPin(eid->portBUSY, eid->pinBUSY) == GPIO_PIN_SET) {
	    	HAL_Delay(1);
	    }
}
void EID_SendCmd(EID_Interface *eid, uint8_t command){
	HAL_GPIO_WritePin(eid->portDC, eid->pinDC, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(eid->portCS, eid->pinCS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(eid->hspi, &command, 1, HAL_MAX_DELAY);
	while(__HAL_SPI_GET_FLAG(eid->hspi, SPI_FLAG_BSY));
	HAL_GPIO_WritePin(eid->portCS, eid->pinCS, GPIO_PIN_SET);

}
void EID_SendData(EID_Interface *eid, uint8_t data){
	HAL_GPIO_WritePin(eid->portDC, eid->pinDC, GPIO_PIN_SET);
	HAL_GPIO_WritePin(eid->portCS, eid->pinCS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(eid->hspi, &data, 1, HAL_MAX_DELAY);
	while(!__HAL_SPI_GET_FLAG(eid->hspi, SPI_FLAG_TXE));
	while(__HAL_SPI_GET_FLAG(eid->hspi, SPI_FLAG_BSY));

	HAL_GPIO_WritePin(eid->portCS, eid->pinCS, GPIO_PIN_SET);

}
void EID_Init(EID_Interface *eid) {
    HAL_GPIO_WritePin(eid->portRST, eid->pinRST, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(eid->portRST, eid->pinRST, GPIO_PIN_SET);
    HAL_Delay(10);

    EID_SendCmd(eid, 0x12); // Software Reset
    EID_WaitOnBusy(eid);

    EID_SendCmd(eid, 0x01);
    EID_SendData(eid, 0x27);
    EID_SendData(eid, 0x01);
    EID_SendData(eid, 0x01);
    //Y- X+
    EID_SendCmd(eid, 0x11);
    EID_SendData(eid, 0x01);
    // Set RAM X
    EID_SendCmd(eid, 0x44);
    EID_SendData(eid, 0x01);
    EID_SendData(eid, 0x10); //1-16 width
    // Write VCOM Register
    EID_SendCmd(eid, 0x2C);
    EID_SendData(eid, 0x36);
    // Set RAM Y (295 do 0)a
    EID_SendCmd(eid, 0x45);
    EID_SendData(eid, 0x27);
    EID_SendData(eid, 0x01);
    EID_SendData(eid, 0x00);
    EID_SendData(eid, 0x00);
    // Border Waveform
    EID_SendCmd(eid, 0x3C);
    EID_SendData(eid, 0x01);
    // Display Update
    EID_SendCmd(eid, 0x22);
    EID_SendData(eid, 0xB1);
    EID_SendCmd(eid, 0x20);
    EID_WaitOnBusy(eid);
}
void EID_CursorBegin(EID_Interface *eid) {
    EID_SendCmd(eid, 0x4E); // Set RAM X address counter [cite: 551]
    EID_SendData(eid, 0x00);

    EID_SendCmd(eid, 0x4F); // Set RAM Y address counter [cite: 552]
    EID_SendData(eid, 0x27);
    EID_SendData(eid, 0x01);
}

void EID_Clear(EID_Interface *eid){
	EID_CursorBegin(eid);
	    EID_SendCmd(eid, 0x24);
	    HAL_GPIO_WritePin(eid->portDC, eid->pinDC, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(eid->portCS, eid->pinCS, GPIO_PIN_RESET);
	    uint8_t white = 0xFF;
	    for(uint16_t i=0; i<4737; i++) {
	        HAL_SPI_Transmit(eid->hspi, &white, 1, HAL_MAX_DELAY);
	    }

	    while(__HAL_SPI_GET_FLAG(eid->hspi, SPI_FLAG_BSY));
	    HAL_GPIO_WritePin(eid->portCS, eid->pinCS, GPIO_PIN_SET);

	    EID_SendCmd(eid, 0x22);
	    EID_SendData(eid, 0xC7); // Full Update sequence
	    HAL_Delay(100);
	    EID_SendCmd(eid, 0x20);
	    EID_WaitOnBusy(eid);

}

