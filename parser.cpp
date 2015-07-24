/*
 * parser.cpp
 *
 *  Created on: 06 θώνÿ 2015 γ.
 *      Author: Tata
 */

#include "parser.hpp"

void parser(std::vector<d3<float> >  & coords, vector<d3<int> >  & triangles){
	  std::ifstream myfile;
	  unsigned int num=0;
	  float tempx, tempy,tempz;
	  int tempa, tempb,tempc;
	  myfile.open ("african_head.obj");
	  std::string line;
	  while (std::getline(myfile, line))
	  {

	  if (line[0]=='v' && line[1]==' '){
		  line=line.substr(2);
		  std::istringstream  iss (line);
		  iss>>tempx>>tempy>>tempz;
		  coords.push_back(d3<float>(tempx,tempy,tempz));
	  }




	  if (line[0]=='f' && line[1]==' '){
		  line=line.substr(2);
		  //std::cout<<line<<std::endl;
		  std::sscanf (line.c_str(),"%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",&tempa,&tempb,&tempc);
		  triangles.push_back(d3<int>(tempa, tempb, tempc));
	  }

	  }
return;
}
