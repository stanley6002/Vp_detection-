//
//  edge_detection .h
//  VP detection
//
//  Created by chih-hsiang chang on 1/15/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//


#ifndef __edge_detection_h__
#define __edge_detection_h__

 
#include <cstdio>
#include "DType .h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <vector>
#include <cmath>


#define  uint8 unsigned char
#define  uint  unsigned int

extern  int ImgWidth;
extern  int ImgHeight;

template<class T> class Image 
{
private:
    IplImage* imgp;
    
public:
    Image(IplImage* img=0) 
    {imgp=img;}
    ~Image(){imgp=0;}
    void operator=(IplImage* img) {imgp=img;}
    inline T* operator[](const int rowIndx) 
    {
        return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));
    }
    inline T operator()(const int rowIndx, const int columIndx)
    {
        return (imgp->imageData[rowIndx*imgp->width+columIndx]);
    }
    inline int width()
    {
        return (imgp->width);
    }
    inline int height()
    {
        return (imgp->height); 
    }
   void show_image()
    {
        cvShowImage("example", imgp);
        cvWaitKey();
    }
};
vector<vector<CvPoint> > edge_detection(IplImage* Iplimage);
void  eraseBorder(IplImage* iplEdgeT);
void  eliminateJunction(IplImage* iplEdge, uint8 edgC, uint8 backC);
std::vector< CvPoint> floodfill(CvPoint seed, uint8 edgC, uint8 bckV, IplImage* img, int line_thr);
std::vector<std::vector<CvPoint> > line_segmentation(std::vector<std::vector <CvPoint> > vComp , int &tol);
void linemaxdev(vector<CvPoint>line, float& max, int  &index);

#endif 