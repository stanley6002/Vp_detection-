//
//  VP_nonlinear.c
//  VP detection
//
//  Created by chih-hsiang chang on 2/23/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "VP_nonlinear.h"
#include <math.h>

static int Global_NumEdge=0;
//static double *Global_Centerpt;
//static double *Global_Cline;
static point_D* Global_Centerpt;
static pt3 *Global_Cline;

void fcn_noW(const int *m, const int *n, const double *x, double *fvec, int *iflag)
{
    //printf("%f %f\n", x[0], x[1]);
    double vVP_0 = cos(x[0])*sin(x[1]);
    double vVP_1 = sin(x[0])*sin(x[1]);
    double vVP_2 = cos(x[1]);
    double *vL= malloc(3*sizeof(double));
    double *vC= malloc(2*sizeof(double));
    double sum=0;
    
    int i =0;
    for (i=0;i<Global_NumEdge;i++)
    {

        vL[0]= Global_Cline[i].p1;
        vL[1]= Global_Cline[i].p2;
        vL[2]= Global_Cline[i].p3;
        vC[0]= Global_Centerpt[i].x;
        vC[1]= Global_Centerpt[i].y;
   
        fvec[i] = vL[0]*vVP_0 + vL[1]*vVP_1 + vL[2]*vVP_2;
        // fvec[i] = vL[0]*x[0]+ vL[1]*x[1] + vL[2]*x[2];
        fvec[i] /= sqrt(((-vVP_2*vC[1] + vVP_1)*(-vVP_2*vC[1] + vVP_1)) + (((vVP_2*vC[0] - vVP_0))*(vVP_2*vC[0] - vVP_0)));    
        sum= sum+ fabs(fvec[i]);
    } 
    //printf("sum_error %f \n",sum);
    free(vC);
    free(vL);
    return;
}
void NLVPoptimization(int NumEdg,double *Vangle, point_D* Centerpt,pt3  *Cline, pt3 *VP_result)
{
    int nbIdx=NumEdg;
    Global_NumEdge= NumEdg;
    //double *NLVp= malloc(3*sizeof(double));
    double NLVp[3];
    //Global_Centerpt=malloc(2*NumEdg*sizeof(double));
    //Global_Cline= malloc (3*NumEdg*sizeof(double));
    Global_Centerpt= Centerpt;
    Global_Cline= Cline;
    #define LWA 400
    int info;
    int lwa = LWA;
    double tol=0.000000001;
    //    double wa[LWA];
    int *iwa;
    double *fvec;
    double *wa;
    //    int m = NumEdg;
     int n = 3;
     iwa = (int *)malloc(sizeof(int) * n);
     fvec = (double *)malloc(sizeof(double) * NumEdg);
     wa = (double *)malloc(sizeof(double) * lwa);

     lmdif1_(&fcn_noW, &nbIdx, &n, Vangle, fvec, &tol, &info, iwa, wa, &lwa);    
      // printf("info %i /n",info);
    
      //printf("NLVP %f %f/n",Vangle[0],Vangle[1]);
      NLVp[0] = cos(Vangle[0])*sin(Vangle[1]);
      NLVp[1] = sin(Vangle[0])*sin(Vangle[1]);
      NLVp[2] = cos(Vangle[1]);
     
    double nrm = sqrt(NLVp[0]*NLVp[0]+ NLVp[1]*NLVp[1] + NLVp[2]*NLVp[2]) ;
    NLVp[0] /= nrm;
    NLVp[1] /= nrm;
    NLVp[2] /= nrm;
    
    // printf("NLVP %f %f %f \n",NLVp[0],NLVp[1],NLVp[2]);

   VP_result->p1=NLVp[0];
   VP_result->p2=NLVp[1];
   VP_result->p3=NLVp[2];

    //(double *)malloc(sizeof(double) * m * m);
    //free(Global_NumEdge);
    free(Global_Centerpt);
    free(Global_Cline);
}
