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
  MASK_LINEAR,
  MASK_BIQUAD,
  MASK_AVG
} mask_t;

typedef enum {
  FILTER_LIN,
  FILTER_LOG
} filter_t;

namespace config{
  extern float mask_weight;
  extern float quality;
  extern float ath_weight;
  extern mask_t mask;
  extern unsigned int num_threads;
  extern filter_t filter;
};

#endif/*__DSPTOOLBOX_CONFIG_H__*/
