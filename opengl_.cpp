#include "opengl_.hpp"

int round(float a){
	return (int)(a+0.5);
}


void color_triangle(IShader & shdr, TGAImage &image,  mtrx2d<float> & z_buffer){

	//    matrix<2,3,float> varying_uv;  // triangle uv coordinates, written by the vertex shader, read by the fragment shader
    //matrix<4,3,float> varying_tri; // triangle coordinates (clip coordinates), written by VS, read by FS
    //matrix<3,3,float> varying_nrm; // normal per vertex to be interpolated by FS


	vect<3,float> points[3];



	for(int j=0; j<3;j++){
	for (int i=0; i<3; i++){
	points[j][i]=shdr.varying_tri[i][j];
	}
	}






	int u_border=max(max(points[0][1]/shdr.varying_tri[3][0],points[1][1]/shdr.varying_tri[3][1]),points[2][1]/shdr.varying_tri[3][2]);
	int b_border=min(min(points[0][1]/shdr.varying_tri[3][0],points[1][1]/shdr.varying_tri[3][1]),points[2][1]/shdr.varying_tri[3][2]);

	int r_border=max(max(points[0][0]/shdr.varying_tri[3][0],points[1][0]/shdr.varying_tri[3][1]),points[2][0]/shdr.varying_tri[3][2]);
	int l_border=min(min(points[0][0]/shdr.varying_tri[3][0],points[1][0]/shdr.varying_tri[3][1]),points[2][0]/shdr.varying_tri[3][2]);





	vect<3,int> p;

	float inten_cur;
	vect<3,float> temp_clip;
	vect<3,float> temp;
	TGAColor color2;

	for (p[0]=l_border; p[0]<=r_border; p[0]++){


		if (!(p[0]<width  && p[0]>=0 )) continue;
		for (p[1]=b_border; p[1]<=u_border; p[1]++){

				if (!(p[1]< height &&  p[1]>=0)) continue;

				//proj<2>(pts[0]/pts[0][3]), proj<2>(pts[1]/pts[1][3]), proj<2>(pts[2]/pts[2][3])

				vect<3,float >points_xy[3];
				points_xy[0]=points[0]/shdr.varying_tri[3][0];
				points_xy[1]=points[1]/shdr.varying_tri[3][1];
				points_xy[2]=points[2]/shdr.varying_tri[3][2];

				temp=xy_to_bc(points_xy,p);
				//for (int i=0; i<3; i++)temp_clip[i]=temp[i]/shdr.varying_tri[3][i];

				//temp_clip=temp_clip/(temp_clip[0]+temp_clip[1]+temp_clip[2]);
		        //            float z = pts[0][2]*c.x + pts[1][2]*c.y + pts[2][2]*c.z;
	            //				float w = pts[0][3]*c.x + pts[1][3]*c.y + pts[2][3]*c.z;

				float z = shdr.varying_tri[2][0]*temp[0] + shdr.varying_tri[2][1]*temp[1] + shdr.varying_tri[2][2]*temp[2];
		         float w = shdr.varying_tri[3][0]*temp[0] + shdr.varying_tri[3][1]*temp[1] + shdr.varying_tri[3][2]*temp[2];

				float frag_depth = (float)z/w;


				//p[2]=int(temp_clip*vect<3,float>(float(points[0][2]),float(points[1][2]),float(points[2][2])));

				if ((temp[0]<0 || temp[1]<0 || temp[2]<0 || z_buffer(p[0],p[1]) > frag_depth) ) continue;

				shdr.fragment(temp,color2);
				image.set(p[0], p[1], color2);
				z_buffer(p[0],p[1])=frag_depth;


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
	vect<3,float> x_=vect_mult(up,z_).normalize();
	vect<3,float> y_=vect_mult(z_,x_).normalize();
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



vect<3,float> xy_to_bc(vect<3,float> * abc, vect<3,int> p ){
	vect<3,float> ab = (abc[1]-abc[0]);
	vect<3,float> ac = (abc[2]-abc[0]);
	vect<3,float> p1((float)p[0],(float )p[1],(float) p[2]);
	vect<3,float> pa= abc[0]-p1;
	vect<3,float> nn=vect<3,float>((ab[0]),(ac[0]),(pa[0]));
	vect<3,float> mm=vect<3,float>((ab[1]),(ac[1]),(pa[1]));
	vect<3,float> res = vect_mult(nn,mm);
	//printf("res  %f %f %f\n",res[0],res[1],res[2]);
	if (std::fabs(res[2])<1.f) return vect<3,float>(-1.f,1.f,1.f); //// triangle is degenerate, in this case return smth with negative coordinates
	return vect<3,float>(1.f-float(res[0]+res[1])/res[2],float(res[0])/res[2],float(res[1])/res[2]);
}
