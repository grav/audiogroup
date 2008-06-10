/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            wavplay.cc
 *
 *  Tue Jun 10 08:25:42 CEST 2008
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
#include "wavplay.h"
// gcc -lao -ldl -lm -o ao_example ao_example.c

#include <ao/ao.h>

void wavplay(samples_t *samples, samplerate_t srate)
{
	ao_device *device;
	ao_sample_format format;
	int default_driver;
	char *buffer;
	int buf_size;
	int sample;
	
	ao_initialize();
	
	// Setup for default driver
	
	default_driver = ao_default_driver_id();

	format.bits = 16;
	format.channels = 1;
	format.rate = srate;
	format.byte_format = AO_FMT_LITTLE;
	
	// Open driver
	device = ao_open_live(default_driver, &format, NULL /* no options */);
	if (device == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return;
	}   
	
	// Play some stuff
	buf_size = samples->size * format.bits/8 * format.channels;
	buffer = (char*)calloc(buf_size, sizeof(char));
	
	for(int i = 0; i < samples->size; i++) {
    buffer[2 * i] = (int)(samples->samples[i] * 32767) & 0xff;
    buffer[2 * i + 1] = ((int)(samples->samples[i] * 32767) >> 8) & 0xff; 
	}
	ao_play(device, buffer, buf_size);
	
	// Close and shutdown
	ao_close(device);
    
	ao_shutdown();
}
