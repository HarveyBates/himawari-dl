/*******************************************************************************
 *  MIT License
 *
 * Copyright (c) 2023 Harvey Bates
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#ifndef HIMAWARI_DL_IMG_PROCESSING_H
#define HIMAWARI_DL_IMG_PROCESSING_H

#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char filename[128];
  png_bytep* prow;
  png_uint_32 width, height;
  uint32_t grid_rows, grid_columns;
  png_byte bit_depth;
  png_byte color_type;
} Image;

typedef struct {
  uint16_t width;
  uint16_t height;
  uint8_t grid_rows;
  uint8_t grid_columns;
} ImageDimensions;

void read_png(const char* filename, Image* image);
void join_images(const char* filename, Image images[][4]);

ImageDimensions calculate_dimensions(Image* image, uint8_t grid_rows,
                                     uint8_t grid_columns);

#endif  // HIMAWARI_DL_IMG_PROCESSING_H
