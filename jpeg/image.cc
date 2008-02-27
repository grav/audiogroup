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
  double Y = 0 + 0.299 * qRed(rgb) + 0.587 * qGreen(rgb) + 0.114 * qBlue(rgb);
  return Y;
}

static inline double rgb2cr(QRgb rgb)
{
  double Cb = 128 - 0.168736 * qRed(rgb) - 0.331264 * qGreen(rgb) + 0.5 * qBlue(rgb);
  return Cb;
}

static inline double rgb2cb(QRgb rgb)
{
  double Cr = 128 + 0.5 * qRed(rgb) - 0.418688 * qGreen(rgb) - 0.081312 * qBlue(rgb);
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

#define M(x, y) m[y * size + x]
double *Image::getYSubmatrix(int x, int y)
{
  double *m = (double*)malloc(size * size * sizeof(double));

  double val;
  for(int dx = 0; dx < size; dx++) {
    for(int dy = 0; dy < size; dy++) {
      if(x > width()) val = 0.0;
      else val = rgb2y(pixel(dx + x, dy + y));
      M(dx, dy) = val;
    }
  }

  return m;
}

double *Image::getCrSubmatrix(int _x, int _y)
{
  double *m = (double*)malloc(size * size * sizeof(double));

  double val;
  for(int x = _x; x < size; x++) {
    for(int y = _y; y < size; y++) {
      if(x > width()) val = 0.0;
      else val = rgb2cr(pixel(x, y));
      M(x, y) = val;
    }
  }

  return m;
}

double *Image::getCbSubmatrix(int _x, int _y)
{
  double *m = (double*)malloc(size * size * sizeof(double));

  double val;
  for(int x = _x; x < size; x++) {
    for(int y = _y; y < size; y++) {
      if(x > width()) val = 0.0;
      else val = rgb2cb(pixel(x, y));
      M(x, y) = val;
    }
  }

  return m;
}
