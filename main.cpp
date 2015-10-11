

#include "parser.hpp"
#include "matrix.h"
#include "geometry.hpp"

struct Shader  {
    matrix<4,3,float> varying_tri; // triangle coordinates (screen space), written by VS, read by FS
};




vect<3,float> xy_to_bc(vect<3,int> * abc, vect<3,int> p );
vect<3,int> bc_to_xy(vect<3,int> * abc, vect<3,float> bar );

float get_inten(vec<int> p, vec<int> p1, vec<int> p2, float inten_1, float inten_2);

void color_triangle(Shader shdr, TGAImage &image, float inten_0, float inten_1, float inten_2, mtrx2d<int> & z_buffer, vect<3,float> te0, vect<3,float> te1, vect<3,float> te2, TGAImage &text);

//float sqrt3(int x, int y, int z);

matrix<4,4,float> lookat(vect<3,float> &  eye, vect<3,float> &  center, vect<3,float>& up);
matrix<4,4,float> viewport(int x, int y, int w, int h, int depth);

int round(float a);
matrix<4,4,float> perspective(int z_cam);
matrix<4,4,float> viewport(int x, int y, int w, int h, int depth);

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


	std::vector<string> names;
	std::vector<string> diffs;
	names.push_back("african_head.obj");
	names.push_back("floor.obj");

	diffs.push_back("african_head_diffuse.tga");
	diffs.push_back("floor_diffuse.tga");

	char name_file[40];
	char name_file_diff[40];

	//char name_file1[40]="floor.obj";
		//char name_file_diff1[40]="floor_diffuse.tga";
		//parser(name_file_diff1,name_file1,real_coords, triangles, norm, norm_triangles, text, text_triangles, text_file);

	for (int i =0; i<2; i++){

	strcpy(name_file,names[i].c_str());
	strcpy(name_file_diff,diffs[i].c_str());
	printf("file %s %s\n",name_file,name_file_diff);



	real_coords.clear();
	triangles.clear();
	norm.clear();
	norm_triangles.clear();
	text.clear();
	text_triangles.clear();
	text_file.clear();
	parser(name_file_diff,name_file,real_coords, triangles, norm, norm_triangles, text, text_triangles, text_file);

    vect<3,float> eye;
    eye[0]=1; eye[1]=1; eye[2]=3;
    vect<3,float> center;
    center[0]=0; center[1]=0; center[2]=0;
    vect<3,float> up;
    up[0]=0; up[1]=1; up[2]=0;
    vect<3,float> light_vec;
	light_vec[0]=-1; light_vec[1]=-1; light_vec[2]=-1;
	light_vec=light_vec.normalize();



    matrix<4,4,float> ViewPortMtx=viewport(0, 0, width, height, zeight);
    matrix<4,4,float> PerspMtx=perspective((eye-center).norm());
    matrix<4,4,float> view = lookat(eye, center,up);


    matrix<4,4,float> mtrx = ViewPortMtx*PerspMtx*view;
    matrix<4,4,float> mtrx_=mtrx.Adjacent();



//	vect<4,float> light_vec_;

//	light_vec_=((mtrx*matrix<4,1,float>::make_from_vec_col(light_vec_)).col(0)).normalize();
//	vect<3,float> light_vec(light_vec_[0],light_vec_[1],light_vec_[2]);



	for(std::vector<int>::size_type i = 0; i != triangles.size(); i++) {
		vect<3,float> tr[3];
		vect<3,float> n[3];
		vect<3,float> te[3];


			tr[0][0]=real_coords[triangles[i].x-1].x; tr[0][1]=real_coords[triangles[i].x-1].y; tr[0][2]=real_coords[triangles[i].x-1].z;
			tr[1][0]=real_coords[triangles[i].y-1].x; tr[1][1]=real_coords[triangles[i].y-1].y; tr[1][2]=real_coords[triangles[i].y-1].z;
			tr[2][0]=real_coords[triangles[i].z-1].x; tr[2][1]=real_coords[triangles[i].z-1].y; tr[2][2]=real_coords[triangles[i].z-1].z;

			n[0][0]=norm[norm_triangles[i].x-1].x; n[0][1]=norm[norm_triangles[i].x-1].y; n[0][2]=norm[norm_triangles[i].x-1].z;
			n[1][0]=norm[norm_triangles[i].y-1].x; n[1][1]=norm[norm_triangles[i].y-1].y; n[1][2]=norm[norm_triangles[i].y-1].z;
			n[2][0]=norm[norm_triangles[i].z-1].x; n[2][1]=norm[norm_triangles[i].z-1].y; n[2][2]=norm[norm_triangles[i].z-1].z;

			te[0][0]=text[text_triangles[i].x-1].x; te[0][1]=text[text_triangles[i].x-1].y; te[0][2]=text[text_triangles[i].x-1].z;
			te[1][0]=text[text_triangles[i].y-1].x; te[1][1]=text[text_triangles[i].y-1].y; te[1][2]=text[text_triangles[i].y-1].z;
			te[2][0]=text[text_triangles[i].z-1].x; te[2][1]=text[text_triangles[i].z-1].y; te[2][2]=text[text_triangles[i].z-1].z;



		vect<3,float> norm_vec = (vect_mult(tr[2]-tr[0], tr[1]-tr[0])).normalize();


		float cos_alf=light_vec*norm_vec;

		//printf("cos %f\n",cos_alf);


		float cos[3];

		for (int k=0; k<3;k++ ){

			n[k]=n[k].normalize();
			vect<3,float> temp=n[k];
			cos[k]=light_vec*temp;

		}
		//printf("n %f %f %f %f %f %f\n", n[2][0],n[2][1],n[2][2],cos[0],cos[1],cos[2]);

	     vect<4,float> v[3];


	     Shader shdr;

	     for (int j = 0; j<3; j++){
	     for (int k =0; k<3; k++){
	    	 v[j][k]=tr[j][k];
	     }
	     v[j][3]=1;
	     }

	    vect<4,float> res0 =(mtrx*matrix<4,1,float>::make_from_vec_col(v[0])).col(0);
	    vect<4,float> res1 =(mtrx*matrix<4,1,float>::make_from_vec_col(v[1])).col(0);
	    vect<4,float> res2 =(mtrx*matrix<4,1,float>::make_from_vec_col(v[2])).col(0);

	    matrix<4,3,float> temp;
	    for (int i=0; i<4;i++){
	    	temp[i][0]=res0[i];
	    	temp[i][1]=res1[i];
	    	temp[i][2]=res2[i];
	    	}

	    shdr.varying_tri=temp;

	     color_triangle(shdr, image,-cos[0], -cos[1], -cos[2], z_buffer,te[0],te[1],te[2],text_file);

	}
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

int round(float a){
	return (int)(a+0.5);
}


void color_triangle(Shader shdr,TGAImage &image, float inten_0, float inten_1, float inten_2, mtrx2d<int> & z_buffer, vect<3,float> te0, vect<3,float> te1, vect<3,float> te2, TGAImage &text){

//	int z_cam= 1400;

//	perspective(p0 ,z_cam);
//	perspective(p1 ,z_cam);
//	perspective(p2 ,z_cam);

	Shader sh_temp;





vec<int> p0; p0.x=shdr.varying_tri[0][0]/shdr.varying_tri[3][0]; p0.y=shdr.varying_tri[1][0]/shdr.varying_tri[3][0]; p0.z=shdr.varying_tri[2][0]/shdr.varying_tri[3][0];;
vec<int> p1; p1.x=shdr.varying_tri[0][1]/shdr.varying_tri[3][1]; p1.y=shdr.varying_tri[1][1]/shdr.varying_tri[3][1]; p1.z=shdr.varying_tri[2][1]/shdr.varying_tri[3][1];
vec<int> p2; p2.x=shdr.varying_tri[0][2]/shdr.varying_tri[3][2]; p2.y=shdr.varying_tri[1][2]/shdr.varying_tri[3][2]; p2.z=shdr.varying_tri[2][2]/shdr.varying_tri[3][2];




//printf("p0 %d %d %d\n p1 %d %d %d\n p2 %d %d %d\n\n",p0.x,p0.y,p0.z,p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);

int u_border=max(max(p0.y,p1.y),p2.y);
int b_border=min(min(p0.y,p1.y),p2.y);

int r_border=max(max(p0.x,p1.x),p2.x);
int l_border=min(min(p0.x,p1.x),p2.x);


//printf("u %d b %d r %d l %d\n",u_border,b_border,r_border,l_border);

vect<3,int> triangle[3];
triangle[0]=vect<3,int>(p0.x,p0.y,p0.z);
triangle[1]=vect<3,int>(p1.x,p1.y,p1.z);
triangle[2]=vect<3,int>(p2.x,p2.y,p2.z);
vect<3,int> p;

float inten_cur;
vect<3,float> temp_clip;
vect<3,float> temp;

for (p[0]=l_border; p[0]<r_border; p[0]++){
	for (p[1]=b_border; p[1]<u_border; p[1]++){
		if (!((p[0]<width && p[1]< height && p[0]>=0 && p[1]>=0))) continue;
		//printf("temp is %f %f %f\n",temp[0],temp[1],temp[2]);
		//printf("p[0] %d p[1] %d \n",p[0],p[1]);
		temp=xy_to_bc(triangle,p);


		temp_clip[0]=temp[0]/shdr.varying_tri[3][0];
		temp_clip[1]=temp[1]/shdr.varying_tri[3][1];
		temp_clip[2]=temp[2]/shdr.varying_tri[3][2];

		temp_clip=temp_clip/(temp_clip[0]+temp_clip[1]+temp_clip[2]);
		//printf("temp_clip is %f %f %f\n",temp_clip[0],temp_clip[1],temp_clip[2]);

		p[2]=int(temp_clip*vect<3,float>(float(triangle[0][2]),float(triangle[1][2]),float(triangle[2][2])));
		if ((temp[0]<0 || temp[1]<0 || temp[2]<0 || z_buffer(p[0],p[1]) > p[2]) ) continue;




		vect<3,float> te_cur;
		te_cur[0]=temp_clip*vect<3,float>(te0[0],te1[0],te2[0]);
		te_cur[1]=temp_clip*vect<3,float>(te0[1],te1[1],te2[1]);
		te_cur[2]=temp_clip*vect<3,float>(te0[2],te1[2],te2[2]);
		//printf("1 %f %f %f 2 %f %f %f 3 %f %f %f\n",te0[0],te0[1],te0[2],te1[0],te1[1],te1[2],te2[0],te2[1],te2[2]);
		//printf("te_cur is %f %f \n",te_cur[0],te_cur[1]);

			int x = round(text.get_width()*te_cur[0]);
			int y = round(text.get_height()*(1.-te_cur[1]));

			//printf("x %d %d y %d %d\n",x,text.get_width(),y,text.get_height());
			TGAColor color1=text.get(x,y);


			inten_cur=(temp_clip*vect<3,float>(inten_0,inten_1,inten_2));
			//get_inten(p_cur, p_down, p_up, inten_down, inten_up);

			int color_code = (int)(((inten_cur))*255);
			color_code = std::max(std::min(color_code, 255), 0);
			//color_code=abs(color_code) % 255;
			//TGAColor color(color_code,color_code, color_code, 255);
			inten_cur = std::max(std::min(inten_cur,1.0f), 0.0f);
			TGAColor color2(round(((int)color1.r)*inten_cur),round(((int)color1.g)*inten_cur),round(((int)color1.b)*inten_cur),255);


		image.set(p[0], p[1], color2);
		 z_buffer(p[0],p[1])=p[2];


	}
}


return;
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


matrix<4,4,float> perspective(int z_cam){
matrix<4,4,float> mtrx = matrix<4,4,float>::ones();
mtrx[3][2]=-1.f/z_cam;
return mtrx;
}



matrix<4,4,float> viewport(int x, int y, int w, int h, int depth) {
	matrix<4,4,float> m = matrix<4,4,float>::ones();
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}


matrix<4,4,float> lookat(vect<3,float> &  eye, vect<3,float> &  center, vect<3,float>& up) {

	vect<3,float> z_= (eye-center).normalize();
	vect<3,float> y_=up.normalize();
	vect<3,float> x_=vect_mult(y_,z_).normalize();
matrix<4,4,float> new_basis = matrix<4,4,float>::ones();
matrix<4,4,float> shift = matrix<4,4,float>::ones();

for (int i=0; i<3; i++) {
	new_basis[0][i] = x_[i];
	new_basis[1][i] = y_[i];
	new_basis[2][i] = z_[i];
    shift[i][3] = -center[i];
   }

return shift*new_basis;

}




vect<3,int> bc_to_xy(vect<3,int> * abc, vect<3,float> bar ){
return vect<3,int>(int(bar[0]*abc[0][0]+bar[1]*abc[1][0]+bar[2]*abc[2][0]),int(bar[0]*abc[0][1]+bar[1]*abc[1][1]+bar[2]*abc[2][1]),int(bar[0]*abc[0][2]+bar[1]*abc[1][2]+bar[2]*abc[2][2]));
}


vect<3,float> xy_to_bc(vect<3,int> * abc, vect<3,int> p ){
vect<3,int> ab = abc[1]-abc[0];
vect<3,int> ac = abc[2]-abc[0];
vect<3,int> pa= abc[0]-p;
vect<3,float> nn=vect<3,float>(float(ab[0]),float(ac[0]),float(pa[0]));
vect<3,float> mm=vect<3,float>(float(ab[1]),float(ac[1]),float(pa[1]));
vect<3,float> res = vect_mult(nn,mm);
//printf("res  %f %f %f\n",res[0],res[1],res[2]);
if (std::fabs(res[2])<1.f) return vect<3,float>(-1.f,1.f,1.f);
return vect<3,float>(1.f-float(res[0]+res[1])/res[2],float(res[0])/res[2],float(res[1])/res[2]);
}
