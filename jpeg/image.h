/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *           image.h
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
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <QImage> 

typedef enum {
  Y,
  Cr,
  Cb
} channel_t;

typedef enum {
  ZERO,
  COPY_LAST
} border_t;

class Image : public QImage {
public:
  Image(char *filename, int size = 8);
  ~Image();

  void setSubmatrixSize(int size);

  double *getSubmatrix(int x, int y, channel_t channel, border_t border = COPY_LAST);

  void setYUVPixel(int x, int y, double Y, double Cr, double Cb);

private:
  int size;
};

#endif/*__IMAGE_H__*/


// deadline naeste onsdag d.5/3 - fremlaegger d.7/3
