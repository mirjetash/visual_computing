#include <stdlib.h>
#include <stdio.h>
#include "Util.h"

void sorting(int *a, int size){
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if(a[j] > a[i]){
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
    }
  }
}


int main(int argc, char* argv[])
    {
    FILE* ifp;
    FILE* ifp1;
    gray* graymap;
    gray* currentgraymap;
    int ich1, ich2, rows, cols, maxval = 255;
    int i, j;
    int median;
    int filter_size;

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

    ifp1 = fopen("results/median/boat_noise2_median.pgm","w");


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

    /* Memory allocation  */
    int coll = cols + 2;
    int roww = rows + 2;
    graymap = (gray *) malloc(coll * roww * sizeof(gray));

    currentgraymap = (gray *) malloc(coll * roww * sizeof(gray));
    graymap[0] = 0;
    currentgraymap[0] = 0;
    
  
    /* Reading */
    for(i=1; i < rows; i++){
      for(j=0; j < cols ; j++){
          currentgraymap[i* coll + j+1] = pm_getrawbyte(ifp);;
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

    for(int x = 0; x < 1; x++){
      graymap = currentgraymap;
      for(i=1; i < rows; i++){
        for(j=0; j < cols ; j++){
          int current[9] = {graymap[(i-1) * coll + (j)], graymap[(i-1) * coll + (j+1)], graymap[(i-1) * coll + (j+2)], 
                          graymap[(i) * coll + (j)], graymap[(i) * coll + (j+1)], graymap[(i) * coll + (j+2)],
                          graymap[(i+1) * coll + (j)], graymap[(i+1) * coll + (j+1)], graymap[(i+1) * coll + (j+2)]};
          
          sorting(&current, 9); // sort the values of current frame
          median = current[4];
          currentgraymap[i* coll + j+1] = median; 
        }
    }
  } 

  /* Write to file */ 
  for (int i = 1; i < rows; i++){
    for (int j = 0; j < cols; j++)
    {
      sprintf(buffer,"%d ", currentgraymap[i*coll + j+1]);
      fputs(buffer, ifp1);
    }
  }

    /* Closing */
    fclose(ifp);
    fclose(ifp1);
    return 0;
}


