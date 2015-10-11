/*
 * parser.cpp
 *
 *  Created on: 06 θώνÿ 2015 γ.
 *      Author: Tata
 */

#include "parser.hpp"

//void parser(char * name_diff,char *  name, std::vector<vect<3,float>>  & coords , std::vector<vect<3,int>>  & triangles , std::vector<vect<3,float> > & norm, std::vector<vect<3,int> >  & norm_triangles, std::vector<vect<3,float> > & text, std::vector<vect<3,int> >  & text_triangles, TGAImage & textures){

void parser(char * name_diff,char *  name,std::vector<vect<3,float> >  & coords, std::vector<vect<3,int> >  & triangles, std::vector<vect<3,float> > & norm, std::vector<vect<3,int> >  & norm_triangles, std::vector<vect<3,float> > & text, std::vector<vect<3,int> >  & text_triangles, TGAImage & textures){
	  std::ifstream myfile;
	  unsigned int num=0;
	  float tempx, tempy,tempz;
	  int tempa, tempb,tempc, tempa1, tempb1, tempc1, tempa2, tempb2, tempc2;
	  textures.read_tga_file(name_diff);
	  myfile.open (name);
	  std::string line;
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
		  iss>>tempx>>tempy>>tempz;
		  text.push_back(vect<3,float>(tempx,tempy,tempz));
	  }



	  if (line[0]=='f' && line[1]==' '){
		  line=line.substr(2);
		  std::sscanf (line.c_str(),"%d/%d/%d %d/%d/%d %d/%d/%d",&tempa,&tempa2,&tempa1,&tempb,&tempb2, &tempb1,&tempc,&tempc2,&tempc1);
		  triangles.push_back(vect<3,int>(tempa, tempb, tempc));
		  norm_triangles.push_back(vect<3,int>(tempa1, tempb1, tempc1));
		  text_triangles.push_back(vect<3,int>(tempa2, tempb2, tempc2));
	  }

	  }
return;
}
