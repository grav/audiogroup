/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *
 *  Tue Jun 10 10:02:53 CEST 2008
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
#ifndef __DSPTOOLBOX_CONFIG_H__
#define __DSPTOOLBOX_CONFIG_H__

#define NUM_BANDS 32
#define FRAME_SIZE 512

typedef enum {
  THR_LINEAR,
  THR_BIQUAD,
  THR_AVG
} threshold_t;

namespace config{
  extern bool mask;
  extern float quality;
  extern float curve_offset;
  extern threshold_t threshold;
  extern unsigned int num_threads;
};

#endif/*__DSPTOOLBOX_CONFIG_H__*/
