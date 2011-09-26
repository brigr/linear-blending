/**
 * (C) Copyright 2011. All rights are reserved. Sotiris L Karavarsamis.
 * 
 * Given two identical-in-size images, the code creates a morph image
 * by combining the color information of the two original input images.
 */

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib/gthread.h>

#include <getopt.h>

using namespace std;

int alpha = 50;
int doBlur = 0;
IplImage *image1, *image2, *image3, *final;

IplImage* loadImage(char *file) {
	GdkPixbuf *pb;
	GdkColorspace csp;
	GError *gerror = NULL;
	
	IplImage *image;
	
	int width;
	int height;
	int rowstride;
	int chans;
	int bps;
	
	int x;
	int y;
	
	guchar *pixels;
	guchar *p;
	
	// check path
	if(file == NULL) {
		return NULL;
	}

	// load gdk pixbuf from file
	pb = gdk_pixbuf_new_from_file(file, &gerror);
	if(pb == NULL) {
		return NULL;
	}

	// get necessary file information
	width		= gdk_pixbuf_get_width(pb);
	height		= gdk_pixbuf_get_height(pb);
	rowstride	= gdk_pixbuf_get_rowstride(pb);
	chans		= gdk_pixbuf_get_n_channels(pb);
	bps			= gdk_pixbuf_get_bits_per_sample(pb);

	csp			= gdk_pixbuf_get_colorspace(pb);
	pixels		= gdk_pixbuf_get_pixels(pb);


	// create opencv image structure
	image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, chans);
	if(image == NULL) {
		gdk_pixbuf_unref(pb);
		return NULL;
	}

	for(x = 0; x < width; x++) {
		for(y = 0; y < height; y++) {
			// read channel values from pixbuf
			p = pixels + y * rowstride + x * chans;

			// copy RGB channel values to the IplImage structure
			cvSet2D(image, y, x, CV_RGB((int)p[0], (int)p[1], (int)p[2]));
		}
	}

	// free memory associated with image
	gdk_pixbuf_unref(pb);

	// return structure
	return(image);
}

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
	
	cvShowImage("Blender", final);
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
		
		case 't':
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
	
	// initialize gdk
	g_thread_init(NULL);
	g_type_init();


	// load images
	image1 = loadImage(inputImage);
	image3 = loadImage(outputImage);
	
	// create intermediate morph image
	image2 = cvCreateImage(cvGetSize(image1), image1->depth, image1->nChannels);
	
	// check if loading/creation went ok
	if(!image1 || !image2 || !image3) {
		return 1;
	}
	
	// equalize image sizes
	cvResize(image3, image2);
	
	// copy image
	final = cvCloneImage(image1);
	
	// create highgui window
	cvNamedWindow("Blender", 1);
	cvShowImage("Blender", final);
	
	// create slider
	cvCreateTrackbar("Alpha", "Blender", &alpha, 100, blending);
	
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
