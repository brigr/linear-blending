/**
 * (C) Copyright 2011. All rights are reserved. Sotiris L Karavarsamis.
 * Given two identical in size images, the code creates a morph images
 * by combining the color information contained in the two images.
 *
 * Compiling is straightforward: use "make" in order to build the
 * source.
 * 
 * To run the code, use "./linear_blend <image1> <image2>". The images
 * shall be the same dimensions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <getopt.h>

int alpha = 50;
int doBlur = 0;
IplImage *image1, *image2, *image3, *final;

void blending(int alpha) {
	int i, j;
	
	for(i = 0; i < image1->width; i++) {
        for(j = 0; j < image1->height; j++) {
            // get pixel
            CvScalar pix1 = cvGet2D(image1, j, i);
            CvScalar pix2 = cvGet2D(image2, j, i);

            // set pixel
            cvSet2D(final, j, i, cvScalar((1 - (double)alpha / 100)*pix1.val[0] + ((double)alpha / 100)*pix2.val[0],
                                          (1 - (double)alpha / 100)*pix1.val[1] + ((double)alpha / 100)*pix2.val[1],
                                          (1 - (double)alpha / 100)*pix1.val[2] + ((double)alpha / 100)*pix2.val[2]
										 )
				   );
        }
    }
	
	// do some optional blurring
	if(doBlur)
		cvSmooth(final, final, CV_BLUR, 3, 3);
	
	cvShowImage("Blended image", final);
}

void help() {
	fprintf(stderr,	"(C) Copyright 2011. All rights reserved. Sotiris L Karavarsamis.\n"
					"linear-blend help:\n"
					"-b\tforce blurring of output image\n"
					"-i\tspecify input image\n"
					"-t\tspecify target image\n");
}

int main(int argc, char **argv) {
	int c;

	char *inputImage = NULL;
	char *outputImage = NULL;

	while((c = getopt(argc, argv, "i:t:hb")) != -1) {
	  switch(c) {
		case 'b':
			doBlur = 1;
			break;
		
		case 'i':
			inputImage = strdup(optarg);
			break;
		
		case 'o':
			outputImage = strdup(optarg);
			break;
		
		case 'h':
			help();
			exit(EXIT_SUCCESS);
	  }
	}
	
	if(!inputImage || !outputImage) {
		help();
		exit(EXIT_FAILURE);
	}
	
	// load images
	image1 = cvLoadImage(inputImage, 1);
	image2 = cvCreateImage(cvGetSize(image1), image1->depth, image1->nChannels);
	image3 = cvLoadImage(outputImage, 1);
	if(image1 == NULL || image2 == NULL || image3 == NULL) {
		return 1;
	}
	
	// equalize image sizes
	cvResize(image3, image2);
	
	// copy image
	final = cvCloneImage(image1);
	
	// create highgui window
	cvNamedWindow("Blended image", 1);
	cvShowImage("Blended image", final);
	
	// create slider
	cvCreateTrackbar("Alpha", "Blended image", &alpha, 100, blending);
	
	// initialize view
	blending(alpha);
	
	// wait for ESC key
	cvWaitKey(0);
	
	// release image memory
	cvReleaseImage(&image1);
	cvReleaseImage(&image2);
	cvReleaseImage(&final);
	
	// return to the operating system
	return 1;
}
