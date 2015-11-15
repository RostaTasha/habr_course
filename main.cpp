
#include "libs.hpp"
#include "parser.hpp"
#include "matrix.h"
#include "geometry.hpp"
#include "opengl_.hpp"
#include "limits.h"


	mtrx2d<float> z_buffer(width,height,std::numeric_limits<float>::min());
	mtrx2d<float> z_shdr(width,height,std::numeric_limits<float>::min());



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
matrix<4,4,float> shdw;
matrix<4,4,float> shdw_adj;
matrix<4,4,float> fin_mtrx;
matrix<4,4,float> fin_mtrx_adj;
matrix<4,4,float> uniform_Mshadow;


vect<3,float> eye(1.,1.,4.);
vect<3,float> center(0.,0.,0.);
vect<3,float> up(0.,1.,0.);
vect<3,float> light_vec(1.,1.,0.);
vect<3,float> light_vec_shdw;

struct coeffs{
coeffs(int a=0, float b=0, float c=0.f) : ambient(a),b1(b),b2(c){}
int ambient;
float b1;
float b2;
};

coeffs coeff_cur;

struct ShaderGuro : public IShader  {


    virtual vect<4,float> vertex(int iface, int nthvert, Model * model){
    	varying_intensity[nthvert] = std::max(0.f, (model->norm_tri[iface][nthvert]*light_vec)); // get diffuse lighting intensity
        vect<4,float> gl_Vertex;
        gl_Vertex[3]=1;
        for (int i =0; i<3; i++) gl_Vertex[i]= model->coords_tri[iface][nthvert][i];//embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
        gl_Vertex=fin_mtrx*gl_Vertex;

        //varying_nrm.set_col(nthvert, proj<3>((Projection*ModelView).invert_transpose()*embed<4>(model->normal(iface, nthvert), 0.f)));

        vect<4,float> gl_norm;
        gl_norm[0]=(model->norm_tri[iface][nthvert])[0];
        gl_norm[1]=(model->norm_tri[iface][nthvert])[1];
        gl_norm[2]=(model->norm_tri[iface][nthvert])[2];
        gl_norm=fin_mtrx_adj*gl_norm;
        varying_nrm.setCol(nthvert,vect<3,float>(gl_norm[0],gl_norm[1],gl_norm[2]));

        varying_tri.setCol(nthvert, gl_Vertex);
        varying_uv.setCol(nthvert,model->text_tri[iface][nthvert]);

        textures=&(model->textures);
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

		int x = round((*textures).get_width()*te_cur[0]);
		int y = round((*textures).get_height()*(1.-te_cur[1]));
		color = ((*textures).get(x,y))*inten_cur;

    	return false;
    }



};




struct ShaderGuro2 : public IShader  {
	vect<4,float> gl_Vertex;
	vect<4,float> gl_norm;


    virtual vect<4,float> vertex(int iface, int nthvert, Model * model){

        gl_Vertex[3]=1;
        for (int i =0; i<3; i++) gl_Vertex[i]= model->coords_tri[iface][nthvert][i];//embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
        gl_Vertex=fin_mtrx*gl_Vertex;


        gl_norm[0]=(model->norm_tri[iface][nthvert])[0];
        gl_norm[1]=(model->norm_tri[iface][nthvert])[1];
        gl_norm[2]=(model->norm_tri[iface][nthvert])[2];
        gl_norm=fin_mtrx_adj*gl_norm;
        varying_nrm.setCol(nthvert,vect<3,float>(gl_norm[0],gl_norm[1],gl_norm[2]));

        varying_tri.setCol(nthvert, gl_Vertex/gl_Vertex[3]);
        varying_uv.setCol(nthvert,model->text_tri[iface][nthvert]);

        textures=&(model->textures);
        norms=&(model->norms);
        gloss=&(model->gls);
        return gl_Vertex; // transform it to screen coordinates
    }






     ~ShaderGuro2(){
    }


    virtual bool fragment(vect<3,float> bar, TGAColor &color){

    	vect<3,float> temp_clip;

		for (int i=0; i<3; i++)temp_clip[i]=bar[i]/varying_tri[3][i];

		temp_clip=temp_clip/(temp_clip[0]+temp_clip[1]+temp_clip[2]);

		//float inten_cur=(temp_clip*vect<3,float>(varying_intensity[0],varying_intensity[1],varying_intensity[2]));

    	vect<2,float> te_cur;
    	te_cur = varying_uv*bar;

		int x = round((*textures).get_width()*te_cur[0]);
		int y = round((*textures).get_height()*te_cur[1]);

		int x1 = round((*norms).get_width()*te_cur[0]);
		int y1 = round((*norms).get_height()*te_cur[1]);

		int x2 = round((*gloss).get_width()*te_cur[0]);
		int y2 = round((*gloss).get_height()*te_cur[1]);



		TGAColor inten_cur_color = (*norms).get(x1,y1);

		//vect<3,float> norm((float)(inten_cur_color.r),(float)(inten_cur_color.g),(float)(inten_cur_color.b));

		vect<3,float> norm;
	    for (int i=0; i<3; i++)
	        gl_norm[2-i] = (float)inten_cur_color[i]/255.f*2.f - 1.f;
		gl_norm = fin_mtrx_adj*gl_norm;
		for(int i =0; i<3; i++) norm[i]=gl_norm[i];

		norm=norm.normalize();

		vect<3,float> l =light_vec;
		vect<3,float> r;
		r=((2.f*norm*l)*norm-l).normalize();
		//r=r.normalize();



		unsigned int power_n =  (*gloss).get(x2,y2)[0];

		float specular = pow(max<float>(0.f,r[2]),power_n);


		float ambient = coeff_cur.ambient;


		float diffuse=max<float>(0.f,norm*l);




		vect<4,float> ttemp=varying_tri*bar;
		ttemp=uniform_Mshadow*ttemp;
		ttemp=ttemp/ttemp[3];

		//printf("%f %f %f %f\n",ttemp[0],ttemp[1],ttemp[2],ttemp[3]);


		bool flag_shdw=0;
		if ((10.32+ttemp[2])>z_shdr((int)(ttemp[0]),(int)(ttemp[1])) ) flag_shdw=1; // // magic coeff to avoid z-fighting
		float shadow = .3+.7*flag_shdw;



		TGAColor c = ((*textures).get(x,y));
		//printf("spec %f\n",specular);

		for (int i =0; i<3; i++){
			color[i] = min<float>(ambient + c[i]*shadow*(coeff_cur.b1*diffuse +coeff_cur.b2*specular),255);
		}


    	return false;
    }

};



struct Shaderer : public IShader{
vect<4,float> gl_Vertex;
vect<4,float> gl_norm;

~Shaderer(){
}

virtual vect<4,float> vertex(int iface, int nthvert, Model * model){

gl_Vertex[3]=1;
for (int i =0; i<3; i++) gl_Vertex[i]= model->coords_tri[iface][nthvert][i];//embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
 gl_Vertex=shdw*gl_Vertex;


gl_norm[0]=(model->norm_tri[iface][nthvert])[0];
gl_norm[1]=(model->norm_tri[iface][nthvert])[1];
gl_norm[2]=(model->norm_tri[iface][nthvert])[2];
gl_norm=shdw_adj*gl_norm;

varying_nrm.setCol(nthvert,vect<3,float>(gl_norm[0],gl_norm[1],gl_norm[2]));
varying_uv.setCol(nthvert,model->text_tri[iface][nthvert]);
varying_tri.setCol(nthvert, gl_Vertex);
norms=&(model->norms);
	        return gl_Vertex; // transform it to screen coordinates
	    }





virtual bool fragment(vect<3,float> bar, TGAColor &color){

	vect<3,float> temp_clip;

	for (int i=0; i<3; i++)temp_clip[i]=bar[i]/varying_tri[3][i];

	temp_clip=temp_clip/(temp_clip[0]+temp_clip[1]+temp_clip[2]);

	float inten_cur=(temp_clip*vect<3,float>(varying_intensity[0],varying_intensity[1],varying_intensity[2]));


	//vect<2,float> te_cur;
	//for (int i=0; i<2; i++) te_cur[i]=temp_clip*vect<3,float>(varying_uv[i][0],varying_uv[i][1],varying_uv[i][2]);



	//int x1 = round((*norms).get_width()*te_cur[0]);
	//int y1 = round((*norms).get_height()*(1.-te_cur[1]));







	vect<3,float> norm(varying_nrm[0]*temp_clip,
						varying_nrm[1]*temp_clip,
						varying_nrm[2]*temp_clip);

	//TGAColor inten_cur_color = (*norms).get(x1,y1);
	//printf("color %d %d %d %d %d %d\n", inten_cur_color.r, inten_cur_color.g,inten_cur_color.b, x1,y1,norms.get_width());
	//vect<3,float> norm((float)(inten_cur_color.r),(float)(inten_cur_color.g),(float)(inten_cur_color.b));
	vect<4,float> gl_norm;
	for(int i =0; i<3; i++) gl_norm[i]=norm[i];
	gl_norm = shdw_adj*gl_norm;
	for(int i =0; i<3; i++) norm[i]=gl_norm[i];

	norm=norm.normalize();

	vect<4,float> bar_;
	for (int i =0; i<3; i++) bar_[i]=bar[i];

	vect<4,float> p = varying_tri*bar;

	float diffuse=max<float>(0.f,(p[2]/zeight/p[3]));




	for (int i =0; i<3; i++){
		color[i] = 255*diffuse;
	}


	return false;
}






};




int main(int argc, char** argv) {

	TGAImage image(width, height, TGAImage::RGB);
	TGAImage image_shdr(width, height, TGAImage::RGB);





	std::vector<string> names;
	std::vector<string> diffs;
	std::vector<string> nm;
	std::vector<string> gls;
	std::vector<coeffs> coeff;

	names.push_back("african_head.obj");
	//names.push_back("floor.obj");
	names.push_back("african_head_eye_inner.obj");
	//names.push_back("african_head_eye_outer.obj");
	//names.push_back("diablo3_pose.obj");


	diffs.push_back("african_head_diffuse.tga");
	//diffs.push_back("floor_diffuse.tga");
	diffs.push_back("african_head_eye_inner_diffuse.tga");
	//diffs.push_back("african_head_eye_outer_diffuse.tga");
	//diffs.push_back("diablo3_pose_diffuse.tga");


	nm.push_back("african_head_nm.tga");
	//nm.push_back("floor_nm_tangent.tga");
	nm.push_back("african_head_eye_inner_nm.tga");
	//nm.push_back("african_head_eye_outer_nm.tga");
	//nm.push_back("diablo3_pose_nm.tga");



	gls.push_back("african_head_spec.tga");
	//gls.push_back("floor_nm_tangent.tga");
	gls.push_back("african_head_eye_inner_spec.tga");
	//gls.push_back("african_head_eye_outer_spec.tga");
	//gls.push_back("diablo3_pose_spec.tga");


	coeff.push_back(coeffs(20,1.2f,0.6f));
	//coeff.push_back(coeffs(10,1.f,0.6f));
	coeff.push_back(coeffs(20,1.2f,0.6f));
	//coeff.push_back(coeffs(10,1.f,4.6f));
	//coeff.push_back(coeffs(20,1.2f,0.6f));

	char name_file[40];
	char name_file_diff[40];
	char name_file_norm[40];
	char name_file_spec[40];

    ViewPortMtx=viewport(width/8,height/8 , width*3/4, height*3/4, zeight);
    //width/8, height/8, width*3/4, height*3/4
    //PerspMtx=perspective((eye-center).norm());
    PerspMtx=perspective((eye-center).norm());
    view = lookat(eye, center,up);



    shdw=ViewPortMtx*lookat(light_vec,center,up);
    shdw_adj=shdw.Adjacent();

    vect<4,float> gl_light_shdw;
	gl_light_shdw[0]=light_vec[0];
	gl_light_shdw[1]=light_vec[1];
	gl_light_shdw[2]=light_vec[2];

	gl_light_shdw=shdw*gl_light_shdw;

	printf("gl_light_shdw %f %f %f\n",gl_light_shdw[0],gl_light_shdw[1],gl_light_shdw[2]);


    light_vec_shdw=vect<3,float>(gl_light_shdw[0],gl_light_shdw[1],gl_light_shdw[2]);

	light_vec_shdw=light_vec_shdw.normalize();

    fin_mtrx=ViewPortMtx*PerspMtx*view;
    fin_mtrx_adj=(ViewPortMtx*view).invert_transpose();


    uniform_Mshadow=shdw*fin_mtrx.invert();


	//light_dir = proj<3>((Projection*ModelView*embed<4>(light_dir, 0.f))).normalize();
	vect<4,float> gl_light;
	gl_light[0]=light_vec[0];
	gl_light[1]=light_vec[1];
	gl_light[2]=light_vec[2];

	gl_light=ViewPortMtx*PerspMtx*view*gl_light;
	light_vec=vect<3,float>(gl_light[0],gl_light[1],gl_light[2]);

	light_vec=light_vec.normalize();


	for (int i =0; i<names.size(); i++){


	coeff_cur=coeff[i];

	strcpy(name_file,names[i].c_str());
	strcpy(name_file_diff,diffs[i].c_str());
	strcpy(name_file_norm,nm[i].c_str());
	strcpy(name_file_spec,gls[i].c_str());
	printf("file %s %s %s %s\n",name_file,name_file_diff,name_file_norm,name_file_spec);



	Model mdl;





	parser(name_file_diff,name_file,name_file_norm,name_file_spec,mdl);
	Shaderer shader;
	ShaderGuro2 shaderer;


	//printf("OKI DOKI!\n");

	for(std::vector<int>::size_type i = 0; i != mdl.coords_tri.size(); i++) {

		 for (int j=0; j<3; j++) {
		     	shader.vertex(i, j,&mdl);
		        }

	    color_triangle(shader, image_shdr, z_shdr);

	}





	for(std::vector<int>::size_type i = 0; i != mdl.coords_tri.size(); i++) {

		 for (int j=0; j<3; j++) {
		     	shaderer.vertex(i, j,&mdl);
		        }

	    color_triangle(shaderer, image, z_buffer);

	}

	}


	image_shdr.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image_shdr.write_tga_file("output_shr.tga");

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;
}


