/*
 * parser.cpp
 *
 *  Created on: 06 θώνÿ 2015 γ.
 *      Author: Tata
 */

#include "parser.hpp"

//void parser(char * name_diff,char *  name, std::vector<vect<3,float>>  & coords , std::vector<vect<3,int>>  & triangles , std::vector<vect<3,float> > & norm, std::vector<vect<3,int> >  & norm_triangles, std::vector<vect<3,float> > & text, std::vector<vect<3,int> >  & text_triangles, TGAImage & textures){

void parser(char * name_diff,char *  name, Model &  mdl){
	  std::ifstream myfile;
	  unsigned int num=0;
	  float tempx, tempy,tempz;
	  int tempa, tempb,tempc, tempa1, tempb1, tempc1, tempa2, tempb2, tempc2;
	  mdl.textures.read_tga_file(name_diff);
	  myfile.open (name);
	  std::string line;

	  std::vector<vect<3,float> > coords;
	  std::vector<vect<2,float> > text;
	  std::vector<vect<3,float> > norm;
	  //std::vector<vect<3,int> > triangles;
	  //std::vector<vect<3,int> > norm_triangles;
	  //std::vector<vect<3,int> > text_triangles;
	  while (std::getline(myfile, line))
	  {

	  if (line[0]=='v' && line[1]==' '){
		  line=line.substr(2);
		  std::istringstream  iss (line);
		  iss>>tempx>>tempy>>tempz;
		  coords.push_back(vect<3,float>(tempx,tempy,tempz));
	  }


	  if (line[0]=='v' && line[1]=='n'){
		  line=line.substr(2);
		  std::istringstream  iss (line);
		  iss>>tempx>>tempy>>tempz;
		  norm.push_back(vect<3,float>(tempx,tempy,tempz));
	  }

	  if (line[0]=='v' && line[1]=='t'){
		  line=line.substr(2);
		  std::istringstream  iss (line);
		  iss>>tempx>>tempy;
		  text.push_back( vect<2,float>(tempx,tempy) );
	  }



	  if (line[0]=='f' && line[1]==' '){
		  line=line.substr(2);
		  std::sscanf (line.c_str(),"%d/%d/%d %d/%d/%d %d/%d/%d",&tempa,&tempa2,&tempa1,&tempb,&tempb2, &tempb1,&tempc,&tempc2,&tempc1);
		  //triangles.push_back(vect<3,int>(tempa, tempb, tempc));
		  //norm_triangles.push_back(vect<3,int>(tempa1, tempb1, tempc1));
		  //text_triangles.push_back(vect<3,int>(tempa2, tempb2, tempc2));


		  mdl.coords_tri.push_back(
				  vect<3,vect<3,float> >(
						  coords[tempa-1],
						  coords[tempb-1],
						  coords[tempc-1])
						  );

		  mdl.norm_tri.push_back(
				  vect<3,vect<3,float> >(
						  norm[tempa1-1],
						  norm[tempb1-1],
						  norm[tempc1-1])
						  );

		  mdl.text_tri.push_back(
				  vect<3,vect<2,float> >(
						  text[tempa2-1],
						  text[tempb2-1],
						  text[tempc2-1])
						  );

	  }

	  }



	/*  for(std::vector<int>::size_type i = 0; i != triangles.size(); i++) {
		  mdl.coords_tri.push_back(
				  vect<3,vect<3,float> >(
						  coords[triangles[i][0]-1],
						  coords[triangles[i][1]-1],
						  coords[triangles[i][2]-1])
						  );
	  }

		  for(std::vector<int>::size_type i = 0; i != norm_triangles.size(); i++) {
			  mdl.norm_tri.push_back(
					  vect<3,vect<3,float> >(
							  norm[norm_triangles[i][0]-1],
							  norm[norm_triangles[i][1]-1],
							  norm[norm_triangles[i][2]-1])
							  );

		  }

			  for(std::vector<int>::size_type i = 0; i != text_triangles.size(); i++) {
				  mdl.text_tri.push_back(
						  vect<3,vect<2,float> >(
								  text[text_triangles[i][0]-1],
								  text[text_triangles[i][1]-1],
								  text[text_triangles[i][2]-1])
								  );

			  	  }
*/


return;
}
