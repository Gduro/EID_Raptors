#include <EID_Core.h>
#include <EID_Bitmap.h>
#include <stdlib.h>
/*
 While using image2cpp converter, remember about these settings:
 - !Canvas size !- set canvas size same as SCREEN_W and SCREEN_H
 -Dithering:
 	 -Logo - use binary mode
 	 -pictures - use Floyd-Steinberg
 	 -other- adjust as you think is good :)
 - !Flip image! - set it on: Vertically
 - !Draw Mode! - Horizontal- 1bit per pixel
 */
void EID_AddImgToBuffer(uint16_t x_pos, uint16_t y_pos, const uint8_t *img,uint16_t img_w, uint16_t img_h)
{
	for(int i=0;i<4736;i++){
		EID_Buffer[i]=0xFF;
	}
	uint16_t img_bytes_per_row = (img_w + 7) / 8;

	    for(uint16_t y = 0; y < img_h; y++) {
	        for(uint16_t x = 0; x < img_w; x++) {

	            uint8_t bit = (img[y * img_bytes_per_row + (x / 8)] & (0x80 >> (x % 8)));

	            if(!bit) {
	                uint16_t screen_x = x + x_pos;
	                uint16_t screen_y = y + y_pos;

	                if (screen_x < 296 && screen_y < 128) {
	                    // Jeśli lewa strona jest ucięta, spróbuj odwrócić screen_y:
	                    uint16_t flipped_y = 127 - screen_y;

	                    uint16_t buffer_idx = (flipped_y / 8) + (screen_x * 16);
	                    EID_Buffer[buffer_idx] &= ~(0x80 >> (flipped_y % 8));
	                }
	            }
	        }
	    }
}
void EID_DrawBitmap(EID_Interface *eid) {
    EID_CursorBegin(eid);
    EID_SendCmd(eid, 0x24);

    HAL_GPIO_WritePin(eid->portDC, eid->pinDC, GPIO_PIN_SET);
    HAL_GPIO_WritePin(eid->portCS, eid->pinCS, GPIO_PIN_RESET);

    HAL_SPI_Transmit(eid->hspi, (uint8_t*)EID_Buffer, 4736, HAL_MAX_DELAY);

    while(__HAL_SPI_GET_FLAG(eid->hspi, SPI_FLAG_BSY));
    HAL_GPIO_WritePin(eid->portCS, eid->pinCS, GPIO_PIN_SET);

    EID_SendCmd(eid, 0x22);
    EID_SendData(eid, 0xF7);
    EID_SendCmd(eid, 0x20);
    EID_WaitOnBusy(eid);
}
const tImage *EID_GetChar(const tFont *font, char c)
{
	for(int i=0; i<font->length;i++)
	{
		if((long int)c==font->chars[i].code)
		{
			return font->chars[i].image;
		}

	}
	return 0;
};
void EID_StrToChar(char str[])
{
	int len=0;
	while(str[len]!='\0')
	{
		len++;
	}
	tImage *charList=malloc(len*sizeof(tImage));

	for(int i=0;i<len; i++)
	{
		tImage * ptrChar=EID_GetChar(&Font, str[i]);
	}
}
