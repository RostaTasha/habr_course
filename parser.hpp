/*
 * parser.hpp
 *
 *  Created on: 06 θώνÿ 2015 γ.
 *      Author: Tata
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <iostream>
#include <fstream>
#include<vector>
#include <sstream>

class float3 {
public:
	float3(float a=0,float b=0, float c=0){
	f1=a;
	f2=b;
	f3=c;
	}
	float get_1(){
	return f1;
	}
	float get_2(){
	return f2;
	}
	float get_3(){
	return f3;
	}

	void set(float a, float  b ,float c){
	f1=a;
	f2=b;
	f3=c;
	}
private:
  float f1;
  float f2;
  float f3;
} ;


void parser(std::vector<float> & coords, std::vector<int>  & triangles);
float max(float a, float b);
float min(float a, float b);

#endif /* PARSER_HPP_ */
