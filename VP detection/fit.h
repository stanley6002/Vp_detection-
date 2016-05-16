//
//  fit.h
//  VP detection
//
//  Created by chih-hsiang chang on 1/21/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//


#ifndef __fit_h__
#define __fit_h__


#include <cstdio>
#include <iostream>
#include <cv.h>
#include <algorithm> 
#include <highgui.h>
#include "matrix_OP.h"
#include <stdio.h>
#include <math.h>
#include "edge_detection .h"
#include "DType .h"
//using namespace std;


                  //extern const int col = maxima_trial;


//typedef struct  xypoint
//{
//    int x;
//    int y;
//
//} point;
//
//typedef struct  Cpoint
//{
//    double x;
//    double y;
//} point_D;
//
//typedef struct  C1Lpt
//{
//    double p1;
//    double p2;
//    double p3;
//} pt3;

class Edge1
{
public:
//    Edge1()
//    {       
//    }
 double vL_[3];   
 point pointUn1_;
 point pointUn2_;
 point_D Cpoint;
 double skMat[9];
 double VLt[9];
 pt3 C1Lpt;   
};


class Edge2
{
public:
//    Edge2()
//    {
//        Edge_lst= new Edge1;
//    }
    // Edge2 (int num) : size_(num)
    //{
    //    Edge_lst= new Edge1[num];  
    //}
    ~Edge2()
    {
        delete [] Edge_lst;
    }
    void Initialize(int num);
    Edge2&  Norm_(int x, double*point);  
    Edge2& pointUn_Rec(int x, CvPoint2D32f& point1, CvPoint2D32f& point2);
    Edge2& VLt_(int location);
    double VLt_read(int location, int Num);
    
    void  Print_VLt(int location);
    void  Print_VL(int location);
    void  skm_Cpoint(int& location, double*);
    void  Gen_skm(double &x, double &y , double *);
    void  Gen_C1Lpoint_Distance(int location);
    void  GetskMat(int location, double *skmat);
    void  GetPoint(int location, double *point);
    point_D GetCpoint(int location);
    pt3   GetC1Lpt(int location);

    
    inline double operator()(const int location, const int rowIndx) 
    {
        return (Edge_lst[location].vL_[rowIndx]);
    }
    void pointUn(int location);
    int size() const ;
private:
    int size_;
    // double vL_[3];
    Edge1* Edge_lst;
};
inline point_D Edge2::GetCpoint(int location)
{
    return(Edge_lst[location].Cpoint);
}
inline double  Edge2::VLt_read(int location,int Num)
{
    return(Edge_lst[location].VLt[Num]);
}
inline void Edge2::Initialize(int num)
{
    Edge_lst= new Edge1[num];
    size_=num;
}
inline  int Edge2::size() const
{
    return(size_);
}
inline pt3 Edge2::GetC1Lpt(int location)
{
   
    return(Edge_lst[location].C1Lpt);

}
inline void  Edge2::Print_VLt(int location)
{
    std::cout<<Edge_lst[location].VLt[0]<<" "<<Edge_lst[location].VLt[1]<<" "<<Edge_lst[location].VLt[2]<<endl;
    std::cout<<Edge_lst[location].VLt[3]<<" "<<Edge_lst[location].VLt[4]<<" "<<Edge_lst[location].VLt[5]<<endl;
    std::cout<<Edge_lst[location].VLt[6]<<" "<<Edge_lst[location].VLt[7]<<" "<<Edge_lst[location].VLt[8]<<endl;
}
inline void Edge2::Print_VL(int location)
{
    std::cout<<Edge_lst[location].vL_[0]<<" "<<Edge_lst[location].vL_[1]<<" "<<Edge_lst[location].vL_[2]<<endl;
    //std::cout<<Edge_lst[location].vL_[1]<<endl;
    //std::cout<<Edge_lst[location].vL_[2]<<endl;
}
inline void  Edge2::skm_Cpoint(int &location, double* skmat)
{
    double x1= (Edge_lst[location].pointUn1_.x);
    double x2= (Edge_lst[location].pointUn2_.x);
    double x=(x1+x2)/2;
    double y1= Edge_lst[location].pointUn1_.y;
    double y2= Edge_lst[location].pointUn2_.y;
    double y= (y1+y2)/2;
    Edge_lst[location].Cpoint.x=x;
    Edge_lst[location].Cpoint.y=y;
    //std::cout<<Edge_lst[location].pointUn1_.x<<" "<<Edge_lst[location].pointUn1_.y<<endl;
    //std::cout<<Edge_lst[location].pointUn2_.x<<" "<<Edge_lst[location].pointUn2_.y<<endl;
    //std::cout<<x<<" "<<y;
    Gen_skm(x,y,skmat);   
   }
inline void  Edge2::Gen_C1Lpoint_Distance(int location)
{

    double x1= (Edge_lst[location].skMat[0]*Edge_lst[location].pointUn1_.x)+(Edge_lst[location].skMat[1]*Edge_lst[location].pointUn1_.y)+(Edge_lst[location].skMat[2]);
    double x2= (Edge_lst[location].skMat[3]*Edge_lst[location].pointUn1_.x)+(Edge_lst[location].skMat[4]*Edge_lst[location].pointUn1_.y)+(Edge_lst[location].skMat[5]);
    double x3= (Edge_lst[location].skMat[6]*Edge_lst[location].pointUn1_.x)+(Edge_lst[location].skMat[7]*Edge_lst[location].pointUn1_.y)+(Edge_lst[location].skMat[8]);
    
     //    std::cout<<Edge_lst[location].skMat[0]<<" "<<Edge_lst[location].skMat[1]<<" "<<Edge_lst[location].skMat[2]<<endl;
     //    std::cout<<Edge_lst[location].skMat[3]<<" "<<Edge_lst[location].skMat[4]<<" "<<Edge_lst[location].skMat[5]<<endl;
     //    std::cout<<Edge_lst[location].skMat[6]<<" "<<Edge_lst[location].skMat[7]<<" "<<Edge_lst[location].skMat[8]<<endl;
    
     //    std::cout<<"Normal_ ";
     //    std::cout<< Edge_lst[location].pointUn1_.x<<" "<<Edge_lst[location].pointUn1_.y<<endl; 

    Edge_lst[location].C1Lpt.p1=x1;
    Edge_lst[location].C1Lpt.p2=x2;
    Edge_lst[location].C1Lpt.p3=x3;            
   
    //std::cout<<x1<<" "<<x2<<" "<<x3<<endl;
}
inline void  Edge2::Gen_skm(double &x, double &y , double* skemat)
{
    double V[3]={x, y, 1};  
    //std::cout<<x<<" "<<y<<endl;
    skemat[0]=0; 
    skemat[1]=-V[2];
    skemat[2]=V[1];
    skemat[3]=V[2];
    skemat[4]=0;
    skemat[5]=-V[0];
    skemat[6]=-V[1];
    skemat[7]=V[0];
    skemat[8]=0;
}
inline void Edge2::pointUn(int location)
{
    //std::cout<<endl;
    std::cout<<Edge_lst[location].pointUn1_.x<<" "<<Edge_lst[location].pointUn1_.y<<endl;
    std::cout<<Edge_lst[location].pointUn2_.x<<" "<<Edge_lst[location].pointUn2_.y<<endl;

}
inline Edge2& Edge2::VLt_(int location)
{
      Edge_lst[location].VLt[0]=Edge_lst[location].vL_[0]*Edge_lst[location].vL_[0];  Edge_lst[location].VLt[3]=Edge_lst[location].vL_[1]*Edge_lst[location].vL_[0];
      Edge_lst[location].VLt[1]=Edge_lst[location].vL_[0]*Edge_lst[location].vL_[1];  Edge_lst[location].VLt[4]=Edge_lst[location].vL_[1]*Edge_lst[location].vL_[1];
      Edge_lst[location].VLt[2]=Edge_lst[location].vL_[0]*Edge_lst[location].vL_[2];  Edge_lst[location].VLt[5]=Edge_lst[location].vL_[1]*Edge_lst[location].vL_[2];
      
      Edge_lst[location].VLt[6]=Edge_lst[location].vL_[2]*Edge_lst[location].vL_[0];
      Edge_lst[location].VLt[7]=Edge_lst[location].vL_[2]*Edge_lst[location].vL_[1];
      Edge_lst[location].VLt[8]=Edge_lst[location].vL_[2]*Edge_lst[location].vL_[2];
      
     return(*this);
    
}
inline Edge2& Edge2::Norm_(int location, double*point)
{
    double norm_ = sqrt((point[0]* point[0])+(point[1]* point[1])+(point[2]* point[2]));
    Edge_lst[location].vL_[0]= point[0]/norm_;
    Edge_lst[location].vL_[1]= point[1]/norm_;
    Edge_lst[location].vL_[2]= point[2]/norm_;
    return(*this);
}
inline Edge2& Edge2:: pointUn_Rec(int location, CvPoint2D32f& point1, CvPoint2D32f& point2)
{
    Edge_lst[location].pointUn1_.x= point1.x;
    Edge_lst[location].pointUn1_.y= point1.y;
    Edge_lst[location].pointUn2_.x= point2.x;
    Edge_lst[location].pointUn2_.y= point2.y;
    //std::cout<<Edge_lst[location].pointUn2_.x<<" "<<Edge_lst[location].pointUn2_.y<<endl;
    
    double* skm= new double [9];
    skm_Cpoint(location, skm);
    
    Edge_lst[location].skMat[0]= skm[0];  Edge_lst[location].skMat[1]= skm[1];
    Edge_lst[location].skMat[2]= skm[2];  Edge_lst[location].skMat[3]= skm[3];
    Edge_lst[location].skMat[4]= skm[4];  Edge_lst[location].skMat[5]= skm[5];
    Edge_lst[location].skMat[6]= skm[6];  Edge_lst[location].skMat[7]= skm[7];
    Edge_lst[location].skMat[8]= skm[8];  
    // pointUn(location);
    //matrix_print(3,3,skm);
    Gen_C1Lpoint_Distance(location); 
    delete [] skm;
   
    
    return(*this);
}
inline void Edge2::GetskMat(int location, double *skmat)
{
    skmat[0]= Edge_lst[location].skMat[0];skmat[1]= Edge_lst[location].skMat[1];
    skmat[2]= Edge_lst[location].skMat[2];skmat[3]= Edge_lst[location].skMat[3];
    skmat[4]= Edge_lst[location].skMat[4];skmat[5]= Edge_lst[location].skMat[5];
    skmat[6]= Edge_lst[location].skMat[6];skmat[7]= Edge_lst[location].skMat[7];
    skmat[8]= Edge_lst[location].skMat[8];
}
inline void Edge2::GetPoint(int location, double *point)
{
    point[0]= Edge_lst[location].pointUn1_.x;
    point[1]= Edge_lst[location].pointUn1_.y;
   //std::cout<<point[0]<<" "<<point[1]<<endl;
}
extern std::vector<std::vector<bool> > errorlst; 
extern Edge2 Edg;

bool Gen_Hyp(int &i, int &j, double *VP);
void V_hyperror(int &location, double *, double *);
void fit(std::vector<std::vector<CvPoint> > &edgelist);
void PrintMatrix(CvMat *Matrix,int Rows,int Cols);
void matrix_transpose(int m, int n, double *A, double *AT);
int  dgesvd_driver(int m, int n, double *A, double *U, double *S, double *VT);
void Random_selection(int &Indx1, int &Indx2,int size_);
void Random_selection_Non(int &Indx1, int &Indx2, int &size_);
int *NonUniform(std::vector<std::vector<CvPoint> > &edgelist, int &size_);
void  VP_pairs(int &Indx1, int &Indx2, double * VP);
vector<vector <bool> > Hypothesis_Generation(std::vector<std::vector<CvPoint> > &edgelist, int NumEdges /*numEdges*/ , int NumTriasl /* trials*/, bool Uniform, int*CandidacyList,int NumberofCandidacy );
void CandidacySelection( vector<vector<CvPoint> >&edgelist, int NumofCandidacy, int*CandidacyList ); 
int *NonUniform_N(std::vector<std::vector<CvPoint> >&edgelist,int &size_random,int *Candidacylist_Partial,int NumberofCandidacy );
//#ifdef __cplusplus
//}
//#endif

#endif 
