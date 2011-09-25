/**
 * (C) Copyright 2011. All rights are reserved. Sotiris L Karavarsamis.
 * Given two identical in size images, the code creates a morph images
 * by combining the colors of the two images.
 *
 * Compiling is straightforward: use "make" in order to build the
 * source.
 * 
 * To run the code, use "./linear_blend <image1> <image2>". The images
 * shall be the same dimensions.
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>

int alpha = 50;
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
	
	cvShowImage("Blended image", final);
}

int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "%s <image-1> <image-2>\n", argv[0]);
		return 1;
	}
	
	// load images
	image1 = cvLoadImage(argv[1], 1);
	image2 = cvCreateImage(cvGetSize(image1), image1->depth, image1->nChannels);
	image3 = cvLoadImage(argv[2], 1);
	if(image1 == NULL || image2 == NULL || image3 == NULL) {
		return 1;
	}
	
	cvResize(image3, image2);

	final = cvCloneImage(image1);
	
	cvNamedWindow("Blended image", 1);
	cvShowImage("Blended image", final);

    cvCreateTrackbar("Alpha", "Blended image", &alpha, 100, blending);

	blending(alpha);

	cvWaitKey(0);

	cvReleaseImage(&image1);
	cvReleaseImage(&image2);
	cvReleaseImage(&final);
	
	return 1;
}
