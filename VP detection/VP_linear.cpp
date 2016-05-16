//
//  VP_linear.cpp
//  VP detection
//
//  Created by chih-hsiang chang on 2/22/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//
#include "VP_linear.h"
#include <cstdio>

void linear_estimation()
{
    int Num_cluster = (int)cluster_result.size(); 
    std::cout<<"Number of Cluster "<<Num_cluster<<endl;
    double* mAt = new double [9];
    double weight=1.0;
    int location=0;
    double *eig_value= new double[3];
    double *vVP= new double [3];
    pt3 *VP_result= new pt3[1];
   
    


for(int cluster_k=0;cluster_k<Num_cluster;cluster_k++)

    {
        //std::cout<<k<<" Kth_cluster "<<endl;
        location =(int)cluster_result[cluster_k].size();
    if (location>=6)
    {
    for (int j=0;j<location;j++)
       {
        int index = cluster_result[cluster_k][j];
         //std::cout<<index<<" ";
         //Edg.Print_VL(index);
         //std::cout<<endl;
           for (int i=0;i<9;i++)
            {
        
            mAt[i]+= weight *Edg.VLt_read(index,i);
           
            }
        //matrix_print(3, 3, mAt);
        }
        //;
        dsyev_driver(3, mAt,3,eig_value);  // eigen decomposition 
        //matrix_print(3, 3, mAt);
        eig_value[0] =fabs(eig_value[0]);
        eig_value[1] =fabs(eig_value[1]);
        eig_value[2] =fabs(eig_value[2]);
        //std::cout<<eig_value[0]<<" "<<eig_value[1]<<" "<<eig_value[2]<<endl;
        if (eig_value[0] < eig_value[1])
        {
            if (eig_value[0] < eig_value[2])
            {
                vVP[0] = mAt[0];
                vVP[1] = mAt[1];
                vVP[2] = mAt[2];
            }
            else
            {
                vVP[0] = mAt[6];
                vVP[1] = mAt[7];
                vVP[2] = mAt[8];
            }
        }
        else
        {
            if (eig_value[1] < eig_value[2])
            {
                vVP[0] = mAt[3];
                vVP[1] = mAt[4];
                vVP[2] = mAt[5];
            }
            else
            {
                vVP[0] = mAt[6];
                vVP[1] = mAt[7];
                vVP[2] = mAt[8];
            }
        }
        // std::cout<<"Cluster_size "<<cluster_result[4].size()<<endl;
        NL_estimation_driver(vVP,cluster_k,VP_result);
        cout<<"VP_result "<<VP_result->p1<<" "<<VP_result->p2<<" "<<VP_result->p3<<endl;
    }  
    else 
    {
        continue;
    }

        
    }

}
void NL_estimation_driver(double*vVp,int cluster_k,pt3 *VP_result)
{
    int NumEdg= (int)cluster_result[cluster_k].size();
    point_D* Centerpt= new point_D[NumEdg];
    pt3    *Cline = new pt3[NumEdg];
    for(int i=0;i<NumEdg;i++)
    { 
    int index= cluster_result[cluster_k][i];
    Centerpt[i]=Edg.GetCpoint(index);
    Cline[i] = Edg.GetC1Lpt(index);
    }
    double  vAngle[2];
    vAngle[0] = atan2(vVp[1], vVp[0]);
    vAngle[1] = acos(vVp[2]);
    NLVPoptimization(NumEdg,vAngle, Centerpt, Cline,VP_result);
    cout<<"VP_result "<<VP_result->p1<<" "<<VP_result->p2<<" "<<VP_result->p3<<endl;
    
    double err[3] ;
    double error;

for (int i=0;i<NumEdg;i++)
   {
     err[0]= Cline[i].p1*VP_result->p1;
     err[1]= Cline[i].p2*VP_result->p2;
     err[2]= Cline[i].p3*VP_result->p3;
    // cout<<err[0]<<" "<<err[1]<<" "<<err[2]<<endl;
     error= sqrt((err[0]*err[0])+ (err[1]*err[1])+ (err[2]*err[2]));
     cout<<"cumulative error "<<error<<endl;
   }
   
}  
    
    
    //NLVPoptimization();

