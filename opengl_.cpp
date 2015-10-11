#include "opengl_.hpp"

int round(float a){
	return (int)(a+0.5);
}


void color_triangle(Shader shdr,TGAImage &image, float inten_0, float inten_1, float inten_2, mtrx2d<int> & z_buffer, vect<3,float> te0, vect<3,float> te1, vect<3,float> te2, TGAImage &text){

	Shader sh_temp;


	vect<3,int> points[3];



	for(int j=0; j<3;j++){
	for (int i=0; i<3; i++){
	points[j][i]=shdr.varying_tri[i][j]/shdr.varying_tri[3][j];
	}
	}






	int u_border=max(max(points[0][1],points[1][1]),points[2][1]);
	int b_border=min(min(points[0][1],points[1][1]),points[2][1]);

	int r_border=max(max(points[0][0],points[1][0]),points[2][0]);
	int l_border=min(min(points[0][0],points[1][0]),points[2][0]);





	vect<3,int> p;

	float inten_cur;
	vect<3,float> temp_clip;
	vect<3,float> temp;

	for (p[0]=l_border; p[0]<r_border; p[0]++){
		for (p[1]=b_border; p[1]<u_border; p[1]++){

				if (!((p[0]<width && p[1]< height && p[0]>=0 && p[1]>=0))) continue;

				temp=xy_to_bc(points,p);
				for (int i=0; i<3; i++)temp_clip[i]=temp[i]/shdr.varying_tri[3][i];

				temp_clip=temp_clip/(temp_clip[0]+temp_clip[1]+temp_clip[2]);

				p[2]=int(temp_clip*vect<3,float>(float(points[0][2]),float(points[1][2]),float(points[2][2])));
				if ((temp[0]<0 || temp[1]<0 || temp[2]<0 || z_buffer(p[0],p[1]) > p[2]) ) continue;

				vect<3,float> te_cur;
				for (int i=0; i<3; i++) te_cur[i]=temp_clip*vect<3,float>(te0[i],te1[i],te2[i]);


				int x = round(text.get_width()*te_cur[0]);
				int y = round(text.get_height()*(1.-te_cur[1]));
				TGAColor color1=text.get(x,y);


				inten_cur=(temp_clip*vect<3,float>(inten_0,inten_1,inten_2));

				int color_code = (int)(((inten_cur))*255);
				color_code = std::max(std::min(color_code, 255), 0);

				inten_cur = std::max(std::min(inten_cur,1.0f), 0.0f);
				TGAColor color2(round(((int)color1.r)*inten_cur),round(((int)color1.g)*inten_cur),round(((int)color1.b)*inten_cur),255);

				image.set(p[0], p[1], color2);
				z_buffer(p[0],p[1])=p[2];


		}
	}


return;
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
