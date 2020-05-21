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

	int f = atoi(argv[2]); // focal length
	struct point3d *points;
	int N_v = 0;
	points = readOff(argv[1], &N_v);

	FILE* ifp1;
    FILE* ifp2;
    FILE* ifp3;
	ifp1 = fopen("values.txt","w");
    ifp2 = fopen("./results/uv_image_res02.ppm","w");
    ifp3 = fopen("./results/uv_image_rigid.ppm","w");
	
	float u_0 = 200; 
	float v_0 = 200;

	float alpha_u = 0.01;
	float alpha_v = 0.01;
		
	int cols = 400;
	int rows = 400;

    const char buffer1[16]={0};
    int maxval = 255;

	// RIGID TRANSFORMATION
		struct point3d *rigid_tranform_points;
		rigid_tranform_points = (struct point3d *) malloc(N_v * sizeof(struct point3d));
		float TransMatrix[16];
		computeTrans(0.0, 0.5, 0.7, 0.5, 0.5, 0.5, TransMatrix);

		for(int i = 0; i < N_v; i++){
			rigid_tranform_points[i].x = points[i].x*TransMatrix[1] + points[i].y*TransMatrix[2] + points[i].z*TransMatrix[3] + TransMatrix[4];
			rigid_tranform_points[i].y = points[i].x*TransMatrix[5] + points[i].y*TransMatrix[6] + points[i].z*TransMatrix[7] + TransMatrix[8];
			rigid_tranform_points[i].z = points[i].x*TransMatrix[9] + points[i].y*TransMatrix[10] + points[i].z*TransMatrix[11] + TransMatrix[12];
			rigid_tranform_points[i].r = points[i].r;
			rigid_tranform_points[i].g = points[i].g;
			rigid_tranform_points[i].b = points[i].b;
		}
		//End rigid transformation

		//Pinhole projection with rigid transformation
		point2d_t pinhole_points_rigid_tranform[N_v]; // saves the pinhole projection (X_img, Y_img)
		for (int i = 0; i < N_v; i++)
		{
			pinhole_points_rigid_tranform[i].x = rigid_tranform_points[i].x/(1 + rigid_tranform_points[i].z/f);
			pinhole_points_rigid_tranform[i].y = rigid_tranform_points[i].y/(1 + rigid_tranform_points[i].z/f); 
			pinhole_points_rigid_tranform[i].r = rigid_tranform_points[i].r;
			pinhole_points_rigid_tranform[i].g = rigid_tranform_points[i].g;
			pinhole_points_rigid_tranform[i].b = rigid_tranform_points[i].b;
		}
		//END Pinhole projection with rigid transformation

		// ------------------U V Projection With rigid---------------
		point2d_int_t uv_projection_rigid_transform[N_v];  // storing the u and v: pixel coordinates

		for (int i = 0; i < N_v; i++)
		{
			uv_projection_rigid_transform[i].x = pinhole_points_rigid_tranform[i].x/alpha_u + u_0;
			uv_projection_rigid_transform[i].y = pinhole_points_rigid_tranform[i].y/alpha_v + v_0;
			uv_projection_rigid_transform[i].r = pinhole_points_rigid_tranform[i].r;
			uv_projection_rigid_transform[i].g = pinhole_points_rigid_tranform[i].g;
			uv_projection_rigid_transform[i].b = pinhole_points_rigid_tranform[i].b;
		}

		colors_t* gradient_rigid_transform;
		gradient_rigid_transform = (colors_t *) malloc(rows * cols * sizeof(colors_t));

		fputs("P3\n",ifp3);
		sprintf(buffer1,"%d ", cols);
		fputs(buffer1, ifp3);
		sprintf(buffer1,"%d\n", rows);
		fputs(buffer1, ifp3);
		sprintf(buffer1,"%d\n", maxval);
		fputs(buffer1, ifp3);

		for(int k = 0; k < N_v; k++){
			if(uv_projection_rigid_transform[k].x > 0 && uv_projection_rigid_transform[k].x < rows && uv_projection_rigid_transform[k].y > 0 && uv_projection_rigid_transform[k].y < cols){
				colors_t color;
				color.r = uv_projection_rigid_transform[k].r;
				color.g = uv_projection_rigid_transform[k].g;
				color.b = uv_projection_rigid_transform[k].b;
				gradient_rigid_transform[uv_projection_rigid_transform[k].x * cols + uv_projection_rigid_transform[k].y] = color;
			}
		}
		// ------------------End U V Projection With rigid---------------
	//END RIGID TRANSFORMATION



	//NO RIGID TRANSFORMATION
		//----------------- Pinhole Projection Without rigid transformation
		point2d_t pinhole_points[N_v]; // saves the pinhole projection (X_img, Y_img)

		for (int i = 0; i < N_v; i++)
		{
			pinhole_points[i].x = points[i].x/(1 + points[i].z/f);
			pinhole_points[i].y = points[i].y/(1 + points[i].z/f); 
			pinhole_points[i].r = points[i].r;
			pinhole_points[i].g = points[i].g;
			pinhole_points[i].b = points[i].b;
		}

		//----------------------END Pinhole Projection without rigid transformation

		// ------------------U V Projection Without rigid---------------
		point2d_int_t uv_projection[N_v];  // storing the u and v: pixel coordinates

		for (int i = 0; i < N_v; i++)
		{
			uv_projection[i].x = pinhole_points[i].x/alpha_u + u_0;
			uv_projection[i].y = pinhole_points[i].y/alpha_v + v_0;
			uv_projection[i].r = pinhole_points[i].r;
			uv_projection[i].g = pinhole_points[i].g;
			uv_projection[i].b = pinhole_points[i].b;
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


		// colors_t white;
		// white.r =0;
		// white.g =0;
		// white.b =255;
		// for (int i = 0; i < rows; i++){
		// 	for (int j = 0; j < cols; j++)
		// 	{
		// 	gradient[i * cols + j] = white;
		// 	}
		// }

		for(int k = 0; k < N_v; k++){
			if(uv_projection[k].x > 0 && uv_projection[k].x < rows && uv_projection[k].y > 0 && uv_projection[k].y < cols){
				colors_t color;
				color.r = uv_projection[k].r;
				color.g = uv_projection[k].g;
				color.b = uv_projection[k].b;
				gradient[uv_projection[k].x * cols + uv_projection[k].y] = color;
			}			
		}
	//END NO RIGID TRANSFORMATION
		
    /* Write to file both rigid and nonrigid*/ 
    for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++)
		{
			sprintf(buffer1,"%d ", gradient[i * cols + j].r);
			fputs(buffer1, ifp2);
			sprintf(buffer1,"%d ", gradient[i * cols + j].g);
			fputs(buffer1, ifp2);
			sprintf(buffer1,"%d ", gradient[i * cols + j].b);
			fputs(buffer1, ifp2);

			sprintf(buffer1,"%d ", gradient_rigid_transform[i * cols + j].r);
			fputs(buffer1, ifp3);
			sprintf(buffer1,"%d ", gradient_rigid_transform[i * cols + j].g);
			fputs(buffer1, ifp3);
			sprintf(buffer1,"%d ", gradient_rigid_transform[i * cols + j].b);
			fputs(buffer1, ifp3);
		}
		sprintf(buffer1,"%s", "\n");
  		fputs(buffer1, ifp2);
		sprintf(buffer1,"%s", "\n");
  		fputs(buffer1, ifp3);
    }
	
	fclose(ifp1);
	fclose(ifp2);
	fclose(ifp3);
}
