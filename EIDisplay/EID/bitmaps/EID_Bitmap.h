/*
 * EID_Bitmap.h
 *
 *  Created on: Mar 31, 2026
 *      Author: grzeg
 */

#ifndef BITMAPS_EID_BITMAP_H_
#define BITMAPS_EID_BITMAP_H_
#include <stdint.h>

typedef struct {
     const uint8_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;
typedef struct {
     long int code;
     const tImage *image;
     } tChar;
typedef struct {
     int length;
     const tChar *chars;
} tFont;


extern const char heart[];
extern const char cat[];
extern const char rap_logo[];
extern const tFont Font;

#endif /* BITMAPS_EID_BITMAP_H_ */
