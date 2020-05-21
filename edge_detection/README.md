# Edge Detection
Implementation of edge detection approach using gradients and Canny's strategy.

## Computing Gradient

Program file: computing_gradient.c

Using Scharr operators written below it computes the I(x) and I(y) of the PGM image, and then from them it computes the gradient magnitudes.

```
 	    -3  0 3
h1 = 1/16 [-10 0 10]
	    -3 0 3

 	  -3 -10 -3
h2 = 1/16 [0   0  10]
	   3   10  3
```

## User defined threshold

Program file: user_threshold.c

Generates image that displays pixels with gradient magnitudes above a threshold defined by the user.


## Edge Detection: Canny's Approach

Program file: non_maximum_suppression.c, hysteresis.c

Computes edge detection in an image using Canny's approach:
* Non maximum supression: extract local gradient extrema in the gradient direction. 
When a pixel A has an intensity that is larger than B and C, where B and C are pixels in the gradient direction of A, then we keep that pixel, otherwise we change the pixel to zero.

* Hysteresis thresholding
This is applied to the image after Non Maximum Supression. The idea is to keep the pixels on the edges that are above a higher threshold, and we remove the pixels below a lower threshold. For the pixels between these two values we decide to keep it if it is connected to a strong pixel (a pixel above the higher threshold) and we  do not keep (set it to zero) it, if it not connected to a strong pixel.


## Compiling
TO compile the programs
```
make all
```
Creates three programs: computing_gradient, user_threshold, non_maximum_suppression, hysteresis 

## Running the programs
As an example to show the result of each program I used the following image boat.pgm


![boat](https://github.com/mirjetash/visual_computing/blob/master/edge_detection/For_visualization_github/boat.png)

##### To run computing_gradient
```
./computing_gradient <image_filenname.pgm>
```
Generates three images:
 * filterx.pgm: Gradient x of the image
 * filtery.pgm: Gradient y of the image
 * gradient.pgm: Gradient magnitudes of the image

![filterx](https://github.com/mirjetash/visual_computing/blob/master/edge_detection/For_visualization_github/filterx.png?raw=true)
Image filterx
![filtery](https://github.com/mirjetash/visual_computing/blob/master/edge_detection//For_visualization_github/filtery.png?raw=true)
Image filtery
![gradient](https://github.com/mirjetash/visual_computing/blob/master/edge_detection/For_visualization_github/gradient.png?raw=true)
Image gradient

##### To run user_threshold
```
./user_threshold <image_filename.pgm> <threshold_value>
```
Generates an image gradient_threshold.pgm
![gradient_threshold](https://github.com/mirjetash/visual_computing/blob/master/edge_detection/For_visualization_github/gradient_threshold.png?raw=true)
Image gradient_threshold with threshold 50

##### To run non_maximum_supression
```
./non_maximum_supression <image_filename.pgm> 
```
Generates an image gradient_min.pgm
![gradient_min](https://github.com/mirjetash/visual_computing/blob/master/edge_detection/For_visualization_github/gradient_min.png?raw=true)

##### To run hysteresis
```
./hysteresis <image_filename.pgm> <low_threshold_value> <high_threshold_value>
```
**Note** The image passed as an argument should be the result(image) of the non_maximum_supression program

E.g.:   ./hysteresis gradient_min.pgm 10 30

Generates  an image hysteresis_1.pgm
![hysteresis_1](https://github.com/mirjetash/visual_computing/blob/master/edge_detection/For_visualization_github/hysteresis_1.png)
Image hysteresis_1 with low threshold 10 and high threshold 30
