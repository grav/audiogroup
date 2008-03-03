/* DCT and IDCT - listing 1
 * Copyright (c) 2001 Emil Mikulic.
 * http://dmr.ath.cx/
 *
 * Feel free to do whatever you like with this code.
 * Feel free to credit me.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef PI
 #ifdef M_PI
  #define PI M_PI
 #else
  #define PI 3.14159265358979
 #endif
#endif



/* S[u,v] = 1/4 * C[u] * C[v] *
 *   sum for x=0 to width-1 of
 *   sum for y=0 to height-1 of
 *     s[x,y] * cos( (2x+1)*u*PI / 2N ) * cos( (2y+1)*v*PI / 2N )
 *
 * C[u], C[v] = 1/sqrt(2) for u, v = 0
 * otherwise, C[u], C[v] = 1
 *
 * S[u,v] ranges from -2^10 to 2^10
 */

#define COEFFS(Cu,Cv,u,v) { \
	if (u == 0) Cu = 1.0 / sqrt(2.0); else Cu = 1.0; \
	if (v == 0) Cv = 1.0 / sqrt(2.0); else Cv = 1.0; \
	}


double *dct(double *m, int size)
{
  int u,v,x,y;
  double *data = (double*)malloc(size * size * sizeof(double));


  for (v=0; v<size; v++) {
    for (u=0; u<size; u++) {
	double Cu, Cv, z = 0.0;
	
	COEFFS(Cu,Cv,u,v);
	
	for(y = 0; y < size; y++) {
	  for(x = 0; x < size; x++) {
	    double s, q;
	      
	    s = m[y * size + x];//pixel(x, y);
	      
	    q = s * cos((double)(2*x+1) * (double)u * PI/(size * 2.0)/*16.0*/) *
	      cos((double)(2*y+1) * (double)v * PI/(size * 2.0)/*16.0*/);
	      
	    z += q;
	  }
	}
	data[v * size + u] = /*0.25*/ (1.0 / (size / 2.0)) * Cu * Cv * z;
	//if( data[v * size + u] < 0) printf("%f\n", data[v * size + u]);
      }
  }

  return data;
}

double *idct(double *m, int size)
{
  int u,v,x,y;

  double *data = (double*)malloc(size * size * sizeof(double));

  for(y = 0; y < size; y++) {
    for(x = 0; x < size; x++) {
      double z = 0.0;

      for(v = 0; v < size; v++) {
	for(u = 0; u < size; u++) {
	  double S, q;
	  double Cu, Cv;
			
	  COEFFS(Cu,Cv,u,v);
	  S = m[v * size + u];
	  
	  q = Cu * Cv * S *
	    cos((double)(2*x+1) * (double)u * PI/(size * 2.0)/*16.0*/) *
	    cos((double)(2*y+1) * (double)v * PI/(size * 2.0)/*16.0*/);
	  
	  z += q;
	}
      }

      z /= (size / 2.0);//4.0;
      if (z > 255.0) z = 255.0;
      if (z < 0) z = 0.0;

      data[y * size + x] = round(z);
    }
  }
  return data;
}

