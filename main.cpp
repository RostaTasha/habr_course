#include "tgaimage.h"
#include "parser.hpp"








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
	vector<d3<float> > real_coords;
	TGAImage image(width, height, TGAImage::RGB);
	//image.set(50, 50, red);
	std::vector<d3<int> >  triangles;
	parser(real_coords, triangles);


    vecd3<float> light_vec = vecd3<float>((float)(0.),(float)(0.),(float)(-1.));
	for(std::vector<int>::size_type i = 0; i != triangles.size(); i++) {

		d3<float> tr_0=real_coords[triangles[i].x-1];
		d3<float> tr_1=real_coords[triangles[i].y-1];
		d3<float> tr_2=real_coords[triangles[i].z-1];

		vecd3<float> norm_vec = (vecd3<float>(tr_2,tr_0))^(vecd3<float>(tr_1,tr_0));
		norm_vec.normalize();

		float cos_alf=light_vec*norm_vec;
		cout<<cos_alf<<endl;


	    if (cos_alf>0){
	     int color_code = (int)(((cos_alf))*255);
		color_triangle(width*(tr_0.x+1.)/2., height*(tr_0.y+1.)/2., width*(tr_1.x+1.)/2., height*(tr_1.y+1.)/2., width*(tr_2.x+1.)/2., height*(tr_2.y+1.)/2., image, TGAColor(color_code,color_code, color_code, 255));
	}
	   //  color_triangle(width*(tr_0.x+1.)/2., height*(tr_0.y+1.)/2., width*(tr_1.x+1.)/2., height*(tr_1.y+1.)/2., width*(tr_2.x+1.)/2., height*(tr_2.y+1.)/2., image, TGAColor(rand()%255,rand()%255, rand()%255, 255));




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
