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

#ifndef HIMAWARI_DL_HTTP_UTILS_H
#define HIMAWARI_DL_HTTP_UTILS_H

#include <curl/curl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char* memory;
  size_t size;
} HTTP_Data_TypeDef;

struct thread_args {
  char* url;
  char* filename;
};

void* HTTP_GetImage(void* args);
CURLcode HTTP_GetClosestTimestamp(uint64_t unix_t_ms,
                                  HTTP_Data_TypeDef* response);
uint8_t HTTP_JSONToTime(char* json_memory, struct tm* timestamp);

#endif /* HIMAWARI_DL_HTTP_UTILS_H */
