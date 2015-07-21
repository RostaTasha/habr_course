#include "tgaimage.h"
#include "parser.hpp"
#include <stdlib.h>
#include <cmath>
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void color_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color);
void color_triangle_s(int x01, int y0, int y1, int x2, int y2, TGAImage &image, TGAColor color);

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);

#define width 1000
#define height 1000


int main(int argc, char** argv) {
	int x_0,x_1,y_0,y_1;

	TGAImage image(width, height, TGAImage::RGB);
	//image.set(50, 50, red);
	std::vector<float>  coords;
	std::vector<int>  triangles;
	parser(coords, triangles);
	std::cout<<"coords size is "<<coords.size()<<std::endl;
	/*for (int i=0; i<coords.size();i+=3){
		//std::cout<<(int)(width*(coords[i]+1.0)/2)<<" "<<(int)(height*(coords[i+1]+1.0)/2)<<std::endl;
		image.set((int)(width*(coords[i]+1.0)/2),(int)(height*(coords[i+1]+1.0)/2), white);
	}*/

	float x_2,y_2;

	for (unsigned int j=0; j<triangles.size();j+=3){
	//for (unsigned int j=0; j<12;j+=3){
			x_0=(int)(width*(coords[triangles[j]*3-3]+1.0)/2);
			y_0=(int)(height*(coords[triangles[j]*3+1-3]+1.0)/2);
			x_1=(int)(width*(coords[triangles[j+1]*3-3]+1.0)/2);
			y_1=(int)(height*(coords[triangles[j+1]*3+1-3]+1.0)/2);
		line(x_0, y_0, x_1, y_1, image, white);
		x_0=(int)(width*(coords[triangles[j+1]*3-3]+1.0)/2);
		y_0=(int)(height*(coords[triangles[j+1]*3+1-3]+1.0)/2);
		x_1=(int)(width*(coords[triangles[j+2]*3-3]+1.0)/2);
		y_1=(int)(height*(coords[triangles[j+2]*3+1-3]+1.0)/2);
		x_2=x_0;
		y_2=y_0;
		line(x_0, y_0, x_1, y_1, image, white);
		x_0=(int)(width*(coords[triangles[j]*3-3]+1.0)/2);
		y_0=(int)(height*(coords[triangles[j]*3+1-3]+1.0)/2);
		x_1=(int)(width*(coords[triangles[j+2]*3-3]+1.0)/2);
		y_1=(int)(height*(coords[triangles[j+2]*3+1-3]+1.0)/2);
		line(x_0, y_0, x_1, y_1, image, white);
		color_triangle(x_0, y_0, x_1,y_1, x_2, y_2, image, TGAColor(rand()%255, rand()%255, rand()%255, 255));
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;

    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y1;
    for (int x=x1; x>=x0; x--) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;

        if (error2 > dx) {
            y -= (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

void color_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color){


float a_t;
float b_t;

float x_cross1,y_cross1;
float x_cross2,y_cross2;

line(x0, y0, x1, y1, image, red);
line(x0, y0, x2, y2, image, red);
line(x2, y2, x1, y1, image, red);


float u_border=max(max(y0,y1),y2);
float b_border=min(min(y0,y1),y2);

float r_border=max(max(x0,x1),x2);
float l_border=min(min(x0,x1),x2);
float up=false;

//line(r_border, u_border, r_border, b_border, image, blue);
//line(l_border, u_border, l_border, b_border, image, blue);
//line(r_border, b_border, l_border, b_border, image, blue);
//line(r_border, u_border, l_border, u_border, image, blue);


if (l_border==x0) {
std::swap(x0, x1);
std::swap(y0, y1);
}

if (l_border==x2) {
std::swap(x1, x2);
std::swap(y1, y2);
}


if (r_border==x0) {
std::swap(x0, x2);
std::swap(y0, y2);
}

if (r_border==x1) {
std::swap(x1, x2);
std::swap(y1, y2);
}

int cross_coord=(int)((x0-x1)*(float)(y2-y1)/(x2-x1)+y1);
color_triangle_s(x0, y0,cross_coord, x2, y2, image, color);

/*if (u_border==y0){
std::swap(x0, x2);
std::swap(y0, y2);
}

if (u_border==y1){
std::swap(x1, x2);
std::swap(y1, y2);
}

if (b_border==y0){
std::swap(x0, x1);
std::swap(y0, y1);
}

if (b_border==y2){
std::swap(x2, x1);
std::swap(y2, y1);
}*/




}


void color_triangle_s(int x01, int y0, int y1, int x2, int y2, TGAImage &image, TGAColor color){

	if (y0>y1)  std::swap(y0, y1);

	bool steep = false;

	    /*if (x01>x2) {
	        std::swap(x01, x2);
	        std::swap(y0, y1);
	    }*/
	    int dx = x2-x01;
	    int dy = y2-y0;
	    int dy_	= y2-y1;

	    int derror2 = std::abs(dy)*2;
	    int derror2_ = std::abs(dy_)*2;
	    int error2 = 0;
	    int error2_ = 0;
	    int y = y2;
	    int y_=y2;
	    for (int x=x2; x>=x01; x--) {
	    		for (int t=y; t<=y_; t++)
	            image.set(x, t, color);
	        error2 += derror2;
	        error2_+=derror2_;

	        if (error2 > dx) {
	            y -= (y2>y0?1:-1);
	            error2 -= dx*2;
	        }

	        if (error2_ > dx) {
	            y_ -= (y1>y0?1:-1);
	            error2 -= dx*2;
	        }
	    }


}



float max(float a, float b){
return (a>=b) ? a : b;
}

float min(float a, float b){
return (a>=b) ? b : a;
}
