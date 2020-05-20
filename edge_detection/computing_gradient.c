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


    ifp1 = fopen("filterx.pgm","w");
    ifp2 = fopen("filtery.pgm","w");
    ifp3 = fopen("gradient.pgm","w");


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
    xgraymap = (gray *) malloc(coll * roww * sizeof(gray));
    ygraymap = (gray *) malloc(coll * roww * sizeof(gray));
    gradient = (gray *) malloc(coll * roww * sizeof(gray));
    
    
    xgraymap[0] = 0;
    ygraymap[0] = 0;
    gradient[0] = 0;

    /* Reading */
    for(i=1; i < rows; i++)
      for(j=0; j < cols ; j++){
          graymap[i * coll + j+1] = pm_getrawbyte(ifp) ;
      }


    /* Writing */
    fputs("P2\n",ifp1);
    fputs("P2\n",ifp2);
    fputs("P2\n",ifp3);
    unsigned char buffer[16]={0};

        
    sprintf(buffer,"%d ", cols);
    fputs(buffer, ifp1);
    fputs(buffer, ifp2);
    fputs(buffer, ifp3);
    sprintf(buffer,"%d\n", rows);
    fputs(buffer, ifp1);
    fputs(buffer, ifp2);
    fputs(buffer, ifp3);
    sprintf(buffer,"%d\n", maxval);
    fputs(buffer, ifp1);
    fputs(buffer, ifp2);
    fputs(buffer, ifp3);


    // Apply filter
    int resultx;
    int resulty;
    for(i=1; i < rows; i++)
      for(j=0; j < cols ; j++){
          int current[9] = {graymap[(i-1) * coll + (j)], graymap[(i-1) * coll + (j+1)], graymap[(i-1) * coll + (j+2)], 
                          graymap[(i) * coll + (j)], graymap[(i) * coll + (j+1)], graymap[(i) * coll + (j+2)],
                          graymap[(i+1) * coll + (j)], graymap[(i+1) * coll + (j+1)], graymap[(i+1) * coll + (j+2)]};
          
          resultx = 0;
          resulty = 0;
          for (int i = 0; i < 9; i++)
          {
            resultx = resultx + filterx[i] * current[i]; // apply the filter for the current image value
            resulty = resulty + filtery[i] * current[i]; // apply the filter for the current image value
          }
        xgraymap[i * coll + j+1] = abs(resultx);
        ygraymap[i * coll + j+1] = abs(resulty);

        gradient[i * coll + j+1] = sqrt(resultx*resultx + resulty*resulty);
      }


    /* Write to file */ 
    for (int i = 1; i < rows; i++){
      for (int j = 0; j < cols; j++)
      {
        sprintf(buffer,"%d ", xgraymap[i * coll + j+1]);
        fputs(buffer, ifp1);
        sprintf(buffer,"%d ", ygraymap[i * coll + j+1]);
        fputs(buffer, ifp2);
        sprintf(buffer,"%d ", gradient[i * coll + j+1]);
        fputs(buffer, ifp3);
      }
    }


    /* Closing */
    fclose(ifp);
    fclose(ifp1);
    fclose(ifp2);
    fclose(ifp3);
    return 0;
}
