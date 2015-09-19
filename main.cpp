#include "parser.hpp"
#include "matrix.h"
#include "geometry.hpp"


float get_inten(vec<int> p, vec<int> p1, vec<int> p2, float inten_1, float inten_2);
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void color_triangle(vec<int> p0, vec<int> p1, vec<int> p2, TGAImage &image, float inten_0, float inten_1, float inten_2, mtrx2d<int> & z_buffer, vec<float> te0, vec<float> te1, vec<float> te2, TGAImage &text);
void color_triangle_s(vec<int> p0, vec<int> p1, vec<int> p2, TGAImage &image, float inten_0, float inten_1,float inten_2, mtrx2d<int> & z_buffer, vec<float> te0, vec<float> te1, vec<float> te2, TGAImage &text);
//float sqrt3(int x, int y, int z);
int round(float a);
void perspective(vec<int> & p ,int z_cam);

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);

#define width 1000
#define height 1000
#define zeight 1000

int main(int argc, char** argv) {
	vector<vec<float> > real_coords;
	vector<vec<float> > norm;
	vector<vec<float> > text;
	TGAImage image(width, height, TGAImage::RGB);
	TGAImage text_file;
	std::vector<vec<int> >  triangles;
	std::vector<vec<int> >  norm_triangles;
	std::vector<vec<int> >  text_triangles;

	mtrx2d<int> z_buffer(width,height,std::numeric_limits<int>::min());


	parser(real_coords, triangles, norm, norm_triangles, text, text_triangles, text_file);


	vecvec<float> light_vec = vecvec<float>((float)(0.),(float)(0.),(float)(-1.));
	for(std::vector<int>::size_type i = 0; i != triangles.size(); i++) {

		vec<float> tr_0=real_coords[triangles[i].x-1];
		vec<float> tr_1=real_coords[triangles[i].y-1];
		vec<float> tr_2=real_coords[triangles[i].z-1];

		vec<float> n_0=norm[norm_triangles[i].x-1];
		vec<float> n_1=norm[norm_triangles[i].y-1];
		vec<float> n_2=norm[norm_triangles[i].z-1];

		vec<float> te_0=text[text_triangles[i].x-1];
		vec<float> te_1=text[text_triangles[i].y-1];
		vec<float> te_2=text[text_triangles[i].z-1];

		vecvec<float> norm_vec = (vecvec<float>(tr_2,tr_0))^(vecvec<float>(tr_1,tr_0));

		norm_vec.normalize();
		light_vec.normalize();

		float cos_alf=light_vec*norm_vec;

		vecvec<float> a0=(vecvec<float>(n_0.x,n_0.y,n_0.z));
		a0.normalize();
		vecvec<float> a1=(vecvec<float>(n_1.x,n_1.y,n_1.z));
		a1.normalize();
		vecvec<float> a2=(vecvec<float>(n_2.x,n_2.y,n_2.z));
		a2.normalize();

		float cos_0=light_vec*a0;
		float cos_1=light_vec*a1;
		float cos_2=light_vec*a2;

	    if (cos_alf>0){
	     int color_code = (int)(((cos_alf))*255);
	     vec<int> p0(round(width*(tr_0.x+1.)/2.),round(height*(tr_0.y+1.)/2.), round(zeight*(tr_0.z+1.)/2.));
	     vec<int> p1(round(width*(tr_1.x+1.)/2.),round(height*(tr_1.y+1.)/2.), round(zeight*(tr_1.z+1.)/2.));
	     vec<int> p2(round(width*(tr_2.x+1.)/2.),round(height*(tr_2.y+1.)/2.), round(zeight*(tr_2.z+1.)/2.));
		color_triangle(p0,p1,p2, image,-cos_0, -cos_1, -cos_2, z_buffer,te_0,te_1,te_2,text_file);
	    }
	   //  color_triangle(width*(tr_0.x+1.)/2., height*(tr_0.y+1.)/2., width*(tr_1.x+1.)/2., height*(tr_1.y+1.)/2., width*(tr_2.x+1.)/2., height*(tr_2.y+1.)/2., image, TGAColor(rand()%255,rand()%255, rand()%255, 255));
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

int round(float a){
	return (int)(a+0.5);
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

void color_triangle(vec<int> p0, vec<int> p1, vec<int> p2, TGAImage &image, float inten_0, float inten_1, float inten_2, mtrx2d<int> & z_buffer, vec<float> te0, vec<float> te1, vec<float> te2, TGAImage &text){

	int z_cam= 1400;

	perspective(p0 ,z_cam);
	perspective(p1 ,z_cam);
	perspective(p2 ,z_cam);


//line(p0.x, p0.y, p1.x, p1.y, image, red);
//line(p0.x, p0.y, p2.x, p2.y, image, red);
//line(p2.x, p2.y, p1.x, p1.y, image, red);
//	return;

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
std::swap(te0, te1);
}

if (l_border==p2.x) {
std::swap(p1, p2);
std::swap(inten_1, inten_2);
std::swap(te1, te2);
}


if (r_border==p0.x) {
std::swap(p0, p2);
std::swap(inten_0, inten_2);
std::swap(te0, te2);
}

if (r_border==p1.x) {
std::swap(p1, p2);
std::swap(inten_1, inten_2);
std::swap(te1, te2);
}

if (p0.x==p1.x && p0.x==p2.x) return;


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
vec<int> cross_p(x0,cross_coord_y,cross_coord_z);
float inten_cross =get_inten(cross_p, p1, p2, inten_1, inten_2);
vec<float> text_cross;
text_cross.x=get_inten(cross_p, p1, p2, te1.x, te2.x);
text_cross.y=get_inten(cross_p, p1, p2, te1.y, te2.y);


color_triangle_s(p0,cross_p, p2, image, inten_0,inten_cross,inten_2, z_buffer, te0, text_cross, te2,text);
color_triangle_s(p0, cross_p, p1, image, inten_0,inten_cross,inten_1, z_buffer, te0, text_cross, te1,text);
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


void color_triangle_s(vec<int> p0, vec<int> p1, vec<int> p2, TGAImage &image, float inten_0, float inten_1,float inten_2, mtrx2d<int> & z_buffer, vec<float> te0, vec<float> te1, vec<float> te2, TGAImage &text){

	//printf("%f %f %f\n", inten_0, inten_1, inten_2);

	if (p0.y>p1.y) {
		std::swap(p0, p1);
		std::swap(inten_0, inten_1);
		std::swap(te0, te1);
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

	if (x01==x2) return;

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
	vec<int> p_up(t,y_u,z_limitu);
	vec<int> p_down(t,y_b,z_limitb);
	float inten_up =get_inten(p_up, p1, p2, inten_1, inten_2);
	vec<float> text_up;
	text_up.x=get_inten(p_up, p1, p2, te1.x, te2.x);
	text_up.y=get_inten(p_up, p1, p2, te1.y, te2.y);


	float inten_down =get_inten(p_down, p0, p2, inten_0, inten_2);
	vec<float> text_down;
	text_down.x=get_inten(p_down, p0, p2, te0.x, te2.x);
	text_down.y=get_inten(p_down, p0, p2, te0.y, te2.y);


	for (int k = p_down.y; k<=p_up.y; k++){
		float Acur= ((float)(p_up.z-p_down.z))/(p_up.y-p_down.y);
		float Bcur =  p_down.z-Acur*p_down.y;
		z_cur=  Acur*k+Bcur;
		vec<int> p_cur(t,k,z_cur);
		float inten_cur=get_inten(p_cur, p_down, p_up, inten_down, inten_up);
		vec<float> text_cur;
		text_cur.x=get_inten(p_cur, p_down, p_up, text_down.x, text_up.x);
		text_cur.y=get_inten(p_cur, p_down, p_up, text_down.y, text_up.y);
		int x = round(text.get_width()*text_cur.x);
		int y = round(text.get_height()*(1.-text_cur.y));
		TGAColor color1=text.get(x,y);

		int color_code = (int)(((inten_cur))*255);
		color_code = std::max(std::min(color_code, 255), 0);
		//color_code=abs(color_code) % 255;
		//TGAColor color(color_code,color_code, color_code, 255);
		inten_cur = std::max(std::min(inten_cur,1.0f), 0.0f);
		TGAColor color2(round(((int)color1.r)*inten_cur),round(((int)color1.g)*inten_cur),round(((int)color1.b)*inten_cur),255);
		if (((t<width && k< height && t>=0 && k>=0))&& (z_buffer(t,k) < z_cur)){

		 image.set(t, k, color2);

		 z_buffer(t,k)=z_cur;
		}
	}

}
}

float get_inten(vec<int> p, vec<int> p1, vec<int> p2, float inten_1, float inten_2){

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


void perspective(vec<int> & p ,int z_cam){
p.x-=width/2;
p.y-=height/2;
/*float coeff=min((1.0f-(float)zeight/(float)z_cam)*2,1.0f);
float temp_x=coeff*((float)p.x)/(1.0f-(float)p.z/(float)z_cam);
float temp_y=coeff*((float)p.y)/(1.0f-(float)p.z/(float)z_cam);
float temp_z=coeff*((float)p.z)/(1.0f-(float)p.z/(float)z_cam);
p.x=(int)temp_x;
p.y=(int)temp_y;
p.z=(int)temp_z;
p.x+=width/2;
p.y+=height/2;*/
vect<4,float> vct;
vct[0]=p.x;
vct[1]=p.y;
vct[2]=p.z;
vct[3]=1;
matrix<4,4,float> mtrx = matrix<4,4,float>::ones();

mtrx[3][2]=-1.f/z_cam;


vect<4,float> res=mtrx*vct;

res=res/res[3];
p.x=(int)res[0]+width/2;
p.y=(int)res[1]+height/2;
p.z=(int)res[2];
}



