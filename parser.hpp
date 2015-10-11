/*
 * parser.hpp
 *
 *  Created on: 06 θώνÿ 2015 γ.
 *      Author: Tata
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "libs.hpp"
#include "tgaimage.h"
#include "geometry.hpp"


using namespace std;






void parser(char * name_diff,char *  name,std::vector<vect<3,float> >  & coords, std::vector<vect<3,int> >  & triangles, std::vector<vect<3,float> > & norm, std::vector<vect<3,int> >  & norm_triangles, std::vector<vect<3,float> > & text, std::vector<vect<3,int> >  & text_triangles, TGAImage & textures);
float max(float a, float b);
float min(float a, float b);

#endif /* PARSER_HPP_ */
