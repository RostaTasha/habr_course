#include "tgaimage.h"
#include "parser.hpp"








void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void color_triangle(d3<int> p0, d3<int> p1, d3<int> p2, TGAImage &image, TGAColor color, mtrx2d<int> & z_buffer);
void color_triangle_s(d3<int> p0, d3<int> p1, d3<int> p2, TGAImage &image, TGAColor color, mtrx2d<int> & z_buffer);
float sqrt3(int x, int y, int z);

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);

#define width 1000
#define height 1000
#define zeight 1000

int main(int argc, char** argv) {
	vector<d3<float> > real_coords;
	TGAImage image(width, height, TGAImage::RGB);
	std::vector<d3<int> >  triangles;

	mtrx2d<int> z_buffer(width,height,std::numeric_limits<int>::min());

	parser(real_coords, triangles);


    vecd3<float> light_vec = vecd3<float>((float)(0.),(float)(0.),(float)(-1.));
	for(std::vector<int>::size_type i = 0; i != triangles.size(); i++) {

		d3<float> tr_0=real_coords[triangles[i].x-1];
		d3<float> tr_1=real_coords[triangles[i].y-1];
		d3<float> tr_2=real_coords[triangles[i].z-1];

		vecd3<float> norm_vec = (vecd3<float>(tr_2,tr_0))^(vecd3<float>(tr_1,tr_0));

		norm_vec.normalize();
		light_vec.normalize();

		float cos_alf=light_vec*norm_vec;
		cout<<cos_alf<<endl;


	    if (cos_alf>0){
	     int color_code = (int)(((cos_alf))*255);
	     d3<int> p0(width*(tr_0.x+1.)/2.,height*(tr_0.y+1.)/2., zeight*(tr_0.z+1.)/2.);
	     d3<int> p1(width*(tr_1.x+1.)/2.,height*(tr_1.y+1.)/2., zeight*(tr_1.z+1.)/2.);
	     d3<int> p2(width*(tr_2.x+1.)/2.,height*(tr_2.y+1.)/2., zeight*(tr_2.z+1.)/2.);
		color_triangle(p0,p1,p2, image, TGAColor(color_code,color_code, color_code, 255), z_buffer);
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

void color_triangle(d3<int> p0, d3<int> p1, d3<int> p2, TGAImage &image, TGAColor color, mtrx2d<int> & z_buffer){





//line(x0, y0, x1, y1, image, red);
//line(x0, y0, x2, y2, image, red);
//line(x2, y2, x1, y1, image, red);


int u_border=max(max(p0.y,p1.y),p2.y);
int b_border=min(min(p0.y,p1.y),p2.y);

int r_border=max(max(p0.x,p1.x),p2.x);
int l_border=min(min(p0.x,p1.x),p2.x);
//float up=false;

//line(r_border, u_border, r_border, b_border, image, blue);
//line(l_border, u_border, l_border, b_border, image, blue);
//line(r_border, b_border, l_border, b_border, image, blue);
//line(r_border, u_border, l_border, u_border, image, blue);


if (l_border==p0.x) {
std::swap(p0, p1);
}

if (l_border==p2.x) {
std::swap(p1, p2);
}


if (r_border==p0.x) {
std::swap(p0, p2);
}

if (r_border==p1.x) {
std::swap(p1, p2);
}


int x0=p0.x;
int y0=p0.y;
int z0=p0.z;
int x1=p1.x;
int y1=p1.y;
int z1=p1.z;
int x2=p2.x;
int y2=p2.y;
int z2=p2.z;

int cross_coord_y=(int)((x0-x1)*(float)(y2-y1)/(float)(x2-x1)+y1);
int cross_coord_z=(int)((x0-x1)*(float)(z2-z1)/(float)(x2-x1)+z1);
d3<int> cross_p(x0,cross_coord_y,cross_coord_z);
color_triangle_s(p0,cross_p, p2, image, color, z_buffer);
color_triangle_s(p0, cross_p, p1, image, color, z_buffer);
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


void color_triangle_s(d3<int> p0, d3<int> p1, d3<int> p2, TGAImage &image, TGAColor color, mtrx2d<int> & z_buffer){


	if (p0.y>p1.y)  std::swap(p0, p1);


	int x01=p0.x;
	int y0=p0.y;
	int z0=p0.z;
	int y1=p1.y;
	int z1=p1.z;
	int x2=p2.x;
	int y2=p2.y;
	int z2=p2.z;
	int z_cur=0;
	int z_limitu;
	int z_limitb;


float A1=(float)(y2-y1)/(x2-x01);
float B1 = y1-A1*x01;

float A1_=(float)(z2-z1)/(x2-x01);
float B1_ = z1-A1*x01;

float A0=(float)(y2-y0)/(x2-x01);
float B0 = y0-A0*x01;

float A0_=(float)(z2-z0)/(x2-x01);
float B0_ = z0-A0*x01;


if (x01<x2){
for (int t=x01; t<x2; t++){
	int y_u = (int)(A1*t+B1);
	int y_b = (int)(A0*t+B0);
	for (int k = y_b; k<=y_u; k++){
		z_limitu=A1_*t+B1_;
		z_limitb=A0_*t+B0_;
		float Acur=(float)(z_limitu-z_limitb)/(y_u-y_b);
		float Bcur = z_limitb-Acur*y_b;
		z_cur=Acur*k+Bcur;
		if (z_buffer(t,k) < z_cur){
		 image.set(t, k, color);
		 z_buffer(t,k)=z_cur;
		}
	}

}

} else{

	for (int t=x2; t<x01; t++){
		int y_u = (int)(A1*t+B1);
		int y_b = (int)(A0*t+B0);
		for (int k = y_b; k<=y_u; k++){
			z_limitu=A1_*t+B1_;
			z_limitb=A0_*t+B0_;
			float Acur=(float)(z_limitu-z_limitb)/(y_u-y_b);
			float Bcur = z_limitb-Acur*y_b;
			z_cur=Acur*k+Bcur;
			if (z_buffer(t,k) < z_cur){
			 image.set(t, k, color);
			 z_buffer(t,k)=z_cur;
			}
		}

	}
}


}






float max(float a, float b){
return (a>=b) ? a : b;
}

float min(float a, float b){
return (a>=b) ? b : a;
}
