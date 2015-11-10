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

class Model{
public:
	std::vector<vect<3,vect<3,float> > >  coords_tri;
	std::vector<vect<3,vect<3,float> > > norm_tri;
	std::vector<vect<3,vect<2,float> > > text_tri;
	TGAImage  textures;
	TGAImage norms;
	TGAImage gls;

};




void parser(char * name_diff,char *  name, char * name_norm, char * name_gls, Model &  mdl);
float max(float a, float b);
float min(float a, float b);

#endif /* PARSER_HPP_ */
