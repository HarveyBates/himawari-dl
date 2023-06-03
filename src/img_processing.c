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

#include "img_processing.h"

void read_png(const char* filename, Image* image) {
  FILE* pfile;
  png_structp ppng;
  png_infop pinfo;
  png_uint_32 y;

  image->prow = NULL;
  image->width = 0;
  image->height = 0;

  pfile = fopen(filename, "rb");

  /* Check file accessible */
  if (!pfile) {
    fprintf(stderr, "File %s could not be found, or opened.\n", filename);
    return;
  }

  /* Initialise png */
  ppng = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!ppng) {
    fprintf(stderr, "Error creating png struct.\n");
    png_destroy_read_struct(&ppng, NULL, NULL);
    fclose(pfile);
    return;
  }

  /* Create png information structure */
  pinfo = png_create_info_struct(ppng);
  if (!pinfo) {
    fprintf(stderr, "Error creating info struct.\n");
    png_destroy_read_struct(&ppng, NULL, NULL);
    fclose(pfile);
    return;
  }

  if (setjmp(png_jmpbuf(ppng))) {
    fprintf(stderr, "Error during initialisation.\n");
    png_destroy_read_struct(&ppng, &pinfo, NULL);
    fclose(pfile);
    return;
  }

  /* Attach image ptr to file */
  png_init_io(ppng, pfile);

  /* Get image metadata */
  png_read_info(ppng, pinfo);

  /* Get image width and height in pixels */
  image->width = png_get_image_width(ppng, pinfo);
  image->height = png_get_image_height(ppng, pinfo);

  /* Set all read images to RGB (prevents artefacts) */
  png_set_gray_to_rgb(ppng);

  /* Update png information */
  png_read_update_info(ppng, pinfo);

  image->bit_depth = png_get_bit_depth(ppng, pinfo);
  image->color_type = png_get_color_type(ppng, pinfo);

  /* Allocate row memory */
  image->prow = (png_bytep*)malloc(sizeof(png_bytep) * (image->height));
  if (!image->prow) {
    fprintf(stderr, "Allocation failure\n");
    goto cleanup;
  }

  for (y = 0; y < image->height; y++) {
    image->prow[y] = (png_byte*)malloc(png_get_rowbytes(ppng, pinfo));
    if (!image->prow[y]) {
      fprintf(stderr, "Allocation failure\n");
      goto cleanup;
    }
  }

  /* Read image data */
  png_read_image(ppng, image->prow);

  /* Clean up */
cleanup:
  png_free_data(ppng, pinfo, PNG_FREE_ALL, -1);
  png_destroy_read_struct(&ppng, &pinfo, NULL);
  fclose(pfile);
}

void write_png(const char* filename, Image* image) {
  FILE* pfile;
  png_structp ppng;
  png_infop pinfo;
  png_uint_32 y;

  pfile = fopen(filename, "wb");

  /* Check file accessible */
  if (!pfile) {
    fprintf(stderr, "File %s could not be found, or opened.\n", filename);
    return;
  }

  ppng = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!ppng) {
    fprintf(stderr, "Error creating png write struct.\n");
    png_destroy_write_struct(&ppng, NULL);
    fclose(pfile);
    return;
  }

  pinfo = png_create_info_struct(ppng);
  if (!pinfo) {
    fprintf(stderr, "Error creating info struct.\n");
    png_destroy_write_struct(&ppng, NULL);
    fclose(pfile);
    return;
  }

  if (setjmp(png_jmpbuf(ppng))) {
    fprintf(stderr, "Error on setup.\n");
    png_destroy_write_struct(&ppng, &pinfo);
    fclose(pfile);
    return;
  }

  /* Attach image ptr to file */
  png_init_io(ppng, pfile);

  png_set_IHDR(ppng, pinfo, image->width, image->height, image->bit_depth,
               image->color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);

  /* Write pixels to output image */
  png_write_info(ppng, pinfo);
  png_write_image(ppng, image->prow);
  png_write_end(ppng, NULL);

  /* Clean up */
  for (y = 0; y < image->height; y++) {
    free(image->prow[y]);
  }
  free(image->prow);

  png_free_data(ppng, pinfo, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&ppng, &pinfo);
  fclose(pfile);
}

ImageDimensions calculate_dimensions(Image* image, uint8_t grid_rows,
                                     uint8_t grid_columns) {
  ImageDimensions dims;
  dims.width = image->width * grid_rows;
  dims.height = image->height * grid_columns;
  dims.grid_rows = grid_rows;
  dims.grid_columns = grid_columns;
  return dims;
}

void join_images(const char* filename, Image images[][4]) {
  Image out_image;
  uint32_t x, y, row, column, colour;
  uint32_t pixel_x, pixel_y;
  ImageDimensions dims;

  dims = calculate_dimensions(&images[0][0], 4, 4);
  out_image.width = dims.width;
  out_image.height = dims.height;
  out_image.bit_depth = images[0][0].bit_depth;
  out_image.color_type = images[0][0].color_type;

  /* Allocate space for final image */
  out_image.prow = (png_bytep*)malloc(sizeof(png_bytep) * (out_image.height));
  if (!out_image.prow) {
    fprintf(stderr, "Allocation failure\n");
  }

  for (y = 0; y < out_image.height; y++) {
    /* Three bytes per pixel (RGB) */
    out_image.prow[y] =
        (png_byte*)malloc((sizeof(png_byte) * out_image.width) * 3);
    if (!out_image.prow[y]) {
      fprintf(stderr, "Allocation failure\n");
    }
  }

  out_image.grid_rows = 4;
  out_image.grid_columns = 4;

  /* Get the current grid row image */
  pixel_y = 0;
  for (row = 0; row < out_image.grid_rows; row++) {
    pixel_x = 0;
    /* Get the current grid column image */
    for (column = 0; column < out_image.grid_columns; column++) {
      Image image = images[row][column];
      for (y = 0; y < image.height; y++) {
        for (x = 0; x < image.width; x++) {
          /* Handle RGB pixel data */
          for (colour = 0; colour < 3; colour++) {
            out_image.prow[pixel_y + y][(pixel_x + x) * 3 + colour] =
                image.prow[y][(x * 3) + colour]; /* + color */
          }
        }
      }
      pixel_x += image.width;
    }
    pixel_y += images[row][column].width;
  }

  /* write to file */
  write_png(filename, &out_image);

  /* Clear memory allocation */
  for (row = 0; row < out_image.grid_rows; row++) {
    for (column = 0; column < out_image.grid_columns; column++) {
      Image image = images[row][column];
      for (y = 0; y < image.height; y++) {
        free(image.prow[y]);
      }
      free(image.prow);
    }
  }
}
