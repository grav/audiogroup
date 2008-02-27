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

#include <QVBoxLayout>
#include <QImage>

#include <QPushButton>

MainWindow::MainWindow()
{
  QVBoxLayout *l = new QVBoxLayout();
  setLayout(l);

  QPushButton *btn = new QPushButton("ding");
  l->addWidget(btn);
  connect(btn, SIGNAL(clicked()), this, SLOT(ding()));

  src_img = new Image("thomas.jpg");
  dst_img = new Image("thomas.jpg");
//img1->width(), img1->height(), QImage::Format_RGB32);

  JpegViewer *v1 = new JpegViewer(this, src_img);
  l->addWidget(v1);

  JpegViewer *v2 = new JpegViewer(this, dst_img);
  l->addWidget(v2);

  resize(600, 600);
}

MainWindow::~MainWindow()
{

}

void MainWindow::ding()
{
  /*
  for(int i = 0; i < 10000; i++) {
    dst_img->setPixel(rand() % dst_img->width(), rand() % dst_img->height(),
                      qRgb(rand() % 255, rand() % 255, rand() % 255));
  }
  repaint();
  */

  int size = 8;

  src_img->setSubmatrixSize(size);
  dst_img->setSubmatrixSize(size);

  for(int x = 0; x < src_img->width(); x += size) {
    for(int y = 0; y < src_img->height(); y += size) {
      double *m = src_img->getYSubmatrix(x, y);

      double *dct_m = dct(m, size);

      // quantize
      int qx,qy;
      for(qy=0; qy<size; qy++)
        for(qx=0; qx<size; qx++)
          if(qx > 1 || qy > 1) dct_m[qy * size + qx] = 0.0;

      // eoq

      double *idct_m = idct(dct_m, size);

      for(int dx = 0; dx < size; dx++) {
        for(int dy = 0; dy < size; dy++) {
            dst_img->setPixel(x + dx, y + dy,
                              qRgb(idct_m[dy * size + dx],
                                   idct_m[dy * size + dx],
                                   idct_m[dy * size + dx])
                              );
        }
      }
      repaint();
      free(m);
      free(dct_m);
    }
  }

  repaint();
}
