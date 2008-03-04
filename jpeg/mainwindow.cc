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

#include "huffman.h"

#include "quant_tables.h"
#include <math.h>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QImage>

#include <QApplication>

MainWindow::MainWindow(char *imagefile)
{
  QVBoxLayout *l = new QVBoxLayout();
  setLayout(l);

  QHBoxLayout *box = new QHBoxLayout();

  go_btn = new QPushButton("Go to work");
  box->addWidget(go_btn);
  connect(go_btn, SIGNAL(clicked()), this, SLOT(ding()));

  QPushButton *reset_btn = new QPushButton("Reset");
  box->addWidget(reset_btn);
  connect(reset_btn, SIGNAL(clicked()), this, SLOT(reset()));

  QPushButton *diff_btn = new QPushButton("Diff");
  box->addWidget(diff_btn);
  connect(diff_btn, SIGNAL(clicked()), this, SLOT(diff()));
  
  quality_slider = new Slider(QString("Quality"), -300, 300, 0);
  box->addWidget(quality_slider);

  size_slider = new Slider(QString("Matrix size (8 uses quant. table)"), 1, 32, 8);
  box->addWidget(size_slider);

  l->addLayout(box);

  src_img = new Image(imagefile);
  dst_img = new Image(imagefile);
//img1->width(), img1->height(), QImage::Format_RGB32);

  status = new QLabel("Status");
  status->setFixedHeight(15);
  l->addWidget(status);

  JpegViewer *v1 = new JpegViewer(this, src_img);
  l->addWidget(v1);

  JpegViewer *v2 = new JpegViewer(this, dst_img);
  l->addWidget(v2);

 
  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));

  resize(src_img->width() * 2 + 20, src_img->height() * 2 + 40);
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
        m[qy * size + qx] = (int)m[qy * size + qx];
      }
    }
    // printf("After:\n");
    //printfMatrix(m, size);
  } else {
    int q = (int)(((double)quality + 150.0) / 300.0 * (double)size);
    for(int qy = 0; qy < size; qy++) {
      for(int qx = 0; qx < size; qx++) {
        if(qx > q || qy > q) m[qy * size + qx] = 0.0;
        m[qy * size + qx] = (int)m[qy * size + qx];
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
        m[qy * size + qx] = (int)m[qy * size + qx];
      }
    }
  } else {
    int q = (int)(((double)quality + 150.0) / 300.0 * (double)size);
    for(int qy = 0; qy < size; qy++) {
      for(int qx = 0; qx < size; qx++) {
        if(qx > q || qy > q) m[qy * size + qx] = 0.0;
        m[qy * size + qx] = (int)m[qy * size + qx];
      }
    }
  }
}

void MainWindow::reset()
{
  status->setText("Image has been reset");
  for(int x = 0; x < src_img->width(); x++) {
    for(int y = 0; y < src_img->height(); y++) {
      dst_img->setPixel(x, y, src_img->pixel(x, y));
    }
  }
  repaint();
}

double sqr(double a) {return a * a;}
void MainWindow::diff()
{
  double  mse = 0.0;

  for(int x = 0; x < src_img->width(); x++) {
    for(int y = 0; y < src_img->height(); y++) {
      int sr = qRed(src_img->pixel(x, y));
      int dr = qRed(dst_img->pixel(x, y));

      int sg = qGreen(src_img->pixel(x, y));
      int dg = qGreen(dst_img->pixel(x, y));

      int sb = qBlue(src_img->pixel(x, y));
      int db = qBlue(dst_img->pixel(x, y));

      dst_img->setPixel(x, y, qRgb(fabs(sr - dr), fabs(sg - dg), fabs(sb - db)));
      mse += sqr(fabs(sr - dr)) + sqr(fabs(sg - dg)) + sqr(fabs(sb - db));
    }
  }

  mse /= src_img->width() * src_img->height() * 3;

  int max = pow(2,8)-1;

  double psnr = 10 * log10(sqr(max)/mse);

  char buf[64];
  sprintf(buf, "PSNR: %f dB", psnr);
  status->setText(buf);

  repaint();
}

void MainWindow::ding()
{
  status->setText("Working...");

  std::string out;

  go_btn->setEnabled(false);
  timer.start(500);

  border_t border = COPY_LAST;
  //  channel_t channel = Y;
  int size = size_slider->value();
  int quality = quality_slider->value();

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

      out += huffman_ac_encode(dct_m_Y, size);
      out += huffman_ac_encode(dct_m_Cr, size);
      out += huffman_ac_encode(dct_m_Cb, size);

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
  /*
  printf("sizeof %d bits (%d bytes)\n", out.length(), out.length() / 8);
  FILE *fp = fopen("output.asc", "w");
  fprintf(fp, "%s", out.c_str());
  fclose(fp);
  */
  
  char buf[512];
  sprintf(buf, "Working...done. Reduced image from %d bytes to %d bytes (%f%% compression)",
          width() * height() * 3,
          out.length() / 8,
          (out.length() / 8.0) / (width() * height() * 3.0) * 100);
  status->setText(buf);
  go_btn->setEnabled(true);
}
