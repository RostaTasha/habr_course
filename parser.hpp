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
class vec{
public:
vec (T a, T b, T c){
x=a;
y=b;
z=c;
}
vec(){
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
class vecvec : public vec<T> {
public:

	vecvec (T a, T b, T c) : vec<T>::vec(a, b, c){
	}

	vecvec (vec<T> a, vec<T>  b){
		vec<T>::x=a.x-b.x;
		vec<T>::y=a.y-b.y;
		vec<T>::z=a.z-b.z;
	}
const vecvec operator+(const vecvec& rv) const {
        return  vecvec(vec<T>::x+rv.x,vec<T>::y+rv.y,vec<T>::z+rv.z);
    }

const vecvec operator-(const vecvec& rv) const {
        return  vecvec(vec<T>::x-rv.x,vec<T>::y-rv.y,vec<T>::z-rv.z);
    }

float operator*(const vecvec& rv) const {
        return vec<T>::x*rv.x+ vec<T>::y*rv.y+ vec<T>::z*rv.z;
    }

const vecvec operator^(const vecvec& rv) const {
        return  vecvec(vec<T>::y*rv.z-vec<T>::z*rv.y, vec<T>::z*rv.x-vec<T>::x*rv.z, vec<T>::x*rv.y-vec<T>::y*rv.x);
    }

void normalize(){
	float temp=sqrt(vec<T>::x*vec<T>::x+vec<T>::y*vec<T>::y+vec<T>::z*vec<T>::z);
	vec<T>::x=vec<T>::x/temp;
	vec<T>::y=vec<T>::y/temp;
	vec<T>::z=vec<T>::z/temp;
}

};



void parser(char * name_diff,char *  name,std::vector<vec<float> >  & coords, std::vector<vec<int> >  & triangles, std::vector<vec<float> > & norm, std::vector<vec<int> >  & norm_triangles, std::vector<vec<float> > & text, std::vector<vec<int> >  & text_triangles, TGAImage & textures);
float max(float a, float b);
float min(float a, float b);

#endif /* PARSER_HPP_ */
