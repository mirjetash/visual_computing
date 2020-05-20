#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Util.h"


int main(int argc, char* argv[])
    {
    FILE* ifp;
    FILE* ifp1;
    FILE* ifp2;
    FILE* ifp3;
    gray* graymap;
    gray* xgraymap;
    gray* ygraymap;
    gray* gradient;
    int ich1, ich2, rows, cols, maxval=255, pgmraw;
    int i, j;

    double filterx[9] = {-0.1875, 0, 0.1875, 
                        -0.625, 0, 0.625,
                        -0.1875, 0, 0.1875,};

    double filtery[9] = {-0.1875, -0.625, -0.1875, 
                        0, 0, 0,
                        0.1875, 0.625, 0.1875,};

    /* Arguments */
    if ( argc != 4 ){
      printf("\nUsage: %s file \n\n low_threshold  high_threshold", argv[0]);
      exit(0);
    }

    /* Opening */
    ifp = fopen(argv[1],"r");
    if (ifp == NULL) {
      printf("error in opening file %s\n", argv[1]);
      exit(1);
    }


    ifp1 = fopen("./hysteresis_1.pgm","w");

    int u_threshold = atoi(argv[2]);
    int l_threshold = atoi(argv[3]);

    /*  Magic number reading */
    ich1 = getc( ifp );
    if ( ich1 == EOF )
        pm_erreur( "EOF / read error / magic number" );
    ich2 = getc( ifp );
    if ( ich2 == EOF )
        pm_erreur( "EOF /read error / magic number" );
    if(ich2 != '2' && ich2 != '5')
      pm_erreur(" wrong file type ");
    else
      if(ich2 == '2')
        pgmraw = 0;
      else pgmraw = 1;

    /* Reading image dimensions */
    cols = pm_getint( ifp );
    rows = pm_getint( ifp );
    maxval = pm_getint( ifp );

    /* Memory allocation  */
    int coll = cols + 2; 
    int roww = rows + 2; 
    graymap = (gray *) malloc(coll * roww * sizeof(gray));
    graymap[0] = 0;
    gradient = (gray *) malloc(coll * roww * sizeof(gray));
    
  
    gradient[0] = 0;

    /* Reading */
    for(i=1; i < rows; i++)
      for(j=0; j < cols ; j++){
          graymap[i * coll + j+1] = pm_getint(ifp) ;
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


    // Apply filter
    int resultx;
    int resulty;


    for(i=1; i < rows; i++)
      for(j=0; j < cols ; j++){
          int current[9] = {graymap[(i-1) * coll + (j)], graymap[(i-1) * coll + (j+1)], graymap[(i-1) * coll + (j+2)], 
                          graymap[(i) * coll + (j)], graymap[(i) * coll + (j+1)], graymap[(i) * coll + (j+2)],
                          graymap[(i+1) * coll + (j)], graymap[(i+1) * coll + (j+1)], graymap[(i+1) * coll + (j+2)]};
                  
        if(graymap[(i) * coll + (j+1)] > u_threshold) // check if pixel is above the higher threshold and keep it if it is
          gradient[i * coll + j+1] = graymap[(i) * coll + (j+1)];
        else if (graymap[(i) * coll + (j+1)] < l_threshold) // check if pixel is below the lower threshold and set it to 0 if it is
            gradient[i * coll + j+1] = 0;
        else{
            for (int i = 0; i < 9; i++){ // check if the pixel is connected to any strong pixel(above the higher threshold)
                if(current[i] >= u_threshold){
                    gradient[i * coll + j+1] = graymap[(i) * coll + (j+1)];
                    break;
                }else
                    gradient[i * coll + j+1] = 0;
            } 
          }
      }


    /* Write to file */ 
    for (int i = 1; i < rows; i++){
      for (int j = 0; j < cols; j++)
      {
        sprintf(buffer,"%d ", gradient[i * coll + j+1]);
        fputs(buffer, ifp1);
      }
    }


    /* Closing */
    fclose(ifp);
    fclose(ifp1); 
    return 0;
}
