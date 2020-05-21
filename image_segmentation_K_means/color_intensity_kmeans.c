#include <stdlib.h>
#include <stdio.h>
#include "Util.h"
#include <math.h>

typedef struct cluster_center
{
  int red;
  int green;
  int blue;
}pixel_t;


int main(int argc, char* argv[])
    {
    FILE* ifp;
    FILE* ifp1;
    pixel_t* graymap;
    int ich1, ich2, rows, cols, maxval=255, pgmraw;
    int i, j;

    /* Arguments */
    if ( argc != 5 ){
      printf("\nUsage: %s file \n\n", argv[0]);
      exit(0);
    }

    /* Opening */
    ifp = fopen(argv[1],"r");
    if (ifp == NULL) {
      printf("error in opening file %s\n", argv[1]);
      exit(1);
    }

    int k;
    k = atoi(argv[2]);

    ifp1 = fopen(argv[3],"w");

    int no_iter = atoi(argv[4]);

    /*  Magic number reading */
    ich1 = getc( ifp );
    if ( ich1 == EOF )
        pm_erreur( "EOF / read error / magic number" );
    ich2 = getc( ifp );
    if ( ich2 == EOF )
        pm_erreur( "EOF /read error / magic number" );
    if(ich2 != '3' && ich2 != '6')
      pm_erreur(" wrong file type ");
    else
      if(ich2 == '3')
         pgmraw = 0;
      else pgmraw = 1;

    /* Reading image dimensions */
    cols = pm_getint( ifp );
    rows = pm_getint( ifp );
    maxval = pm_getint( ifp );

    /* Memory allocation  */
    graymap = (pixel_t *) malloc(cols * rows * sizeof(pixel_t));

    /* Reading */
    for(i=0; i < rows; i++)
      for(j=0; j < cols ; j++){
        graymap[i*cols +j].red = pm_getrawbyte(ifp);      
        graymap[i*cols +j].green = pm_getrawbyte(ifp);      
        graymap[i*cols +j].blue = pm_getrawbyte(ifp);      
      }
        
    /* K-means */
    // graymap set of datapoints
    // int k = 4; // number of clusters
    int labels[cols*rows]; // set of labels for each data point
    int iterations = 0;
    int min_dist;
    int distance;
    // Initialize k cluster centers
    pixel_t cluster_centers[k];    
    for (int i = 0; i < k; i++)
    {
      cluster_centers[i].red = rand() % 256; // random numer btw 0 and 255
      cluster_centers[i].green = rand() % 256;
      cluster_centers[i].blue = rand() % 256;
    }

    printf("Working...\n");

    while(iterations < no_iter){
      for(i=0; i < rows; i++) // for each data point
        for(j=0; j < cols ; j++){
          min_dist = 400;
          for (int t = 0; t < k; t++){ // for each cluster center
            distance = sqrt(pow(graymap[i*cols+j].red - cluster_centers[t].red, 2)+ pow(graymap[i*cols+j].green - cluster_centers[t].green, 2) +pow(graymap[i*cols+j].blue - cluster_centers[t].blue, 2));
            if (distance < min_dist){  // closest cluster from the pixel
              labels[i*cols + j] = t;
              min_dist = distance;
            }
          }
        }
      // find average of points(pixels) that have the same cluster, set that as the new cluster center
      for (int t = 0; t < k; t++){
        int pxl_same_label_red = 0;
        int pxl_same_label_green = 0;
        int pxl_same_label_blue = 0;
        int counter = 0;
        for (int i = 0; i < rows; i++){
          for (int j = 0; j < cols; j++){
            if (labels[i*cols+j] == t){
              counter++; // found one more pixel on the same cluster
              pxl_same_label_red = pxl_same_label_red + graymap[i*cols+j].red;
              pxl_same_label_green = pxl_same_label_green + graymap[i*cols+j].green;
              pxl_same_label_blue = pxl_same_label_blue + graymap[i*cols+j].blue;
            }
          }
        }
        if (counter!=0)
        {
          cluster_centers[t].red = pxl_same_label_red/counter;   
          cluster_centers[t].green = pxl_same_label_green/counter;   
          cluster_centers[t].blue = pxl_same_label_blue/counter;   
        }
      }
      iterations++;
    }

     
    /* Writing */
    fputs("P6\n",ifp1);    
    unsigned char buffer[16]={0};

    sprintf(buffer,"%d ", cols);
    fputs(buffer, ifp1);
    sprintf(buffer,"%d\n", rows);
    fputs(buffer, ifp1);
    sprintf(buffer,"%d\n", maxval);
    fputs(buffer, ifp1);

    for(i=0; i < rows; i++){
      for(j=0; j < cols ; j++){
        for (int t = 0; t < k; t++)
        {
          if (labels[i*cols+j]==t)
          {
            // sprintf(buffer,"%d ", cluster_centers[t]);
            // fputs(buffer, ifp1);
            fputc(cluster_centers[t].red, ifp1);
            fputc(cluster_centers[t].green, ifp1);
            fputc(cluster_centers[t].blue, ifp1);
          }
        }
      }
    }
      /* Closing */
      fclose(ifp);
      fclose(ifp1);
    printf("DONE!\n");
      return 0;
}
