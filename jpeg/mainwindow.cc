/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *           mainwindow.cc
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
#include "mainwindow.h"

#include "jpeg_viewer.h"

#include "dct.h"

#include "quant_tables.h"
#include <math.h>
#include <QVBoxLayout>
#include <QImage>

#include <QApplication>

#include <QPushButton>

MainWindow::MainWindow(char *imagefile)
{
  QVBoxLayout *l = new QVBoxLayout();
  setLayout(l);

  QPushButton *btn = new QPushButton("ding");
  l->addWidget(btn);
  connect(btn, SIGNAL(clicked()), this, SLOT(ding()));

  src_img = new Image(imagefile);
  dst_img = new Image(imagefile);
//img1->width(), img1->height(), QImage::Format_RGB32);

  JpegViewer *v1 = new JpegViewer(this, src_img);
  l->addWidget(v1);

  JpegViewer *v2 = new JpegViewer(this, dst_img);
  l->addWidget(v2);

 
  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));

  resize(600, 600);
}

MainWindow::~MainWindow()
{

}

void MainWindow::timeout()
{
  repaint();
  qApp->processEvents();
}


void printfMatrix(double *m, int size)
{
  for(int qx = 0; qx < size; qx++) {
    for(int qy = 0; qy < size; qy++) {
      printf("\t%.2f", m[qy * size + qx]);
    }
    printf("\n");
  }
}

void quantize_lum(double *m, int size, int quality)
{
  if(size == 8) {
    //    printf("Before:\n");
    //printfMatrix(m, size);
    for(int qy = 0; qy < size; qy++) {
      for(int qx = 0; qx < size; qx++) {
        if(fabs(m[qy * size + qx]) < quant_lum_table[qy][qx] - quality) m[qy * size + qx]  = 0;
      }
    }
    // printf("After:\n");
    //printfMatrix(m, size);
  } else {
    for(int qy = 0; qy < size; qy++) {
      for(int qx = 0; qx < size; qx++) {
        if(qx > 1 || qy > 1) m[qy * size + qx] = 0.0;
      }
    }
  }
}

void quantize_chrom(double *m, int size, int quality)
{
  if(size == 8) {
    for(int qy = 0; qy < size; qy++) {
      for(int qx = 0; qx < size; qx++) {
        if(fabs(m[qy * size + qx]) < quant_chrom_table[qy][qx] - quality) m[qy * size + qx]  = 0;
      }
    }
  } else {
    for(int qy = 0; qy < size; qy++) {
      for(int qx = 0; qx < size; qx++) {
        if(qx > 1 || qy > 1) m[qy * size + qx] = 0.0;
      }
    }
  }
}

void MainWindow::ding()
{
  timer.start(500);

  border_t border = COPY_LAST;
  channel_t channel = Y;
  int size = 8;
  int quality = 0;

  src_img->setSubmatrixSize(size);
  dst_img->setSubmatrixSize(size);

  for(int x = 0; x < src_img->width(); x += size) {
    for(int y = 0; y < src_img->height(); y += size) {
      double *m_Y = src_img->getSubmatrix(x, y, Y, border);
      double *m_Cr = src_img->getSubmatrix(x, y, Cr, border);
      double *m_Cb = src_img->getSubmatrix(x, y, Cb, border);

      double *dct_m_Y = dct(m_Y, size);
      double *dct_m_Cr = dct(m_Cr, size);
      double *dct_m_Cb = dct(m_Cb, size);

      quantize_lum(dct_m_Y, size, quality);
      quantize_chrom(dct_m_Cr, size, quality);
      quantize_chrom(dct_m_Cb, size, quality);

      // eoq

      double *idct_m_Y = idct(dct_m_Y, size);
      double *idct_m_Cr = idct(dct_m_Cr, size);
      double *idct_m_Cb = idct(dct_m_Cb, size);

      for(int dx = 0; dx < size; dx++) {
        for(int dy = 0; dy < size; dy++) {
          if((x + dx < dst_img->width()) &&
             (y + dy < dst_img->height())) {
            dst_img->setYUVPixel(x + dx, y + dy,
                                 idct_m_Y[dy * size + dx],
                                 idct_m_Cr[dy * size + dx],
                                 idct_m_Cb[dy * size + dx]);
          }
        }
        qApp->processEvents();
     }
      free(m_Y);
      free(m_Cb);
      free(m_Cr);
      free(dct_m_Y);
      free(dct_m_Cr);
      free(dct_m_Cb);
    }
  }

  timer.stop();
  repaint();
}
