
#include "libs.hpp"
#include "parser.hpp"
#include "matrix.h"
#include "geometry.hpp"
#include "opengl_.hpp"




matrix<4,4,float> lookat(vect<3,float> &  eye, vect<3,float> &  center, vect<3,float>& up);
matrix<4,4,float> viewport(int x, int y, int w, int h, int depth);
matrix<4,4,float> perspective(int z_cam);
matrix<4,4,float> viewport(int x, int y, int w, int h, int depth);


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);





int main(int argc, char** argv) {
	vector<vect<3,float> > real_coords;
	vector<vect<3,float> > norm;
	vector<vect<3,float> > text;
	TGAImage image(width, height, TGAImage::RGB);
	TGAImage text_file;
	std::vector<vect<3,int> >  triangles;
	std::vector<vect<3,int> >  norm_triangles;
	std::vector<vect<3,int> >  text_triangles;

	mtrx2d<int> z_buffer(width,height,std::numeric_limits<int>::min());


	std::vector<string> names;
	std::vector<string> diffs;
	names.push_back("african_head.obj");
	names.push_back("floor.obj");
	names.push_back("african_head_eye_inner.obj");

	diffs.push_back("african_head_diffuse.tga");
	diffs.push_back("floor_diffuse.tga");
	diffs.push_back("african_head_eye_inner_diffuse.tga");



	char name_file[40];
	char name_file_diff[40];



	for (int i =0; i<3; i++){

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


	vect<3,float> tr[3];
	vect<3,float> n[3];
	vect<3,float> te[3];
	float cos[3];

	vect<4,float> res0;
	vect<4,float> res1;
	vect<4,float> res2;

    vect<4,float> v[3];
    Shader shdr;

	for(std::vector<int>::size_type i = 0; i != triangles.size(); i++) {

		for (int j=0; j<3; j++){
			for (int k=0; k<3; k++){
				tr[j][k]=real_coords[triangles[i][j]-1][k];
				n[j][k]=norm[norm_triangles[i][j]-1][k];
				te[j][k]=text[text_triangles[i][j]-1][k];
			}
		}

		for (int k=0; k<3;k++ ){

			n[k]=n[k].normalize();
			vect<3,float> temp=n[k];
			cos[k]=light_vec*temp;
		}


	     for (int j = 0; j<3; j++){
	    	 for (int k =0; k<3; k++){
	    		 v[j][k]=tr[j][k];
	    	 }
	    	 v[j][3]=1;
	     }

	    res0 =(mtrx*matrix<4,1,float>::make_from_vec_col(v[0])).col(0);
	    res1 =(mtrx*matrix<4,1,float>::make_from_vec_col(v[1])).col(0);
	    res2 =(mtrx*matrix<4,1,float>::make_from_vec_col(v[2])).col(0);

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


