/*
 * parser.cpp
 *
 *  Created on: 06 θώνÿ 2015 γ.
 *      Author: Tata
 */

#include "parser.hpp"

void parser(std::vector<d3<float> >  & coords, std::vector<d3<int> >  & triangles, std::vector<d3<float> > & norm, std::vector<d3<int> >  & norm_triangles){
	  std::ifstream myfile;
	  unsigned int num=0;
	  float tempx, tempy,tempz;
	  int tempa, tempb,tempc, tempa1, tempb1, tempc1;
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


	  if (line[0]=='v' && line[1]=='n'){
		  line=line.substr(2);
		  std::istringstream  iss (line);
		  iss>>tempx>>tempy>>tempz;
		  norm.push_back(d3<float>(tempx,tempy,tempz));
	  }



	  if (line[0]=='f' && line[1]==' '){
		  line=line.substr(2);
		  //std::cout<<line<<std::endl;
		  std::sscanf (line.c_str(),"%d/%*d/%d %d/%*d/%d %d/%*d/%d",&tempa,&tempa1,&tempb,&tempb1,&tempc,&tempc1);
		  triangles.push_back(d3<int>(tempa, tempb, tempc));
		  norm_triangles.push_back(d3<int>(tempa1, tempb1, tempc1));
	  }

	  }
return;
}
