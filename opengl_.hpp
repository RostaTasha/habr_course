#ifndef OPENGL_
#define OPENGL_

#include "libs.hpp"
#include "tgaimage.h"
#include "geometry.hpp"
#include "parser.hpp"

#define width 1000
#define height 1000
#define zeight 1000

struct IShader{
    virtual ~IShader(){};
    virtual vect<4,float> vertex(int iface, int nthvert,Model * model) = 0;
    virtual bool fragment(vect<3,float> bar, TGAColor &color) = 0;
    TGAImage * textures;
    TGAImage * norms;
    TGAImage * gloss;
    vect<3,float> varying_intensity; // written by vertex shader, read by fragment shader
    matrix<2,3,float> varying_uv;  // triangle uv coordinates, written by the vertex shader, read by the fragment shader
    matrix<4,3,float> varying_tri; // triangle coordinates (clip coordinates), written by VS, read by FS
    matrix<3,3,float> varying_nrm; // normal per vertex to be interpolated by FS
};







vect<3,float> xy_to_bc(vect<3,int> * abc, vect<3,int> p );

void color_triangle(IShader & shdr, TGAImage &image,  mtrx2d<int> & z_buffer);



#endif
