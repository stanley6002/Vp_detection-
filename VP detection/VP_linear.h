//
//  VP_linear.h
//  VP detection
//
//  Created by chih-hsiang chang on 2/22/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//


#ifndef VP_LINEAR_H
#define VP_LINEAR_H


#include "Jlinkage.h"
#include "Utility.h"
#include "matrix_OP.h"
#include "VP_nonlinear.h"
#include <cstdio>


void linear_estimation(); 
void NL_estimation_driver(double*vVp, int k,pt3 *VP_result);


#endif 