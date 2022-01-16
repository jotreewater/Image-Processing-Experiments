#include <iostream>
#include <fstream>
#include <cstdlib>

#include "image.h"

int readImageHeader(const char *fname, int& N, int& M, int& Q, bool& type)
{
 int i, j;
 unsigned char *charImage;
 char header [100], *ptr;
 std::ifstream ifp;

 ifp.open(fname, std::ios::in | std::ios::binary);

 if (!ifp) {
     std::cout << "Can't read image: " << fname << std::endl;
   exit(1);
 }

 // read header

 type = false; // PGM

 ifp.getline(header,100,'\n');
 if ( (header[0] == 80) &&  /* 'P' */
      (header[1]== 53) ) {  /* '5' */
      type = false;
 }
 else if ( (header[0] == 80) &&  /* 'P' */
      (header[1] == 54) ) {        /* '6' */
      type = true;
 } 
 else {
     std::cout << "Image " << fname << " is not PGM or PPM" << std::endl;
   exit(1);
 }

ifp.getline(header,100,'\n');
 while(header[0]=='#')
   ifp.getline(header,100,'\n');

 M=strtol(header,&ptr,0);
 N=atoi(ptr);

 ifp.getline(header,100,'\n');

 Q=strtol(header,&ptr,0);

 ifp.close();

 return(1);
}
