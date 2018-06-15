#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Maxfiles.h"
#include <MaxSLiCInterface.h>

#include "../CPUCode/ppmIO.h"

int abs_max(int idx, int i, int max, int32_t *inImage) {
  int a = abs(inImage[idx] - inImage[i]);
  return a > max ? a : max;
}

void apply_difference_operator(int width, int height, int32_t *inImage, int32_t *outImage) {
  int idx = 0;
  int idx_up = idx - width;
  int idx_down = idx + width;

  for (int l = 0; l < height; ++l) {
    for (int c = 0; c < width; ++c) {
      int max = 0;

      int first_col = c == 0;
      int last_col = c == (width - 1);

      int first_lin = l == 0;
      int last_lin = l == (height - 1);

      if (!first_lin) {
        if (!first_col) max = abs_max(idx, idx_up - 1, max, inImage);
        max = abs_max(idx, idx_up, max, inImage);
        if (!last_col) max = abs_max(idx, idx_up + 1, max, inImage);
      }

      if (!first_col) max = abs_max(idx, idx - 1, max, inImage);
      if (!last_col) max = abs_max(idx, idx + 1, max, inImage);

      if (!last_lin) {
        if (!first_col) max = abs_max(idx, idx_down - 1, max, inImage);
        max = abs_max(idx, idx_down, max, inImage);
        if (!last_col) max = abs_max(idx, idx_down + 1, max, inImage);
      }

      outImage[idx] = max;

      idx++;
      idx_up++;
      idx_down++;
    }
  }
}

int main(void) {
  printf("Loading image.\n");
  int32_t *inImage;
  int width = 0, height = 0;
  //loadImage("/home/a01656520/WebIDE-Projects/DifferenceOperator/CPUCode/test1.ppm", &inImage, &width, &height, 1);
  //loadImage("/home/a01656520/WebIDE-Projects/DifferenceOperator/CPUCode/test2.ppm", &inImage, &width, &height, 1);
  loadImage("/home/a01656520/WebIDE-Projects/DifferenceOperator/CPUCode/lena.ppm", &inImage, &width, &height, 1);


  uint64_t n = width * height;
  size_t size = n * sizeof(int32_t);
  int32_t *outImage = malloc(size);

//  apply_difference_operator(width, height, inImage, outImage);

  printf("Running Kernel.\n");
  DifferenceOperator(n, inImage, size, outImage, size);

  printf("Saving image.\n");
  //writeImage("/home/a01656520/WebIDE-Projects/DifferenceOperator/CPUCode/test1_difference.ppm", outImage, width, height, 1);
  //writeImage("/home/a01656520/WebIDE-Projects/DifferenceOperator/CPUCode/test2_difference.ppm", outImage, width, height, 1);
  writeImage("/home/a01656520/WebIDE-Projects/DifferenceOperator/CPUCode/lena_difference.ppm", outImage, width, height, 1);

  printf("Exiting\n");
  return 0;
}
