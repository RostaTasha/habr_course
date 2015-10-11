#ifndef OPENGL_
#define OPENGL_

#include "libs.hpp"
#include "tgaimage.h"
#include "geometry.hpp"

#define width 1000
#define height 1000
#define zeight 1000


struct Shader  {
    matrix<4,3,float> varying_tri; // triangle coordinates (screen space), written by VS, read by FS
};




vect<3,float> xy_to_bc(vect<3,int> * abc, vect<3,int> p );

void color_triangle(Shader shdr, TGAImage &image, float inten_0, float inten_1, float inten_2, mtrx2d<int> & z_buffer, vect<3,float> te0, vect<3,float> te1, vect<3,float> te2, TGAImage &text);



#endif
