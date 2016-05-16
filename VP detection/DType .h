//
//  DType .h
//  VP detection
//
//  Created by chih-hsiang chang on 2/26/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#ifndef __DType_h__
#define __DType_h__

#ifdef __cplusplus
extern "C"
{
#endif 
//#include "fit.h"
//    void NLVPoptimization(double *Vangle,point_D* Centerpt,pt3  *Cline);
    
# define Canny_thd 800
# define Canny_mask 5
# define BLACK 0
# define WHITE 255
# define line_threshold 25
# define minLength      40
# define tolerance      2.0   //  line tolerance
# define segmentation    50  
# define maxima_trial   10000
# define error_deviation  0.0001
# define error_deviation1 0.01  
# define Focallength 500
// # define deviation 2.0;
 typedef struct  xypoint
    {
        double x;
        double y;
        
    } point;
 typedef struct  Cpoint
    {
        double x;
        double y;
    } point_D;
    
 typedef struct  C1Lpt
    {
        double p1;
        double p2;
        double p3;
    } pt3;

  #ifdef __cplusplus
};
#endif

#endif





