/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *           slider.cc
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
#include "slider.h"

#include <QVBoxLayout>

Slider::Slider(QString title, int min, int max, int val)
	: slider(Qt::Horizontal), label(title)
{
	setLayout(new QVBoxLayout());

	this->title = title;

	slider.setMinimum(min);
	slider.setMaximum(max);
	slider.setValue(val);

	layout()->addWidget(&label);
	layout()->addWidget(&slider);

	connect(&slider, SIGNAL(valueChanged(int)), this, SLOT(valupdate(int)));

  valupdate(val);

	setFixedSize(220, 55);
}

int Slider::value()
{
	return slider.value();
}

void Slider::valupdate(int value)
{
	char val[32];
	sprintf(val, "%d", value);
	label.setText(title + ": " + val);
}
