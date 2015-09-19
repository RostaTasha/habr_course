/*
 * matrix.h
 *
 *  Created on: 26 рту. 2015 у.
 *      Author: Tata
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdio.h>
#include <string.h>

#if 0
class matrix{
public:
	int n;
	int m;
	float * ar;

	matrix(int n =1, int m = 1){
		ar = new float[n*m];
		this->n=n;
		this->m=m;
		for (int i=0; i<n*m;i++)
		ar[i]=0;
	}

	matrix(int n=1,int m=1, float * ar_i=NULL){
		ar = new float [n*m];
		this->n=n;
		this->m=m;
		memcpy(ar,ar_i,m*n*4);
	}

	~matrix(){
		delete [] ar;
	}

	matrix& operator=( matrix &mat_in ){
	delete [] ar;
	memcpy(ar,mat_in.ar,mat_in.m*mat_in.n*4);
	n=mat_in.n;
	m=mat_in.m;
	}

	float& operator() ( int i, int j){
	return ar[i+n*j];
	}

	float*& operator() (){
	return ar;
	}

	int get_n(){
	return n;
	};

	int get_m(){
	return m;
	};

	matrix& operator*( matrix &mat_in ){
	int mm=mat_in.get_m();
	float  * ar_temp = new float  [n*mm];
	for (int i =0; i< n; i++)
		for (int j =0; j <mat_in.m;j++){

			for (int k =0; k < m; k++)
			ar_temp[i+n*j]+=(*this)(i,k)*mat_in(k,j);

	}

	matrix temp_m   = matrix(n,mm, ar_temp);
	delete [] ar_temp;

	return temp_m;
	}


};

#endif


#endif /* MATRIX_H_ */
