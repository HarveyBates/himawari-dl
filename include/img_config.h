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

#ifndef HIMAWARI_DL_IMG_CONFIG_H
#define HIMAWARI_DL_IMG_CONFIG_H

#include <curl/curl.h>
#include <stdlib.h>

typedef enum {
  RESOLUTION_VERY_LOW = 1,
  RESOLUTION_LOW = 4,        /* 4 x 4 = 16 images*/
  RESOLUTION_MEDIUM = 8,     /* 8 x 8 = 64 images */
  RESOLUTION_HIGH = 16,      /* 16 * 16 = 256 images */
  RESOLUTION_ULTRA_HIGH = 20 /* 20 x 20 = 400 images */
} IMGResolution;

uint16_t build_url(char (*urls)[90], uint16_t n_urls, size_t url_size,
                   IMGResolution res, struct tm* timestamp);

#endif /* HIMAWARI_DL_IMG_CONFIG_H */
