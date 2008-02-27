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

double a[8][8] = 
{
  {0,1,2,3,4,5,6,7},
  {0,1,2,3,4,5,6,7},
  {0,1,2,3,4,5,6,7},
  {0,1,2,3,4,5,6,7},
  {0,1,2,3,4,5,6,7},
  {0,1,2,3,4,5,6,7},
  {0,1,2,3,4,5,6,7},
  {0,1,2,3,4,5,6,7}
};

#define pixel(x,y) a[x][y]

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
	      
	    q = s * cos((double)(2*x+1) * (double)u * PI/16.0) *
	      cos((double)(2*y+1) * (double)v * PI/16.0);
	      
	    z += q;
	  }
	}
	//data[v][u] = 0.25 * Cu * Cv * z;
	data[v * size + u] = 0.25 * Cu * Cv * z;
      }
  }

  return data;
}
/*
void dct(double data[8][8])
{
  int u,v,x,y;
  
  for (v=0; v<8; v++)
    for (u=0; u<8; u++)
      {
	double Cu, Cv, z = 0.0;
	
	COEFFS(Cu,Cv,u,v);
	
	for (y=0; y<8; y++)
	  for (x=0; x<8; x++)
	    {
	      double s, q;
	      
	      s = pixel(x, y);
	      
	      q = s * cos((double)(2*x+1) * (double)u * PI/16.0) *
		cos((double)(2*y+1) * (double)v * PI/16.0);
	      
	      z += q;
	    }
	
	data[v][u] = 0.25 * Cu * Cv * z;
      }
}
*/

/* play with this bit */
void quantize(double dct_buf[8][8])
{
	int x,y;

	for (y=0; y<8; y++)
	for (x=0; x<8; x++)
	if (x > 3 || y > 3) dct_buf[y][x] = 0.0;
}



void idct(double data[8][8],
	const int xpos, const int ypos)
{
	int u,v,x,y;

#if 0
	/* show the frequency data */
	double lo=0, hi=0;
	if (fabs(hi) > fabs(lo))
		lo = -hi;
	else
		hi = -lo;

	for (y=0; y<8; y++)
	for (x=0; x<8; x++)
	{
		byte z = (byte)( (data[y*8 + x] + 1024.0) / 2048.0 * 255.0);
		put_pixel(im, x+xpos, y+ypos, z);
	}

#else
	/* iDCT */
	for (y=0; y<8; y++)
	for (x=0; x<8; x++)
	{
		double z = 0.0;

		for (v=0; v<8; v++)
		for (u=0; u<8; u++)
		{
			double S, q;
			double Cu, Cv;
			
			COEFFS(Cu,Cv,u,v);
			S = data[v][u];

			q = Cu * Cv * S *
				cos((double)(2*x+1) * (double)u * PI/16.0) *
				cos((double)(2*y+1) * (double)v * PI/16.0);

			z += q;
		}

		z /= 4.0;
		if (z > 255.0) z = 255.0;
		if (z < 0) z = 0.0;

		pixel(x+xpos, y+ypos) = round(z);
	}
#endif
}


/*
int main()
{
  int x,y;
  for(x=0;x<8;x++){
    for(y=0;y<8;y++){
      printf("%.2f ",a[x][y]);
    }
    printf("\n");
  }
  printf("\n\n");

  double b[8][8];

  dct(b,0,0);

  for(x=0;x<8;x++){
    for(y=0;y<8;y++){
      printf("%.2f ",b[x][y]);
    }
    printf("\n");
  }

  idct(b,0,0);

  for(x=0;x<8;x++){
    for(y=0;y<8;y++){
      printf("%.2f ",a[x][y]);
    }
    printf("\n");
  }

  return 0;
}
*/
