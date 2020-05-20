#include <stdlib.h>
#include <stdio.h>
#include "Util.h"

int main(int argc, char* argv[])
    {
    FILE* ifp;
    FILE* ifp1;
    gray* graymap, currentgraymap;
    int ich1, ich2, rows, cols, maxval=255;
    int i, j;
    int arraySize = 12; 
    double filter[9] = {0.0625,0.125,0.0625,0.125,0.25,0.125,0.0625,0.125,0.0625};

    /* Arguments */
    if ( argc != 2 ){
      printf("\nUsage: %s file \n\n", argv[0]);
      exit(0);
    }

    /* Opening */
    ifp = fopen(argv[1],"r");
    if (ifp == NULL) {
      printf("error in opening file %s\n", argv[1]);
      exit(1);
    }

    ifp1 = fopen("results/binomial1/boat_noise2_binomial.pgm","w");


    /*  Magic number reading */
    ich1 = getc( ifp );
    if ( ich1 == EOF )
        pm_erreur( "EOF / read error / magic number" );
    ich2 = getc( ifp );
    if ( ich2 == EOF )
        pm_erreur( "EOF /read error / magic number" );
    if(ich2 != '2' && ich2 != '5')
      pm_erreur(" wrong file type ");


    /* Reading image dimensions */
    cols = pm_getint( ifp );
    rows = pm_getint( ifp );
    maxval = pm_getint( ifp );

    
    // allocate extra two rows and two columns for padding the image with zeros
    int coll = cols + 2; 
    int roww = rows + 2; 
    graymap = (gray *) malloc(coll * roww * sizeof(gray));
    graymap[0] = 0;
    

    /* Reading */
    for(i=1; i < rows; i++){
      for(j=0; j < cols ; j++){
          // write values of image in array skipping the first row& column of zeros
          graymap[i* coll + j+1] = pm_getrawbyte(ifp);
        }
    }
    
    /* Writing */
    fputs("P2\n",ifp1);

    unsigned char buffer[16]={0};

    sprintf(buffer,"%d ", cols);
    fputs(buffer, ifp1);
    sprintf(buffer,"%d\n", rows);
    fputs(buffer, ifp1);
    sprintf(buffer,"%d\n", maxval);
    fputs(buffer, ifp1);

    int result;
      for(i=1; i < rows; i++){
        for(j=0; j < cols ; j++){
          // get the sorrounding values
          int current[9] = {graymap[(i-1) * coll + (j)], graymap[(i-1) * coll + (j+1)], graymap[(i-1) * coll + (j+2)], 
                          graymap[(i) * coll + (j)], graymap[(i) * coll + (j+1)], graymap[(i) * coll + (j+2)],
                          graymap[(i+1) * coll + (j)], graymap[(i+1) * coll + (j+1)], graymap[(i+1) * coll + (j+2)]};
          result = 0;
          for (int i = 0; i < 9; i++)
          {
            result = result + filter[i] * current[i]; // apply the filter for the current image value
          }
          sprintf(buffer,"%d ",  result); 
          fputs(buffer, ifp1); // write the new value to new file
        }
        fputs("\n", ifp1);
      }

    /* Closing */
    fclose(ifp);
    fclose(ifp1);
    return 0;
}


