# Image Formation

Program to project a 3D point cloud to an image plane using the pinhole camera model.

The process to form an image is:
 * Place the object properly in front of the camera nby applying rigid transformations to it
 * Project the object though pinhole to image plane
 * Fit the image plane in uv coordinates (pixel coordsinates) in an image.


#### Compile the programs
Using the Makefile
```
make all
```

## Input Files
3D files are .off format.
To read the .off file a function called readOff is implemented in imageFormationUtils.c



