#include "tgaimage.h"
#include "parser.hpp"







float get_inten(d3<int> p, d3<int> p1, d3<int> p2, float inten_1, float inten_2);
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void color_triangle(d3<int> p0, d3<int> p1, d3<int> p2, TGAImage &image, float inten_0, float inten_1, float inten_2, mtrx2d<int> & z_buffer);
void color_triangle_s(d3<int> p0, d3<int> p1, d3<int> p2, TGAImage &image, float inten_0, float inten_1,float inten_2, mtrx2d<int> & z_buffer);
float sqrt3(int x, int y, int z);

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);

#define width 1000
#define height 1000
#define zeight 1000

int main(int argc, char** argv) {
	vector<d3<float> > real_coords;
	vector<d3<float> > norm;
	TGAImage image(width, height, TGAImage::RGB);
	std::vector<d3<int> >  triangles;
	std::vector<d3<int> >  norm_triangles;

	mtrx2d<int> z_buffer(width,height,std::numeric_limits<int>::min());

	parser(real_coords, triangles, norm, norm_triangles);


    vecd3<float> light_vec = vecd3<float>((float)(0.),(float)(0.),(float)(-1.));
	for(std::vector<int>::size_type i = 0; i != triangles.size(); i++) {

		d3<float> tr_0=real_coords[triangles[i].x-1];
		d3<float> tr_1=real_coords[triangles[i].y-1];
		d3<float> tr_2=real_coords[triangles[i].z-1];

		d3<float> n_0=norm[norm_triangles[i].x-1];
		d3<float> n_1=norm[norm_triangles[i].y-1];
		d3<float> n_2=norm[norm_triangles[i].z-1];

		vecd3<float> norm_vec = (vecd3<float>(tr_2,tr_0))^(vecd3<float>(tr_1,tr_0));

		norm_vec.normalize();
		light_vec.normalize();

		float cos_alf=light_vec*norm_vec;
		cout<<cos_alf<<endl;

		float cos_0=light_vec*vecd3<float>(n_0,d3<float>(0,0,0));
		float cos_1=light_vec*vecd3<float>(n_1,d3<float>(0,0,0));
		float cos_2=light_vec*vecd3<float>(n_2,d3<float>(0,0,0));


	    if (cos_alf>0){
	     int color_code = (int)(((cos_alf))*255);
	     d3<int> p0(width*(tr_0.x+1.)/2.,height*(tr_0.y+1.)/2., zeight*(tr_0.z+1.)/2.);
	     d3<int> p1(width*(tr_1.x+1.)/2.,height*(tr_1.y+1.)/2., zeight*(tr_1.z+1.)/2.);
	     d3<int> p2(width*(tr_2.x+1.)/2.,height*(tr_2.y+1.)/2., zeight*(tr_2.z+1.)/2.);
		color_triangle(p0,p1,p2, image,cos_0, cos_1, cos_2, z_buffer);
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

void color_triangle(d3<int> p0, d3<int> p1, d3<int> p2, TGAImage &image, float inten_0, float inten_1, float inten_2, mtrx2d<int> & z_buffer){





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
std::swap(inten_0, inten_1);
}

if (l_border==p2.x) {
std::swap(p1, p2);
std::swap(inten_1, inten_2);
}


if (r_border==p0.x) {
std::swap(p0, p2);
std::swap(inten_0, inten_2);
}

if (r_border==p1.x) {
std::swap(p1, p2);
std::swap(inten_1, inten_2);
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
float inten_cross =get_inten(cross_p, p1, p2, inten_1, inten_2);
color_triangle_s(p0,cross_p, p2, image, inten_0,inten_cross,inten_2, z_buffer);
color_triangle_s(p0, cross_p, p1, image, inten_0,inten_cross,inten_1, z_buffer);
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


void color_triangle_s(d3<int> p0, d3<int> p1, d3<int> p2, TGAImage &image, float inten_0, float inten_1,float inten_2, mtrx2d<int> & z_buffer){


	if (p0.y>p1.y) {
		std::swap(p0, p1);
		std::swap(inten_0, inten_1);
	}


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

//////////////*//////////////////
////////////\ *	 \///////////////
//////////\	  *		\////////////
////////\	  *			\////////
//////\		  *				*////
////\         *\     ////////////
//*//////////////////////////////
////////////////////////////////
//
float A1=((float)(y2-y1))/(x2-x01);
float B1 = y1-A1*x01;

float A1_=((float)(z2-z1))/(x2-x01);
float B1_ = z1-A1_*x01;

float A0=((float)(y2-y0))/(x2-x01);
float B0 = y0-A0*x01;

float A0_=((float)(z2-z0))/(x2-x01);
float B0_ = z0-A0_*x01;

int max_v=(x01<x2) ? x2: x01;
int min_v=(x01<x2) ? x01: x2;


for (int t=min_v; t<=max_v; t++){
	int y_u = (int)(A1*t+B1);
	int y_b = (int)(A0*t+B0);
	z_limitu=A1_*t+B1_;
	z_limitb=A0_*t+B0_;
	d3<int> p_up(t,y_u,z_limitu);
	d3<int> p_down(t,y_b,z_limitb);
	float inten_up =get_inten(p_up, p1, p2, inten_1, inten_2);
	float inten_down =get_inten(p_down, p0, p2, inten_0, inten_2);

	for (int k = y_b; k<=y_u; k++){
		float Acur=((float)(p_up.z-p_down.z))/(p_up.y-p_down.y);
		float Bcur = p_down.z-Acur*p_down.y;
		z_cur=Acur*k+Bcur;
		d3<int> p_cur(t,k,z_cur);
		float inten_cur=get_inten(p_cur, p_down, p_up, inten_down, inten_up);
		int color_code = (int)(((inten_cur))*255);
		TGAColor color(color_code,color_code, color_code, 255);
		if (z_buffer(t,k) < z_cur){
		 image.set(t, k, color);
		 z_buffer(t,k)=z_cur;
		}
	}

}



}





float get_inten(d3<int> p, d3<int> p1, d3<int> p2, float inten_1, float inten_2){

	int max_v = max(abs(p2.y-p1.y), max(abs(p2.x-p1.x),abs(p2.z-p1.z)));
	return (max_v==abs(p2.y-p1.y)) ? (inten_1+(p.y-p1.y)*(inten_2-inten_1)/(p2.y-p1.y)) : (max_v==abs(p2.x-p1.x)) ?
			(inten_1+(p.x-p1.x)*(inten_2-inten_1)/(p2.x-p1.x)) : (inten_1+(p.z-p1.z)*(inten_2-inten_1)/(p2.z-p1.z));
}


float max(float a, float b){
return (a>=b) ? a : b;
}

float min(float a, float b){
return (a>=b) ? b : a;
}
