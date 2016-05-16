//
//  VP_nonlinear.h
//  VP detection
//
//  Created by chih-hsiang chang on 2/23/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

//#include "DType.h"

#ifndef __VP_nonlinear_h__
#define __VP_nonlinear_h__
# include <cminpack.h>
# include "DType .h"
#ifdef __cplusplus
extern "C" {
    
     void NLVPoptimization(int NumEdg,double *Vangle, point_D* Centerpt,pt3 *Cline,pt3 *VP_result);
     //void NLVPoptimization(int NumEdg,double *Vangle, point_D* Centerpt,pt3 *Cline);
     void fcn_W(const int *m, const int *n, const double *x, double *fvec, int *iflag);
     void fcn_noW(const int *m, const int *n, const double *x, double *fvec, int *iflag);


#endif 
    
    
#ifdef __cplusplus
} 
#endif

#endif 




//void NLVPoptimization(double *Vangle, point_D* Centerpt,pt3  *Cline);











