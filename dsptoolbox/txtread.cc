/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            txtread.cc
 *
 *  Fri Feb  6 12:24:21 CET 2009
 *  Copyright 2009 Bent Bisballe Nyeng
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
#include "txtread.h"

#include <stdio.h>
#include <string>
#include <string.h>

samples_t *txtread(const char *filename)
{
  FILE *handle = fopen(filename, "r");

  size_t size = 0;
  
  char data[] = {'\0','\0'};
  std::string num;

  // Find length
  while(!feof(handle)) {
    fread(&data, 1, 1, handle);
    if(strchr("-.0123456789+Ee", data[0])) {
      num += data[0];
    } else {
      if(num.length()) {
        size++;
        num = "";
      }
    }
  }

  // Allocate samples
  samples_t *s = new samples_t(size);

  // Reset file
  fseek(handle, 0, SEEK_SET);

  int pos = 0;
  // Read the samples
  while(!feof(handle)) {
    fread(&data, 1, 1, handle);
    if(strchr("-.0123456789+Ee", data[0])) {
      num += data[0];
    } else {
      if(num.length()) {
        s->samples[pos] = atof(num.c_str());
        pos++;
        num = "";
      }
    }
  }

  fclose(handle);

  return s;
}
