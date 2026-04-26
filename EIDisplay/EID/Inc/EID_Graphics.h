/*
 * EID_Graphics.h
 *
 *  Created on: Mar 31, 2026
 *      Author: grzeg
 */

#ifndef INC_EID_GRAPHICS_H_
#define INC_EID_GRAPHICS_H_
#include <stdint.h>

#include "stm32f4xx_hal.h"
void EID_AddImgToBuffer(uint16_t x_pos, uint16_t y_pos, const uint8_t *img,uint16_t img_w, uint16_t img_h);
void EID_DrawBitmap(EID_Interface *eid);
long int *EID_GetChar(const tFont *font, char c);
void EID_StrToChar(char str[]);

#endif /* INC_EID_GRAPHICS_H_ */
