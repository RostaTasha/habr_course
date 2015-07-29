/*
 * parser.hpp
 *
 *  Created on: 06 θώνÿ 2015 γ.
 *      Author: Tata
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "tgaimage.h"
#include <iostream>
#include <fstream>
#include<vector>
#include <sstream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <algorithm>    // std::sort
#include <limits>

using namespace std;

template <typename T>
class d3{
public:
d3 (T a, T b, T c){
x=a;
y=b;
z=c;
}
d3(){
x=0;
y=0;
z=0;
}

T x;
T y;
T z;
};



template <typename T>
class mtrx2d{

public:



	T & operator()(const int i, const int j){
	 return (ar[i+w*j]);
	}

	mtrx2d(int h, int w, T val=0){
	this->h=h;
	this->w=w;
	for (int i =0; i< h; i++){
		for (int j =0; j< w; j++){
		ar.push_back(val);
	}
	}
	}




	vector<T> ar;
	int h;
	int w;
};

template <typename T>
class vecd3 : public d3<T> {
public:

	vecd3 (T a, T b, T c) : d3<T>::d3(a, b, c){
	}

	vecd3 (d3<T> a, d3<T>  b){
		d3<T>::x=a.x-b.x;
		d3<T>::y=a.y-b.y;
		d3<T>::z=a.z-b.z;
	}
const vecd3 operator+(const vecd3& rv) const {
        return  vecd3(d3<T>::x+rv.x,d3<T>::y+rv.y,d3<T>::z+rv.z);
    }

const vecd3 operator-(const vecd3& rv) const {
        return  vecd3(d3<T>::x-rv.x,d3<T>::y-rv.y,d3<T>::z-rv.z);
    }

float operator*(const vecd3& rv) const {
        return d3<T>::x*rv.x+ d3<T>::y*rv.y+ d3<T>::z*rv.z;
    }

const vecd3 operator^(const vecd3& rv) const {
        return  vecd3(d3<T>::y*rv.z-d3<T>::z*rv.y, d3<T>::z*rv.x-d3<T>::x*rv.z, d3<T>::x*rv.y-d3<T>::y*rv.x);
    }

void normalize(){
	float temp=sqrt(d3<T>::x*d3<T>::x+d3<T>::y*d3<T>::y+d3<T>::z*d3<T>::z);
	d3<T>::x=d3<T>::x/temp;
	d3<T>::y=d3<T>::y/temp;
	d3<T>::z=d3<T>::z/temp;
}

};



void parser(std::vector<d3<float> >  & coords, std::vector<d3<int> >  & triangles, std::vector<d3<float> > & norm, std::vector<d3<int> >  & norm_triangles, std::vector<d3<float> > & text, std::vector<d3<int> >  & text_triangles, TGAImage & textures);
float max(float a, float b);
float min(float a, float b);

#endif /* PARSER_HPP_ */
