//
//  matrix_OP.c
//  VP detection
//
//  Created by chih-hsiang chang on 1/21/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//
#define N 5
#define LDA N
#include "matrix_OP.h"
#include <cminpack.h>
void dsyev_driver(int n1, double*mat, int lda1, double *w1)
{
     
    //int n = N;
    //int lda = LDA; 
    int info;
    int lwork;
    double wkopt[50];
    //double* work;
    /* Local arrays */
     double w[N];
     lwork = 50; 
     dsyev( "Vectors", "L", &n1, mat, &lda1, w, &wkopt, &lwork, &info );
     //print_matrix( "Eigenvalues", 1, n1, w, 1 );
      w1[0]= w[0];
      w1[1]= w[1];
      w1[2]= w[2];
}

int dgesvd_driver(int m, int n, double *A, double *U, double *S, double *VT) {
    double *AT, *UT, *V;
    
    char jobu = 'a';
    char jobvt = 'a';
    
    int lda = m;
    int ldu = m;
    int ldvt = n;
    
    int lwork = 10 * MAX(3 * MIN(m, n) + MAX(m, n), 5 * MIN(m, n));
    double *work;
    
    int info;
    
    /* Transpose A */
    AT = (double *)malloc(sizeof(double) * m * n);    
    matrix_transpose(m, n, A, AT);
    
    /* Create temporary matrices for output of dgesvd */
    UT = (double *)malloc(sizeof(double) * m * m);
    V = (double *)malloc(sizeof(double) * n * n);
    
    work = (double *)malloc(sizeof(double) * lwork);
    // double *work = new double [(int)lwork];
    
    
    dgesvd_(&jobu, &jobvt, &m, &n, AT, &lda, S, UT, &ldu, V, &ldvt, work, &lwork, &info);
    
    if (info != 0) 
    {
        printf("[dgesvd_driver] An error occurred\n");
    }
    
    matrix_transpose(m, m, UT, U);
    matrix_transpose(n, n, V, VT);
    
    free(AT);
    free(UT); 
    free(V);
    free(work);
    
    if (info == 0)
        return 1;
    else
        return 0;
}
void matrix_transpose(int m, int n, double *A, double *AT) {
    int i, j;
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            AT[j * m + i] = A[i * n + j];
}
/* Compute the matrix product R = A^T B */
void matrix_transpose_product(int Am, int An, int Bm, int Bn, double *A, double *B, double *R) {
    int r = An;
    int c = Bn;
    int m = Am;
    
    //#if defined(WIN32) || defined(NO_CBLAS)
    int i, j, k;
    //#endif
    
    if (Am != Bm) {
        printf("Error: the number of rows of A and the "
               "number of rows of B must be equal\n");
        return;
    }
    
    //#if !defined(WIN32) && !defined(NO_CBLAS)
    //    cblas_dgemm_driver_transpose(r, m, c, A, B, R);
    //#else
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            R[i * c + j] = 0.0;
            for (k = 0; k < m; k++) {
                R[i * c + j] += A[k * An + i] * B[k * Bn + j];
            }
        }
    }
    //#endif
}

/* Compute the matrix product R = A B^T */
void matrix_transpose_product2_old(int Am, int An, int Bm, int Bn, 
                                   double *A, double *B, double *R) 
{
    int r = Am;
    int c = Bm;
    int m = An;
    
    int i, j, k;
    
    if (An != Bn) {
        printf("Error: the number of columns of A and the "
               "number of columns of B must be equal\n");
        return;
    }
    
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            R[i * c + j] = 0.0;
            for (k = 0; k < m; k++) {
                R[i * c + j] += A[i * An + k] * B[j * Bn + k];
            }
        }
    }
}
void matrix_print(int m, int n, double *A) {
    int i, j;
    
    for (i = 0; i < m; i++) {
        printf("  ");
        for (j = 0; j < n; j++) {
            printf(" %0.6e ", A[i * n + j]);
        }
        printf("\n");
    }
}
void matrix_product(int Am, int An, int Bm, int Bn, 
                    const double *A, const double *B, double *R)
{
    int r = Am;
    int c = Bn;
    int m = An;
    
    //#if defined(WIN32) || defined(NO_CBLAS)
    int i, j, k;
    //#endif
    
    if (An != Bm) {
        printf("[matrix_product] Error: the number of columns of A and the "
               "number of rows of B must be equal\n");
        return;
    }
    
    //#if !defined(WIN32) && !defined(NO_CBLAS)
    //    cblas_dgemm_driver(r, m, c, (double *) A, (double *) B, R);
    //#else
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            R[i * c + j] = 0.0;
            for (k = 0; k < m; k++) {
                R[i * c + j] += A[i * An + k] * B[k * Bn + j];
            }
        }
    }
    //#endif
}
void print_matrix( char* desc, int m, int n, double* a, int lda ) {
    int i, j;
    printf( "\n %s\n", desc );
    for( i = 0; i < m; i++ ) {
        for( j = 0; j < n; j++ ) printf( " %6.2f", a[i+j*lda] );
        printf( "\n" );
    }
}