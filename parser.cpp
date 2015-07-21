/*
 * parser.cpp
 *
 *  Created on: 06 θώνÿ 2015 γ.
 *      Author: Tata
 */

#include "parser.hpp"

void parser(std::vector<float> & coords, std::vector<int>  & triangles){
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
		  //std::cout<<line<<std::endl;
		  std::istringstream  iss (line);
		  iss>>tempx>>tempy>>tempz;
		  //std::cout<<tempx<<" "<<tempy<<" "<<tempz<<std::endl;
		  coords.push_back(tempx);
		  coords.push_back(tempy);
		  coords.push_back(tempz);
	  }




	  if (line[0]=='f' && line[1]==' '){
		  line=line.substr(2);
		  //std::cout<<line<<std::endl;
		  std::sscanf (line.c_str(),"%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",&tempa,&tempb,&tempc);
		  //std::cout<<tempa<<" "<<tempb<<" "<<tempc<<std::endl;
		  //std::cout<<tempx<<" "<<tempy<<" "<<tempz<<std::endl;
		  //coords.push_back(tempy);
		  triangles.push_back(tempa);
		  triangles.push_back(tempb);
		  triangles.push_back(tempc);
	  }

	  }
return;
}
