/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *           vessel.cc
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
#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QDialog>
#include <QTimer>
#include <QPushButton>
#include <QLabel>

#include "slider.h"

#include "image.h"

class MainWindow : public QDialog {
Q_OBJECT
public:
  MainWindow(char *imagefile);
  ~MainWindow();

public slots:
  void ding();
  void diff();
  void reset();
  void timeout();

private:
  Image *src_img;
  Image *dst_img;

  QTimer timer;
  Slider *quality_slider;
  Slider *size_slider;

  QPushButton *go_btn;
  QLabel *status;
};

#endif/*__MAINWINDOW_H__*/
