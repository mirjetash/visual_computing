#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Util.h"


int main(int argc, char* argv[])
    {
    FILE* ifp;
    FILE* ifp1;
    FILE* ifp2;
    gray* graymap;
    gray* gradient_direction;
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


    ifp1 = fopen("gradient_min.pgm","w");



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
    gradient_direction = (gray *) malloc(coll * roww * sizeof(gray));
    
    
    gradient_direction[0] = 0;
    gradient[0] = 0;

    /* Reading */
    for(i=1; i < rows; i++)
      for(j=0; j < cols ; j++){
          graymap[i * coll + j+1] = pm_getrawbyte(ifp) ;
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
          
          resultx = 0;
          resulty = 0;
          for (int i = 0; i < 9; i++)
          {
            resultx = resultx + filterx[i] * current[i]; // apply the filter for the current image value
            resulty = resulty + filtery[i] * current[i]; // apply the filter for the current image value
          }

        int angle;
	angle = atan2(resulty,resultx)/3.14159*180.0;


        // Calculate the approximate angle  
        int approximate_angle;
        if((angle < 22.5) && (angle > -22.5) || (angle > 157.5) && (angle < -157.5))
          approximate_angle = 0;
        if((angle > 22.5) && (angle < 67.5) || (angle < -112.5) && (angle > -157.5))
          approximate_angle = 45;
        if((angle > 67.5) && (angle < 112.5) || (angle < -67.5) && (angle > -112.5))
          approximate_angle = 90;
        if((angle > 112.5) && (angle < 157.5) || (angle < -22.5) && (angle > -67.5))
          approximate_angle = 135;


        gradient_direction[i*coll + j+1] = approximate_angle;
        sprintf(buffer,"%d ", gradient_direction[i * coll + j+1]);
        //fputs(buffer, ifp2);

        gradient[i * coll + j+1] = sqrt(abs(resultx)*abs(resultx) + abs(resulty)*abs(resulty));


      }


    for (int i = 1; i < rows; i++){
      for (int j = 0; j < cols; j++){
        if(gradient_direction[i*coll + j+1 ]==0){
          if(!((gradient[i*coll + j+1] > gradient[(i) * coll + (j)]) && (gradient[i*coll + j+1] > gradient[(i) * coll + (j+2)])))
            gradient[i*coll + j+1] = 0;
        }

        if(gradient_direction[i*coll + j+1 ]==45){
          if(!((gradient[i*coll + j+1] > gradient[(i-1) * coll + (j+2)]) && (gradient[i*coll + j+1] > gradient[(i+1) * coll + (j)])))
            gradient[i*coll + j+1] = 0;
        }


        if(gradient_direction[i*coll + j+1 ]==90){
          if(!((gradient[i*coll + j+1] > gradient[(i-1) * coll + (j+1)]) && (gradient[i*coll + j+1] > gradient[(i+1) * coll + (j+1)])))
            gradient[i*coll + j+1] = 0;
        }

        if(gradient_direction[i*coll + j+1 ]==135){
          if(!((gradient[i*coll + j+1] > gradient[(i-1) * coll + (j)]) && (gradient[i*coll + j+1] > gradient[(i+1) * coll + (j+2)])))
            gradient[i*coll + j+1] = 0;
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
    //fclose(ifp2); 
    return 0;
}
