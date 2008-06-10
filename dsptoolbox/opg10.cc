/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            opg10.cc
 *
 *  Wed Apr 30 12:22:15 CEST 2008
 *  Copyright 2008 Bent Bisballe Nyeng
 *  deva@aasimon.org
 ****************************************************************************/

/*
 *  This file is part of DSPToolBox.
 *
 *  DSPToolBox is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  DSPToolBox is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with DSPToolBox; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */
#include "samples.h"
#include "dft.h"
#include "klt.h"

#include <stdio.h>
#include <stdlib.h> 

int main()
{
  samplerate_t xfs;
  samples_t x(30);

	int idx = 0;
	char in[]  = "-4 1 1 -1 8 5 0 0 -3 1 9 12 15 0 5 -1 -4 0 7 6 8 6 -3 -9 -3 1 0 2 -1 -2 ";
	char *p = in;
	char *k = in;
	printf("Input:\n");
	while(*p != '\0') {
		if(*p == ' ') {
			*p = '\0';
			double num = atof(k);
			printf("\tRE: %f\n", num);
			x.samples[idx] = num;
			idx++;
			k = p+1;
		}
		p++;
	}

	complex_samples_t *_dft = dft(&x);

	printf("DFT:\n");
	for(idx = 0; idx < _dft->size; idx++) {
		printf("\tRE: %f\tIM: %f\n", _dft->samples[idx][RE], _dft->samples[idx][IM]);
	}

	delete _dft;

	complex_samples_t *_klt = klt(&x);

	printf("KLT:\n");
	for(idx = 0; idx < _klt->size; idx++) {
		printf("\tRE: %f\tIM: %f\n", _klt->samples[idx][RE], _klt->samples[idx][IM]);
	}

	delete _klt;

  return 0;
}
