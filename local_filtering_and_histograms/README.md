# Local Filtering and Historgrams

## Filtering

* File binomial_filter.c
A Program that reads a PGM image and computes the image smoothed with the following binomial filter:
```
	       1 2 1
b(m,n) = 1/26 [2 4 2] 
	       1 2 1
```

* File median.c
A program that implements and applies a median filter to an image.


## Compiling the code

A Makfile is procided to automatically build both programs.

```
make all
```

To run

```
./median <image_file.pgm>
./binomial_filter <image.file.pgm>
```

