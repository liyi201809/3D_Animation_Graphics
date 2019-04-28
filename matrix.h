#ifndef matrix_H
#define matrix_H

#include <iostream>

/// Identity matrix
const GLdouble identity[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};


void printMatrix( const GLdouble* m )
{
	for( int i=0; i<16; ++i ) std::cout<<m[i]<<" ";
}


void project( const GLdouble* m, const GLdouble* p, GLdouble* q )
{
	for( int i=0; i<3; ++i ){
		q[i] = 0;
		for( int j=0; j<4; ++j )
			q[i] += m[i+4*j]*p[j];
	}
	q[3] = p[3];
}


void inverse( const double *m, double *p )
{
	{
		for(int i=0; i<3; ++i )
			for(int j=0; j<3; ++j )
				p[i+4*j] = m[j+4*i];
	}

	{
		double q[3]={0,0,0};
		for(int i=0; i<3; ++i ){
			for(int j=0; j<3; ++j )
				q[i] -= p[i+4*j]*m[12+j];
		}
		p[12]=q[0]; p[13]=q[1]; p[14]=q[2];
	}

	p[3] = p[7] = p[11] = 0; p[15] = 1;
}

#endif
