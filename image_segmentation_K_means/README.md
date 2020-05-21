# Image Segmentation: K-Means
Implementaion of Image Segmentation using K-Means considering the pixel color and, weighted pixel position and color.


#### Compile the programs
Using the Makefile
```
make all
```
Compiles two programs:
 * color_intensity_kmeans
 * spatial_color_kmeans

## K-Means: Pixel Color Intensity
File: color_intensity_kmeans.c

Segmentation of an image using K-means, taking color intensity of a pixel of the image into account.

## K-Means: Pixel Position and Pixel Color Intensity
File: spatial_color_kmeans.c

Segmentation of an image using K-means, based on the pixel position and their color intersity.


#### How to run
```
./color_intensity_kmeans <Image_filename.ppm>  <Number_of_K_clusters> <filename_tobe_segmented> <number_of_interations>
```


```
./spatial_color_kmeans <Image_filename.ppm>  <Number_of_K_clusters> <filename_tobe_segmented> <number_of_interations>
```


![color_intensity_results](https://github.com/mirjetash/visual_computing/blob/master/image_segmentation_K_means/For_visualization_github/color_intensity_results.png)


![spatial_results](https://github.com/mirjetash/visual_computing/blob/master/image_segmentation_K_means/For_visualization_github/spatial_results.png)

