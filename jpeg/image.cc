/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *           image.cc
 *
 *  Tue Feb 12 14:30:50 CET 2008
 *  Copyright 2008 Bent Bisballe Nyeng
 *  deva@aasimon.org
 ****************************************************************************/

/*
 *  This is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with it; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */
#include "image.h"

/**
   From Wikipedia:

   JPEG-Y'CbCr (601) from "digital 8-bit R'G'B'  "
   ========================================================================
   Y' =       + 0.299    * R'd + 0.587    * G'd + 0.114    * B'd
   Cb = 128   - 0.168736 * R'd - 0.331264 * G'd + 0.5      * B'd
   Cr = 128   + 0.5      * R'd - 0.418688 * G'd - 0.081312 * B'd

 **/

static inline double rgb2y(QRgb rgb)
{
//  double Y = 0 + 0.299 * qRed(rgb) + 0.587 * qGreen(rgb) + 0.114 * qBlue(rgb);
  double Y = 16  + 1/256.0 * (65.738  * qRed(rgb) +  129.057 * qGreen(rgb) + 25.064  * qBlue(rgb));  
  return Y;
}

static inline double rgb2cr(QRgb rgb)
{
//  double Cb = 128 - 0.168736 * qRed(rgb) - 0.331264 * qGreen(rgb) + 0.5 * qBlue(rgb);
  double Cb = 128 + 1/256.0 * ( -37.945  * qRed(rgb) - 74.494  * qGreen(rgb) + 112.439  * qBlue(rgb));
  return Cb;
}

static inline double rgb2cb(QRgb rgb)
{
//  double Cr = 128 + 0.5 * qRed(rgb) - 0.418688 * qGreen(rgb) - 0.081312 * qBlue(rgb);
  double Cr = 128 + 1/256.0 * (  112.439  * qRed(rgb) - 94.154  * qGreen(rgb) -  18.285  * qBlue(rgb));
  return Cr;
}

Image::Image(char *filename, int sz)
  : QImage(filename)
{
  setSubmatrixSize(sz);
}

Image::~Image()
{
}

void Image::setSubmatrixSize(int sz)
{
  size = sz;
}


static inline int clip(int a) { if(a > 255) return 255; if(a < 0) return 0; return a; }
void Image::setYUVPixel(int x, int y, double Y, double Cr, double Cb)
{
  /*
    C = Y - 16
    D = U - 128
    E = V - 128

    Using the previous coefficients and noting that clip() denotes clipping a value to the range of 0 to 255,
    the following formulas provide the conversion from YUV to RGB:
    
    R = clip(( 298 * C           + 409 * E + 128) >> 8)
    G = clip(( 298 * C - 100 * D - 208 * E + 128) >> 8)
    B = clip(( 298 * C + 516 * D           + 128) >> 8)
   */

#if 1
  int C = Y - 16;
  int D = Cr - 128;
  int E = Cb - 128;

  int R = clip(( 298 * C           + 409 * E + 128) >> 8);
  int G = clip(( 298 * C - 100 * D - 208 * E + 128) >> 8);
  int B = clip(( 298 * C + 516 * D           + 128) >> 8);
#else
  int Yi = (int)Y;
  int Cri = (int)Cr;
  int Cbi = (int)Cb;

  int R = clip(( 298.082 * Yi                 + 408.583 * Cri ) / 256 - 222.921);
  int G = clip(( 298.082 * Yi - 100.291 * Cbi - 208.120 * Cri ) / 256 + 135.576);
  int B = clip(( 298.082 * Yi + 516.412 * Cbi                 ) / 256 - 276.836);
#endif

  QRgb rgb = qRgb(R, G, B);

  setPixel(x, y, rgb);
}


#define M(x, y) m[y * size + x]
static inline double rgb2yuv(QRgb rgb, channel_t ch)
{
  switch(ch) {
  case Y:
    return rgb2y(rgb);
  case Cr:
    return rgb2cr(rgb);
  case Cb:
    return rgb2cb(rgb);
  }
  return -1;
}


double *Image::getSubmatrix(int x, int y, channel_t ch, border_t border)
{
  double *m = (double*)malloc(size * size * sizeof(double));

  double val;
  for(int dx = 0; dx < size; dx++) {
    for(int dy = 0; dy < size; dy++) {
      if(dx + x >= width() || dy + y >= height()) {
        switch(border) {
        case ZERO:
          val = 0.0;
          break;

        case COPY_LAST:
          int _x = dx + x;
          int _y = dy + y;
          if(dx + x >= width()) _x = width() - 1;
          if(dy + y >= height()) _y = height() - 1;
          val = rgb2yuv(pixel(_x, _y), ch);
        }
      } else {
        val = rgb2yuv(pixel(dx + x, dy + y), ch);
      }
      M(dx, dy) = val;
    }
  }

  return m;
}
