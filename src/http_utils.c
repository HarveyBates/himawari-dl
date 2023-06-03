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

const char* USER_AGENT = "HIMAWARI-DL";

void* HTTP_GetImage(void* arguments) {
  CURLcode result;
  FILE* file;
  struct thread_args* args = arguments;

  CURL* curl = curl_easy_init();
  if (!curl) {
    printf("curl failed\n");
    return NULL;
  }

  /* Open a file to write to */
  file = fopen(args->filename, "wb");
  if (!file) {
    printf("error opening file\n");
    return NULL;
  }

  /* Setup CURL request */
  curl_easy_setopt(curl, CURLOPT_URL, args->url);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
  curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

  result = curl_easy_perform(curl);

  if (result != CURLE_OK) {
    printf("error dl\n");
    fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(result));
  }

  curl_easy_cleanup(curl);
  fclose(file);

  return NULL;
}

/**
 * Allocate memory from HTTP request.
 *
 * @see https://curl.se/libcurl/c/getinmemory.html
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 * @return
 */
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb,
                                  void* userp) {
  size_t realsize = size * nmemb;
  HTTP_Data_TypeDef* mem = (HTTP_Data_TypeDef*)userp;

  char* ptr = realloc(mem->memory, mem->size + realsize + 1);

  if (!ptr) {
    fprintf(stderr, "Memory allocation error.\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

CURLcode HTTP_GetClosestTimestamp(uint64_t unix_t_ms,
                                  HTTP_Data_TypeDef* response) {
  char url[80];
  const char* TIMESTAMP_BASE_URL =
      "https://ncthmwrwbtst.cr.chiba-u.ac.jp/img/D531106/latest.json?_=";
  CURLcode result;
  CURL* curl;

  /* Build the URL */
  snprintf(url, sizeof(url), "%s%llu", TIMESTAMP_BASE_URL, unix_t_ms);

  curl = curl_easy_init();

  if (!curl) {
    return CURLE_FAILED_INIT;
  }

  /* Setup CURL request */
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
  curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

  /* malloc first then realloc above in callback */
  response->memory = malloc(1 * sizeof(char));
  response->size = 0;

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

  result = curl_easy_perform(curl);

  if (result != CURLE_OK) {
    fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(result));
  }

  curl_easy_cleanup(curl);

  return result;
}

/**
 * @brief Converts the raw latest image JSON response into a struct tm.
 *
 * Example input: {"date":"2023-05-28 02:50:00"...
 *
 * @param json_memory JSON response from HTTP request.
 */
uint8_t HTTP_JSONToTime(char* json_memory, struct tm* timestamp) {
  if (!strptime(json_memory, "{\"date\":\"%Y-%m-%d %H:%M:%S", timestamp)) {
    fprintf(stderr, "Time conversion failed\n");
    return 0;
  }
  return 1;
}
