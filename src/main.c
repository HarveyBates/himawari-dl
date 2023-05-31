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

#include "http_utils.h"
#include "img_config.h"
#include "pthread.h"

int main(int argc, char **argv) {
  char urls[16][100];
  pthread_t tid[16];
  uint16_t allocated_urls, i;
  struct tm time_now;
  HTTP_Data_TypeDef request;
  struct thread_args args[16];
  char filename[16][30];
  int error;

  /* Setup CURL */
  if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
    return 0;
  }

  HTTP_GetClosestTimestamp(time(NULL), &request);

  if (request.memory) {
    printf("URL: %s\n", request.memory);
  }

  if (!HTTP_JSONToTime(request.memory, &time_now)) {
    fprintf(stderr, "Time conversion failed\n");
  }

  allocated_urls = build_url(urls, 16, 100, RESOLUTION_LOW, &time_now);
  for (i = 0; i < allocated_urls; i++) {
    snprintf(filename[i], sizeof(filename[i]), "imgs/img_%d.png", i + 1);
    args[i].url = urls[i];
    args[i].filename = filename[i];
    error = pthread_create(&tid[i], NULL, HTTP_GetImage, (void *)&args[i]);
    if (0 != error) {
      fprintf(stdout, "Error running thread (%d), errno %d\n", i, error);
    }
  }

  /* Rejoin threads */
  for (i = 0; i < allocated_urls; i++) {
    pthread_join(tid[i], NULL);
  }

  /* Memory cleanup */
  curl_global_cleanup();
  free(request.memory);

  return 0;
}
