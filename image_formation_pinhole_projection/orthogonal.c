#include "imageFormationUtils.h"
#include <stdio.h>
#include <stdlib.h>
// struct point3d{
//    float  x; // If you want to access x, you need to do p.x, suppose you decleared p as "struct point3d p;"
//    float  y; // Similar to above
//    float  z;
//    int r;
//    int g;
//    int b;
// };


typedef struct point2d
{
	float x;
	float y;
	int r;
    int g;
    int b;
}point2d_t;

typedef struct point2d_int
{
	int x;
	int y;
	int r;
    int g;
    int b;
}point2d_int_t;


typedef struct colors
{
	int r;
    int g;
    int b;
}colors_t;

int main(int argc, char* argv[])
{
	struct point3d *points;
	int N_v = 0;
	points = readOff(argv[1], &N_v);

	FILE* ifp1;
    FILE* ifp2;
	ifp1 = fopen("values.txt","w");
    ifp2 = fopen("./orth/orthogonal.ppm","w");
	
	float u_0 = 500; 
	float v_0 = 500;

	float alpha_u = 0.001;
	float alpha_v = 0.001;
		
	int cols = 1000;
	int rows = 1000;

    const char buffer1[16]={0};
    int maxval = 255;

    // ------------------U V Projection Without rigid---------------
    point2d_int_t uv_projection[N_v];  // storing the u and v: pixel coordinates

    for (int i = 0; i < N_v; i++)
    {
        uv_projection[i].x = points[i].x/alpha_u + u_0;
        uv_projection[i].y = points[i].y/alpha_v + v_0;        
        uv_projection[i].r = points[i].r;
        uv_projection[i].g = points[i].g;
        uv_projection[i].b = points[i].b;
    }

    // ------------------END U V Projection Without rigid---------------		

    colors_t* gradient;
    gradient = (colors_t *) malloc(rows * cols * sizeof(colors_t));

    fputs("P3\n",ifp2);
    sprintf(buffer1,"%d ", cols);
    fputs(buffer1, ifp2);
    sprintf(buffer1,"%d\n", rows);
    fputs(buffer1, ifp2);
    sprintf(buffer1,"%d\n", maxval);
    fputs(buffer1, ifp2);

    for(int k = 0; k < N_v; k++){
        if(uv_projection[k].x > 0 && uv_projection[k].x < rows && uv_projection[k].y > 0 && uv_projection[k].y < cols){
            colors_t color;
            color.r = uv_projection[k].r;
            color.g = uv_projection[k].g;
            color.b = uv_projection[k].b;
            gradient[uv_projection[k].x * cols + uv_projection[k].y] = color;
        }
    }
    /* Write to file*/ 
    for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++)
		{
			sprintf(buffer1,"%d ", gradient[i * cols + j].r);
			fputs(buffer1, ifp2);
			sprintf(buffer1,"%d ", gradient[i * cols + j].g);
			fputs(buffer1, ifp2);
			sprintf(buffer1,"%d ", gradient[i * cols + j].b);
			fputs(buffer1, ifp2);

		}
		sprintf(buffer1,"%s", "\n");
  		fputs(buffer1, ifp2);
    }
	
	fclose(ifp1);
	fclose(ifp2);
}
