/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *           huffman.cc
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
#include "huffman.h"
#include "huffman_tables.h"
#include <math.h>
#include <assert.h>

// instansvariabel som vælger lum eller chrom table
encode_table *int[][];

Huffman::Huffman(... luminance eller chrominance)
{
  // Choose either luminance or chrominance table for encoding
	if (c == Channel.LUMINANCE) {
		encode_table = lum_table;
	} else {
		encode_table= chrom_table;
	}

	// Build decode map
	decodeMap = new HashMap<String,Pair<Integer,Integer>>();
	for(int r=0; r<16; r++){
		for(int s=0; s<10; s++){
			Pair<Integer,Integer> p = new Pair<Integer,Integer>(r,s);
			decodeMap.put(encode_table[r][s], p);
		}
	}
}

Huffman::~Huffman()
{

}

std::string huffman_ac_encode(double *m, int msize)
{
	std::string s = "";
	
	int runlength = 0;
	for(int i=0;i<c.length;i++){
		if (c[i]==0){
			runlength++;
		} else {
			s += encodeCoeff(c[i],runlength);
			runlength=0;
		}
	}
	return s + encode_table[0][0]; // EOB
}

double *huffman_ac_decode(std::string codeStream, int blockSize)
{
	int[] c = new int[blockSize]; // Coefficient array
	
	int i = 0; // pointer into codeStream
	int j = 0; // pointer into coefficient array
	
	std::string key = ""; // the possible key extracted from codeStream
	
	while(i < codeStream.length() ){
		// grab a character
		key += codeStream.substring(i,i+1); i++;
		Pair<Integer,Integer> p = decodeMap.get(key); // fetch (runlength,size) pair from decodeMap
		if ( p != null){
			// reset search key
			key = "";

			int runlength = p.getFirst();
			int size = p.getSecond();
			for (int k=0;k<runlength;k++){
				c[j]=0; j++;
			}
			if (size>0){
				String binStr = codeStream.substring(i, i+size); i+=size;
				c[j]=Integer.parseInt(binStr, 2); j++;
			}
			if (runlength==0 && size==0){
				// Fill rest of coeff. array with zeros
				while(j<blockSize){
					c[j]=0; j++;
				}
			}				
		}
	}
	return c;
}

// Encode single coefficient and runlength
std::string encodeCoeff(int val, int runlength)
{
		// Find no. bits to represent number to encode
		int size = bitSize(val);

		// make several requests if runlength > maxrun
		int maxrun = 15;
		std::string s = "";
		for(int i=0; i<runlength/maxrun;i++){
			s += encode_table[maxrun][0];
		}
		s+= encode_table[runlength % maxrun][size];
		s += Integer.toString(val,2);
		return s;
}

	// Calc number of bits to represent value
int bitSize(int val)
{
		// Values must be normalized
		assert(val>=0);
		return (int)(log(val)/log(2)) + 1;
}

// Zig zag trough block and make an array
int[] zigzag(int[][] block){
	int N = block.length;
	int M = block[0].length;
	int m = 0;
	int n = 0;
	int[] c = new int[block.length*block[0].length];
	boolean n_inc = true;
	for (int i=0;i<c.length;i++){
		c[i] = block[m][n];
		if (n_inc){
			n++;
			m--;
			// Change scan direction
			if (m<0 || n>=N) n_inc = false; // skift retning
			// Corner
			if (m<0 && n>=N) { 
				m = 1; // Increase extra
				n = N-1;
			} else if (m<0 && n<N) {
				m = 0;
			} else if (n>=N && m>=0) {					
				n=N-1;
				m += 2;
			}
		} else {
			n--;
			m++;
			// Change scan direction
			if (n<0 || m>=M) n_inc = true; // skift retning
			// Corner
			if (n<0 && m>=M) { 
				n = 1; // Increase extra
				m = M-1;
			} else if (n<0 && m<M) {
				n = 0;
			} else if (m>=M && n>=0) {					
				m=M-1;
				n+=2;
			}
		}
	}
	
	return c;
}

// Create coeff block from array
// Fy fy, kodekopiering
int[][] antiZigzag(int[] c, int M, int N){
	assert(c.length==M*N);
	int[][] block = new int[M][N];
	int m = 0;
	int n = 0;
	boolean n_inc = true;
	for (int i=0;i<c.length;i++){
		block[m][n] = c[i];
		if (n_inc){
			n++;
			m--;
			// Change scan direction
			if (m<0 || n>=N) n_inc = false; // skift retning
			// Corner
			if (m<0 && n>=N) { 
				m = 1; // Increase extra
				n = N-1;
			} else if (m<0 && n<N) {
				m = 0;
			} else if (n>=N && m>=0) {					
				n=N-1;
				m += 2;
			}
		} else {
			n--;
			m++;
			// Change scan direction
			if (n<0 || m>=M) n_inc = true; // skift retning
			// Corner
			if (n<0 && m>=M) { 
				n = 1; // Increase extra
				m = M-1;
			} else if (n<0 && m<M) {
				n = 0;
			} else if (m>=M && n>=0) {					
				m=M-1;
				n+=2;
			}
		}
	}
	
	return block;
}

