#include "tgaimage.h"
#include "parser.hpp"
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <algorithm>    // std::sort

using namespace std;


class threed{
public:
threed (int a, int b, int c){
x=a;
y=b;
z=c;
}
threed(){
x=0;
y=0;
z=0;
}


int x;
int y;
int z;
};


class triangle{
public:
threed p0;
threed p1;
threed p2;
triangle(threed a0, threed a1, threed a2){
p0=a0;
p1=a1;
p2=a2;
}

threed get_norm(){

	int norm_x=(p2.y-p0.y)*(p1.z-p0.z)-(p1.y-p0.y)*(p2.z-p0.z);
	int norm_y=(p1.x-p0.x)*(p2.z-p0.z)-(p2.x-p0.x)*(p1.z-p0.z);
	int norm_z=(p2.x-p0.x)*(p1.y-p0.y)-(p1.x-p0.x)*(p2.y-p0.y);


return threed(norm_x,norm_y,norm_z);
}



float mean_h(int i=2){
	switch (i)
	      {
	         case 0:
	            return (float)(p1.x+p2.x+p0.x)/3.;
	         case 1:
	        	 return (float)(p1.y+p2.y+p0.y)/3.;
	         default:
	         	 return (float)(p1.z+p2.z+p0.z)/3.;
	      }
}

};

bool mean_h(triangle t1, triangle t2){

return t1.mean_h()<t2.mean_h();

}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void color_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color);
void color_triangle_s(int x01, int y0, int y1, int x2, int y2, TGAImage &image, TGAColor color);
float sqrt3(int x, int y, int z);

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);

#define width 1000
#define height 1000
#define zeight 1000

int main(int argc, char** argv) {
	int x_0,x_1,y_0,y_1,z_0,z_1, x_2,y_2,z_2;
	vector<triangle> vectriangles;
	TGAImage image(width, height, TGAImage::RGB);
	//image.set(50, 50, red);
	std::vector<float>  coords;
	std::vector<int>  triangles;
	parser(coords, triangles);
	std::cout<<"coords size is "<<coords.size()<<std::endl;
	std::cout<<"triangles size is "<<triangles.size()<<std::endl;
	/*for (int i=0; i<coords.size();i+=3){
		//std::cout<<(int)(width*(coords[i]+1.0)/2)<<" "<<(int)(height*(coords[i+1]+1.0)/2)<<std::endl;
		image.set((int)(width*(coords[i]+1.0)/2),(int)(height*(coords[i+1]+1.0)/2), white);
	}*/

	for (unsigned int j=0; j<triangles.size();j+=3){
	//for (unsigned int j=0; j<12;j+=3){
			x_0=(int)(width*(coords[triangles[j]*3-3]+1.0)/2);
			y_0=(int)(height*(coords[triangles[j]*3+1-3]+1.0)/2);
			z_0= (int)(zeight*(coords[triangles[j]*3+2-3]+1.0)/2);
			x_1=(int)(width*(coords[triangles[j+1]*3-3]+1.0)/2);
			y_1=(int)(height*(coords[triangles[j+1]*3+1-3]+1.0)/2);
			z_1=(int)(zeight*(coords[triangles[j+1]*3+2-3]+1.0)/2);
			x_2=(int)(width*(coords[triangles[j+2]*3-3]+1.0)/2);
			y_2=(int)(height*(coords[triangles[j+2]*3+1-3]+1.0)/2);
			z_2=(int)(zeight*(coords[triangles[j+2]*3+1-3]+1.0)/2);
			vectriangles.push_back(triangle(threed(x_0,y_0,z_0),threed(x_1,y_1,z_1),threed(x_2,y_2,z_2)));
	}


	sort(vectriangles.begin(), vectriangles.end(), mean_h);
//		line(x_0, y_0, x_1, y_1, image, white);

//		line(x_1, y_1, x_2, y_2, image, white);

//		line(x_0, y_0, x_2, y_2, image, white);
    int light_x=0;
    int light_y=0;
    int light_z=-1;
    printf("size is %d",vectriangles.size() );
	for(std::vector<int>::size_type i = 0; i != vectriangles.size(); i++) {
		threed temp=vectriangles[i].get_norm();

		float cos_alf = (light_x*temp.x+light_y*temp.y+light_z*temp.z)/sqrt3(temp.x,temp.y,temp.z)/sqrt3(light_x,light_y,light_z);


	     if (cos_alf>0){
	     int color_code = (int)(((cos_alf))*255);
		color_triangle(vectriangles[i].p0.x, vectriangles[i].p0.y, vectriangles[i].p1.x,vectriangles[i].p1.y, vectriangles[i].p2.x, vectriangles[i].p2.y, image, TGAColor(color_code,color_code, color_code, 255));
	}





	}



	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;


}
float sqrt3(int x, int y, int z){
return sqrtf((float)x*x+(float)y*y+(float)z*z);
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




//line(x0, y0, x1, y1, image, red);
//line(x0, y0, x2, y2, image, red);
//line(x2, y2, x1, y1, image, red);


int u_border=max(max(y0,y1),y2);
int b_border=min(min(y0,y1),y2);

int r_border=max(max(x0,x1),x2);
int l_border=min(min(x0,x1),x2);
//float up=false;

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

int cross_coord=(int)((x0-x1)*(float)(y2-y1)/(float)(x2-x1)+y1);
color_triangle_s(x0, y0,cross_coord, x2, y2, image, color);
color_triangle_s(x0, y0, cross_coord, x1, y1, image, color);
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

float A1=(float)(y2-y1)/(x2-x01);
float B1 = y1-A1*x01;

float A0=(float)(y2-y0)/(x2-x01);
float B0 = y0-A0*x01;



if (x01<x2){
for (int t=x01; t<x2; t++){
	for (int k = (int)(A0*t+B0); k<=(int)(A1*t+B1); k++)
		 image.set(t, k, color);

}

} else{

	for (int t=x2; t<x01; t++){
		for (int k = (int)(A0*t+B0); k<=(int)(A1*t+B1); k++)
			 image.set(t, k, color);

	}
}


}






float max(float a, float b){
return (a>=b) ? a : b;
}

float min(float a, float b){
return (a>=b) ? b : a;
}
