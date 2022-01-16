#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "image.h"

int writeImage(char fname[], ImageType& image)
{
 int i, j;
 int N, M, Q;
 unsigned char *charImage;
 std::ofstream ofp;

 image.getImageInfo(N, M, Q);

 charImage = (unsigned char *) new unsigned char [M*N];

 // convert the integer values to unsigned char

 int val;

 for(i=0; i<N; i++)
   for(j=0; j<M; j++) {
     image.getPixelVal(i, j, val);
     charImage[i*M+j]=(unsigned char)val;
   }

 ofp.open(fname, std::ios::out | std::ios::binary);

 if (!ofp) {
   std::cout << "Can't open file: " << fname << std::endl;
   exit(1);
 }

 ofp << "P5" << std::endl;
 ofp << M << " " << N << std::endl;
 ofp << Q << std::endl;

 ofp.write( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));

 if (ofp.fail()) {
   std::cout << "Can't write image " << fname << std::endl;
   exit(0);
 }

 ofp.close();

 delete [] charImage;

 return(1);

}