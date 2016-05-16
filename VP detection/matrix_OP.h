//
//  matrix_OP.h
//  VP detection
//
//  Created by chih-hsiang chang on 1/21/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//


#ifndef __matrix_OP_h__
#define __matrix_OP_h__

#ifdef __cplusplus
extern "C" {
#endif
#define MAX(x,y) (((x) < (y)) ? (y) : (x))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
    
#define MAX4(a,b,c,d) (MAX(MAX(a,b), MAX(c,d)))
#define MIN4(a,b,c,d) (MIN(MIN(a,b), MIN(c,d)))
    
#define DEG2RAD(d) ((d) * (M_PI / 180.0))
#define RAD2DEG(r) ((r) * (180.0 / M_PI))
    
#define SQ(x) ((x) * (x))
#define CB(x) ((x) * (x) * (x))

    int  dgesvd_driver(int m, int n, double *A, double *U, double *S, double *VT);
    void matrix_transpose(int m, int n, double *A, double *AT);
    void matrix_print(int m, int n, double *A);
    void matrix_transpose_product(int Am, int An, int Bm, int Bn, double *A, double *B, double *R);
    void matrix_transpose_product2_old(int Am, int An, int Bm, int Bn, 
                                       double *A, double *B, double *R);
    void matrix_product(int Am, int An, int Bm, int Bn, 
                        const double *A, const double *B, double *R);
    
    void  dsyev_driver(int n, double*mat, int lda, double* w);
    void  print_matrix( char* desc, int m, int n, double* a, int lda );
     #ifdef __cplusplus
}
#endif

#endif 