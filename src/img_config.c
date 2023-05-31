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

#include "img_config.h"

const char* IMG_BASE_URL = "https://ncthmwrwbtst.cr.chiba-u.ac.jp/img/D531106";

uint16_t build_url(char (*urls)[100], uint16_t n_urls, size_t url_size,
                   IMGResolution res, struct tm* timestamp) {
  uint16_t row = 0, column, index = 0;
  char fmt_time[18];

  /* Ensure the buffer has enough space to hold all the URLS */
  if (n_urls > (res * res)) {
    printf("%d %d\n", res * res, n_urls);
    return 0;
  }

  strftime(fmt_time, sizeof(fmt_time), "%Y/%m/%d/%H%M%S", timestamp);

  for (; row < (uint8_t)res; row++) {
    column = 0;
    for (; column < (uint8_t)res; column++) {
      snprintf(urls[index], url_size, "%s/%dd/550/%s_%d_%d.png", IMG_BASE_URL,
               res, fmt_time, row, column);
      index++;
    }
  }

  return index;
}
