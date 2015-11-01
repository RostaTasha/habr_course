
#include "libs.hpp"
#include "parser.hpp"
#include "matrix.h"
#include "geometry.hpp"
#include "opengl_.hpp"
#include "limits.h"




matrix<4,4,float> lookat(vect<3,float> &  eye, vect<3,float> &  center, vect<3,float>& up);
matrix<4,4,float> viewport(int x, int y, int w, int h, int depth);
matrix<4,4,float> perspective(int z_cam);
matrix<4,4,float> viewport(int x, int y, int w, int h, int depth);


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);

matrix<4,4,float> ViewPortMtx;
matrix<4,4,float> PerspMtx;
matrix<4,4,float> view;


vect<3,float> eye(1.,1.,3.);
vect<3,float> center(0.,0.,0.);
vect<3,float> up(0.,1.,0.);
vect<3,float> light_vec(1.,1.,1.);


struct ShaderGuro : public IShader  {


    virtual vect<4,float> vertex(int iface, int nthvert, Model * model){
    	varying_intensity[nthvert] = std::max(0.f, (model->norm_tri[iface][nthvert]*light_vec)); // get diffuse lighting intensity
        vect<4,float> gl_Vertex;
        gl_Vertex[3]=1;
        for (int i =0; i<3; i++) gl_Vertex[i]= model->coords_tri[iface][nthvert][i];//embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
        gl_Vertex=ViewPortMtx*PerspMtx*view*gl_Vertex;

        //varying_nrm.set_col(nthvert, proj<3>((Projection*ModelView).invert_transpose()*embed<4>(model->normal(iface, nthvert), 0.f)));

        vect<4,float> gl_norm;
        gl_norm[0]=(model->norm_tri[iface][nthvert])[0];
        gl_norm[1]=(model->norm_tri[iface][nthvert])[1];
        gl_norm[2]=(model->norm_tri[iface][nthvert])[2];
        gl_norm=(ViewPortMtx*PerspMtx*view).Adjacent()*gl_norm;
        varying_nrm.setCol(nthvert,vect<3,float>(gl_norm[0],gl_norm[1],gl_norm[2]));

        varying_tri.setCol(nthvert, gl_Vertex);
        varying_uv.setCol(nthvert,model->text_tri[iface][nthvert]);

        textures=model->textures;
        return gl_Vertex; // transform it to screen coordinates
    }


     ~ShaderGuro(){
    }


    virtual bool fragment(vect<3,float> bar, TGAColor &color){

    	vect<3,float> temp_clip;

		for (int i=0; i<3; i++)temp_clip[i]=bar[i]/varying_tri[3][i];

		temp_clip=temp_clip/(temp_clip[0]+temp_clip[1]+temp_clip[2]);

		float inten_cur=(temp_clip*vect<3,float>(varying_intensity[0],varying_intensity[1],varying_intensity[2]));


    	vect<2,float> te_cur;
    	for (int i=0; i<2; i++) te_cur[i]=temp_clip*vect<3,float>(varying_uv[i][0],varying_uv[i][1],varying_uv[i][2]);

		int x = round(textures.get_width()*te_cur[0]);
		int y = round(textures.get_height()*(1.-te_cur[1]));
		color = (textures.get(x,y))*inten_cur;

    	return false;
    }

};





int main(int argc, char** argv) {

	TGAImage image(width, height, TGAImage::RGB);



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

    ViewPortMtx=viewport(0, 0, width, height, zeight);
    PerspMtx=perspective((eye-center).norm());
    view = lookat(eye, center,up);


	//light_dir = proj<3>((Projection*ModelView*embed<4>(light_dir, 0.f))).normalize();
	vect<4,float> gl_light;
	gl_light[0]=light_vec[0];
	gl_light[1]=light_vec[1];
	gl_light[2]=light_vec[2];

	gl_light=ViewPortMtx*PerspMtx*view*gl_light;
	light_vec=vect<3,float>(gl_light[0],gl_light[1],gl_light[2]);

	light_vec=light_vec.normalize();


	for (int i =0; i<names.size(); i++){

	strcpy(name_file,names[i].c_str());
	strcpy(name_file_diff,diffs[i].c_str());
	printf("file %s %s\n",name_file,name_file_diff);



	Model mdl;





	parser(name_file_diff,name_file,mdl);
	ShaderGuro shader;


	printf("OKI DOKI!\n");

	for(std::vector<int>::size_type i = 0; i != mdl.coords_tri.size(); i++) {

		 for (int j=0; j<3; j++) {
		     	 shader.vertex(i, j,&mdl);
		        }

	    color_triangle(shader, image, z_buffer);

	}
	}


	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


