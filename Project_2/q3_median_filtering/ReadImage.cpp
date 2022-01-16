#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iosfwd>
#include "image.h"

int readImage(const char *fname, ImageType& image)
{
 int i, j;
 int N, M, Q;
 unsigned char *charImage;
 char header [100], *ptr;
 std::ifstream ifp;

 ifp.open(fname, std::ios::in | std::ios::binary);

 if (!ifp) {
   std::cout << "Can't read image: " << fname << std::endl;
   exit(1);
 }

 // read header

 ifp.getline(header,100,'\n');
 if ( (header[0]!=80) ||    /* 'P' */
      (header[1]!=53) ) {   /* '5' */
      std::cout << "Image " << fname << " is not PGM" << std::endl;
      exit(1);
 }

ifp.getline(header,100,'\n');
 while(header[0]=='#')
   ifp.getline(header,100,'\n');

 M=strtol(header,&ptr,0);
 N=atoi(ptr);

 ifp.getline(header,100,'\n');
 Q=strtol(header,&ptr,0);

 charImage = (unsigned char *) new unsigned char [M*N];

 ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));

 if (ifp.fail()) {
   std::cout << "Image " << fname << " has wrong size" << std::endl;
   exit(1);
 }

 ifp.close();

 //
 // Convert the unsigned characters to integers
 //

 int val;

 for(i=0; i<N; i++)
   for(j=0; j<M; j++) {
     val = (int)charImage[i*M+j];
     image.setPixelVal(i, j, val);     
   }

 delete [] charImage;

 return (1);

}
