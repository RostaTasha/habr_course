
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
vect<3,float> light_vec(-1.,-1.,-1.);


struct ShaderGuro : public IShader  {


    virtual vect<4,float> vertex(int iface, int nthvert, Model * model){
    	varying_intensity[nthvert] = std::max(0.f, (-1.f)*(model->norm_tri[iface][nthvert]*light_vec)); // get diffuse lighting intensity
        vect<4,float> gl_Vertex;
        gl_Vertex[3]=1;
        for (int i =0; i<3; i++) gl_Vertex[i]= model->coords_tri[iface][nthvert][i];//embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
        gl_Vertex=ViewPortMtx*PerspMtx*view*gl_Vertex;

        //varying_nrm.set_col(nthvert, proj<3>((Projection*ModelView).invert_transpose()*embed<4>(model->normal(iface, nthvert), 0.f)));
        varying_nrm.setCol(nthvert,model->norm_tri[iface][nthvert]);
        varying_tri.setCol(nthvert, gl_Vertex);
        varying_uv.setCol(nthvert,model->text_tri[iface][nthvert]);
        //varying_nrm.setCol(nthvert,)
        textures=model->textures;

        return gl_Vertex; // transform it to screen coordinates
    }


    virtual bool fragment(vect<3,float> bar, TGAColor &color){
    	//float intensity=varying_intensity*bar;
    	//color=TGAColor(255, 255, 255)*intensity;
    	vect<3,float> abc[3];
    	for(int j=0; j<3;j++){
    	for (int i=0; i<3; i++){
    	abc[j][i]=varying_tri[i][j]/varying_tri[3][j];
    	}
    	}

    	vect<3,float> ab = abc[1]-abc[0];
    	vect<3,float> ac = abc[2]-abc[0];
    	vect<3,float> pa= abc[0]-bar;
    	vect<3,float> nn=vect<3,float>(float(ab[0]),float(ac[0]),float(pa[0]));
    	vect<3,float> mm=vect<3,float>(float(ab[1]),float(ac[1]),float(pa[1]));
    	vect<3,float> res = vect_mult(nn,mm);
    	//printf("res  %f %f %f\n",res[0],res[1],res[2]);
    	vect<3,float> temp_clip = vect<3,float>(1.f-float(res[0]+res[1])/res[2],float(res[0])/res[2],float(res[1])/res[2]);
    	for (int i =0; i<3; i++) {temp_clip=temp_clip/varying_tri[3][i];}
    	temp_clip=temp_clip/(temp_clip[0]+temp_clip[1]+temp_clip[2]);
    	if (std::fabs(res[2])<1.f) return true;
    	float inten_cur=(temp_clip*vect<3,float>(varying_intensity[0],varying_intensity[1],varying_intensity[2]));
    	inten_cur = std::max(std::min(inten_cur,1.0f), 0.0f);

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



	for (int i =0; i<3; i++){

	strcpy(name_file,names[i].c_str());
	strcpy(name_file_diff,diffs[i].c_str());
	printf("file %s %s\n",name_file,name_file_diff);



	Model mdl;


	light_vec=light_vec.normalize();



    ViewPortMtx=viewport(0, 0, width, height, zeight);
    PerspMtx=perspective((eye-center).norm());
    view = lookat(eye, center,up);


	parser(name_file_diff,name_file,mdl);
	ShaderGuro shader;



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


