//
//  fit.cpp
//  VP detection
//
//  Created by chih-hsiang chang on 1/21/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "fit.h"
#include "qsort.h"
#include "Jlinkage.h"

//# define Uniform_selection
//# define New_version_Test
# define Old_version 
//#define debug1
//# define initialized(a) Edge Edges[a]; 
//using namespace std;
Edge2 Edg;

vector<vector<bool> > errorlst; 
    
void fit(std::vector<std::vector<CvPoint> >&edgelist)
{
    double sigma[3], VT[9]; 
    double U[9];
    int size = edgelist.size();
    
    cout<<"Total # of edges << "<< size<<endl;
   
    Edg.Initialize(size);    
    for (int j=0;j<size;j++)
    {
    
        int size_row= edgelist[j].size();
        double *V1=  new double[2*size_row+size_row];
        double *V1T= new double[2*size_row+size_row];
        double *V1TV1= new double[9];
        
        
    for (int i=0;i<size_row;i++)
    {
              V1[(i*3)+0]=(1.0/Focallength)*(edgelist[j][i].x-ImgWidth/2);
              V1[(i*3)+1]=(1.0/Focallength)*(edgelist[j][i].y-ImgHeight/2);
              V1[(i*3)+2]=1.0;
    } 
        // matrix_print(size,3,V1);
        matrix_transpose(size_row, 3, V1, V1T);  // INPUT Matrix dimension not output //
        matrix_product(3, size_row, size_row, 3, V1T, V1, V1TV1);
        
        //matrix_print(3, 3, V1TV1);
        int success=dgesvd_driver(3, 3, V1TV1, U, sigma, VT);
        // std::cout<<sigma[0]<<" "<<sigma[1]<<" "<<sigma[2]<<endl; 
        // matrix_print(3, 3, VT);
        double Vt[3]={VT[6],VT[7],VT[8]};    // line ~= edge
        //std::cout<<Vt[0]<<" "<<Vt[1]<<" "<<Vt[2]<<endl;
        Edg.Norm_(j,Vt); 
        Edg.VLt_(j);
        //Edg.Print_VL(j);
        //Edg.Print_VLt(j);
        //std::cout<<Edg(j,0)<<" "<<Edg(j,1)<<" "<<Edg(j,2)<<endl;
        //Edg.pointUn_Rec(j, edgelist[j].front(), edgelist[j].back());
            
        //CvPoint2D32f 
        
        //std::cout<<edgelist[j].back().x<<" "<<edgelist[j].back().y<<endl;
       
        //create normalized coordinate 
        CvPoint2D32f back ;
        CvPoint2D32f front;
        
        back.x =(1.0/500)*(edgelist[j].back().x-(ImgWidth/2));
        back.y =(1.0/500)*(edgelist[j].back().y-(ImgHeight/2));
        front.x =(1.0/500)*(edgelist[j].front().x-(ImgWidth/2));
        front.y =(1.0/500)*(edgelist[j].front().y-(ImgHeight/2));
        ////   
        // substract from center ;
        Edg.pointUn_Rec(j, back , front);
        
        delete[] V1TV1;
        delete[] V1;
        delete[] V1T;  
        
        }  
    
    int x11=8;
    int x12=5;
    double VP11[3];
    VP_pairs(x11,x12,VP11); 
    cout<<VP11[0]<<" "<<VP11[1]<<" "<<VP11[2]<<endl;
    


#ifdef New_version_Test
   
    int Maxima = maxima_trial;
    vector<vector<int> > result_cluster;
   
    bool Uniform=0;
    int *Candidacylist= new int [(int) Edg.size()];    
    vector<int> Candidacylist_V;
    CandidacySelection(edgelist,(int)Edg.size(), Candidacylist); 
    
    
    
    int NumberofCandidacy= 30;
    int NumbeofTrial= 500;
    Maxima = NumbeofTrial;
    

    for(int i=0;i<(int) Edg.size();i++)
    //    for(int i=60;i<(int) Edg.size();i++)
    {
        Candidacylist_V.push_back(Candidacylist[i]);
    }
    


    for (int itera=0;itera<2;itera++)
      {
      
        if (itera==0)
        {
            // copy Edge list//
            int *Candidacylist_Partial= new int [NumberofCandidacy];
            for (int i=0;i<Maxima;i++)
            { 
                errorlst.push_back(vector<bool>());
                for (int j=0;j<NumberofCandidacy;j++)
                {
                    errorlst[i].push_back(bool());
                }
            }   
        
        for (int i= 0;i<NumberofCandidacy;i++)
       {
          Candidacylist_Partial[i]= Candidacylist_V[i];
          //cout<<Candidacylist_Partial[i]<<endl;
        }
        

     
    errorlst= Hypothesis_Generation(edgelist,NumberofCandidacy,NumbeofTrial,Uniform,Candidacylist_Partial, NumberofCandidacy);
    
//    for (int i=0;i<NumbeofTrial;i++)
//    {
//            for (int j=0;j<NumberofCandidacy;j++)
//            {
//                cout<< errorlst[i][j]<<" ";
//            }
//    }
//            
            
            
            
//    for (int i=0;i<30;i++)
//    {
//        cout<<Candidacylist_Partial[i]<<endl;
//    }
    
      NvCluster.clear();  
      Cluster_Initilization_N(NumberofCandidacy, NumbeofTrial, errorlst);
      Compute_Initial_Distance_N();
   
      //double distance;
      int c1,c2;
      double d;
      double d1;
      for (int i=0;i<500;i++)
      {
      d  =  getClosestClusters_N(NvCluster, c1, c2);
      d1 =  CalculateClusterJaccardDist_N(c1 , c2, NvCluster);
      //cout <<"distance_ "<<d<<" "<<Candidacylist_Partial[c1]<<" "<<Candidacylist_Partial[c2]<<endl;
      if (d>=1)
       {
         break;
       }
         mergeClusters_N(NvCluster,c1,c2);
       }
    
            
            
    
//            for(int j=0;j<1;j++)
//            { 
//                int index;
//                index=NvCluster[0].vData[j];
//                cout<<Candidacylist_Partial[index]<<endl;
//            } 
        // for remove index;
    
    int shift_index=0;
    for(int j=0;j<NumberofCandidacy;j++)
         {
       vector<int> result;
        if (NvCluster[j].vData.size()>1)
       {
        //result.push_back();
        // cout<<j<<endl;
       for(int i=0;i<(int)NvCluster[j].vData.size();i++)
       {
        int index=NvCluster[j].vData[i];
        //cout<< index<<endl;
        //cout<<Candidacylist_V[index]<<endl;
        result.push_back(Candidacylist_V[index]);
        Candidacylist_V[index]=NULL;
        shift_index++;
        // if (i==4)
        //   break;
         }
           result_cluster.push_back(result);
           result.clear();
       }
    }
            
    while (shift_index >0)
    {
        for(int i=0;i<NumberofCandidacy;i++)
        {   
            if (Candidacylist_V[i]==0)
            {
                Candidacylist_V.erase(Candidacylist_V.begin()+i);
                shift_index--;
            }
        }
    }
    
        
    //cout<<"new list start"<<endl;
    //for(int i=0;i<(int)Candidacylist_V.size();i++)
    //{
    //    cout<<Candidacylist_V[i]<<endl;
    //}
   
       
    
    
    //  distance= CalculateClusterJaccardDist_N(5, 12);
    
     
    
    
        //      for(int i=0;i< Maxima;i++)
        //      {
        //      for (int j=0;j<NumberofCandidacy;j++)
        //      {
        //          cout<< errorlst[i][j]<<" ";
        //      }
        //        cout<<endl;
        //      }
        //  delete[] Candidacylist;
        delete[] Candidacylist_Partial;
        errorlst.clear();
    }
    if(itera>0)
    {
        int ClusterIndex= (int) result_cluster.size();
        //int ClusterIndex=2;
         NumberofCandidacy+= ClusterIndex;
        int *Candidacylist_Partial= new int [NumberofCandidacy];
        for (int i=0;i<Maxima;i++)
        { 
             errorlst.push_back(vector<bool>());
          for (int j=0;j<NumberofCandidacy;j++)
              {
                 errorlst[i].push_back(bool());
              }
          }   
        
        //std::cout<<Candidacylist_V.size()<<endl;
        
        for (int i= 0;i<NumberofCandidacy;i++)
        {
          
            if (i< ClusterIndex)
            {
                int replace_index = (int)result_cluster[i].size();
                Candidacylist_Partial[i]= result_cluster[i][replace_index-1];
                //Candidacylist_Partial[i]= result_cluster[i][0];
                //cout<<result_cluster[i][replace_index-1]<<endl;
            }
            else 
            {
             Candidacylist_Partial[i]= Candidacylist_V[i-ClusterIndex];   
            }
           
        }   
            for (int i= 0;i<NumberofCandidacy;i++)
            {
              cout<<"Candidacy list "<<Candidacylist_Partial[i]<<endl;
            }
        
      errorlst= Hypothesis_Generation(edgelist,NumberofCandidacy,NumbeofTrial,Uniform,Candidacylist_Partial, NumberofCandidacy);
        

        
        
        
        
      NvCluster.clear();  
      Cluster_Initilization_N(NumberofCandidacy, NumbeofTrial, errorlst);
      Compute_Initial_Distance_N();
        double d, d1;
        int c1, c2;
        for (int i=0;i<500;i++)
        {
            d =  getClosestClusters_N(NvCluster, c1, c2);
            d1=  CalculateClusterJaccardDist_N(c1 , c2, NvCluster);
            cout <<"distance_ "<<d<<" "<<Candidacylist_Partial[c1]<<" "<<Candidacylist_Partial[c2]<<endl;
            if (d>=1)
            {
                break;
            }
            mergeClusters_N(NvCluster,c1,c2);
        }
        int shift_index=0;
       
            
        
        
        for(int j=0;j<NumberofCandidacy;j++)
        { 
            vector<int> result;
            if (NvCluster[j].vData.size()>1)
            {
                //result.push_back();
                // cout<<j<<endl;
                for(int i=0;i<(int)NvCluster[j].vData.size();i++)
                {
                   
                    int index=NvCluster[j].vData[i];
                    //cout<<index<<" "<< Candidacylist_Partial[index]<<" ";
                    //cout<<index<<" "<< Candidacylist_V[index]<<" ";
                    //cout<<Candidacylist_Partial[index]<<" ";
                    if (index<ClusterIndex)
                    {
                        int _index = (int) result_cluster.size();
                        //cout<<index<<" "<<endl;
                        for(int k=0;k<_index;k++)
                        {
                            int pa_cluster_size= (int) result_cluster[k].size();
                            //if (NvCluster[j].vData[i]==result_cluster[k][0])
                            if (NvCluster[j].vData[i]==result_cluster[k][pa_cluster_size-1])
                           {
                           
                               //result.push_back(result_cluster[index][0]);
                               result.push_back(result_cluster[index][pa_cluster_size-1]);
                           }
                        }
                        //result.push_back(result_cluster[index][0]);
                        //cout<<result_cluster[index][0]<<endl;
                    }
                        
                    //result.push_back(Candidacylist_V[index]);
                    result.push_back(Candidacylist_Partial[index]);
                    //cout<<Candidacylist_V[index]<<endl;
                    
                    if (index>ClusterIndex && itera >1)
                    {
                    Candidacylist_V[index]=NULL;
                    shift_index++;
                    }
                    else 
                    {
                        Candidacylist_V[index]=NULL;
                        shift_index++;
                    
                    }
                    // if (i==4)
                    //   break;
                   }
                              // cout<<endl;
                result_cluster.push_back(result); 
                result.clear();
            }      
        }
        
      
        for(int j=0;j<(int) result_cluster.size();j++)
        {
            for(int i=0;i<(int) result_cluster[j].size();i++)
            {
                cout<<result_cluster[j][i]<<",";
            }
            cout<<endl;
        }
        
        
        
        cout<<"result_cluster.size()"<<result_cluster.size()<<endl;
        
        for(int i=0;i<result_cluster.size();i++)
        {
            for(int j=i+1; j< result_cluster.size();j++)
            {
                 int _lastIndex= (int)result_cluster[i].size(); 
                if(result_cluster[i][_lastIndex-1]==result_cluster[j][0])
                // if(result_cluster[i][0]==result_cluster[j][0])
                 {
                     // cout<<result_cluster[i][0]<<" "<<result_cluster[j][0]<<endl;
                     int _indx =(int) result_cluster[j].size();
                     for (int k =1;k<_indx;k++)
                        { 
                        
                    result_cluster[i].push_back(result_cluster[j][k]);
                     }
                   result_cluster.erase(result_cluster.begin()+j);   
                 }  
               
            }
        }
        cout<<"result_cluster.size()"<<result_cluster.size()<<endl;
               
        cout<< "Candidacylist_V.size() " <<Candidacylist_V.size()<<endl;;
        cout<<"NumberofCandidacy  "<<NumberofCandidacy<<endl;
        cout<<"shift_index "<<shift_index<<endl;
        cout<<"result_cluster.size() "<<result_cluster.size()<<endl;
        while (shift_index >0)
        {
          //  for(int i=0;i<30;i++)
        for(int i=0;i<Candidacylist_V.size();i++)
           {   
                if (Candidacylist_V[i]==0)
                {
                    Candidacylist_V.erase(Candidacylist_V.begin()+i);
                    shift_index--;
                }
               //if (shift_index==0)
               //    break;
            }
        }
        for(int i=0;i< Candidacylist_V.size();i++)
        {
        cout<<Candidacylist_V[i]<<" ";
        }
        cout<<endl;
        
        
        delete[] Candidacylist_Partial;
        errorlst.clear();
      
    
    
    }
}
    cout<<"List detail "<<endl;
    for(int j=0;j<(int) result_cluster.size();j++)
    {
    for(int i=0;i<(int) result_cluster[j].size();i++)
    {
        cout<<result_cluster[j][i]<<",";
    }
        cout<<endl;
    }
   
////     
#endif
        
# ifdef  Old_version 
 {
        # define Non_unoform
        int Maxima = maxima_trial;
        for (int i=0;i<Maxima;i++)
        { 
            errorlst.push_back(vector<bool>());
             for (int j=0;j<(int)Edg.size();j++)
            {
                errorlst[i].push_back(bool());
           }
        }
        #ifdef Non_unoform
        int  * Random_list;
        int  size_random;
//        vector<vector<CvPoint> >temp;
//        vector<int>myshufflelist;
//     for (int i=0;i<edgelist.size();i++)
//     {
//         temp.push_back(edgelist[i]);
//     }
//     for (int i=0;i<edgelist.size();i++)
//     {
//         myshufflelist.push_back(i);
//     }
//        random_shuffle(myshufflelist.begin(),myshufflelist.begin()+140);  
//    
//     
//     for (int i=0;i<edgelist.size();i++)
//     {
//         int index= myshufflelist[i];
//         //cout<<index<<" ";
//         edgelist[i]=temp[index];
//     }
     
     
        Random_list= NonUniform(edgelist,size_random);
     
     
        #endif   
        //# ifdef debug1
        //    int sw=23;
        //    std::cout<< edgelist[sw].size()<<endl;  
        //    int i=0; int amount=0; int amount1=0;
        //    while (i<=sw)
        //    {
        //        amount+=(edgelist[i].size());
        //        i++;
        //    } 
        //    i=0;
        //    while (i<sw)
        //    {
        //        amount1+=(edgelist[i].size());
        //        i++;
        //    }
        //    for(int i=amount1;i<amount;i++)
        //    {
        //        std::cout<<Random_list[i]<<endl;
        //    }
        //#endif  
                for (int i=0;i<Maxima;i++)
            
               { 
                int Indx1, Indx2;  
                int Indx_t1, Indx_t2;
                #ifdef Non_unoform
                Random_selection_Non(Indx_t1, Indx_t2, size_random);
                Indx1= Random_list[Indx_t1];
                Indx2= Random_list[Indx_t2];
                
            if (Indx1==Indx2)
            {
                i=i-1;
                continue;
            }
            //     std::cout<<Indx1<<" "<<Indx2<<endl;
              #endif
            
                          double *VP= new double[3];
                           bool err_[(int)Edg.size()];
                           bool test;
                          // VP_pairs(Indx1,Indx2,VP);
//                   Indx1=71;
//                   Indx2=50;
//                   Edg.Print_VL(Indx1);
//                   Edg.Print_VL(Indx2);
                            VP_pairs(Indx1,Indx2,VP);
                  //          std::cout<<VP[0]<<" "<<VP[1]<<" "<<VP[2]<<endl;
                           int Temp=0;
                           
                           for (int j=0;j<(int)Edg.size();j++) 
                           {
                               err_[j]  = Gen_Hyp(i,j,VP);
                               //test= Gen_Hyp(i,j,VP);
                               //if (test==true)
                                if (err_[j]==true)
                               { 
                                   Temp++;
                               }
                                   
                           }
    
            if (Temp<5)
            {
                //std::cout<<"NOT"<<" "<<i<<endl;
                i=i-1;
                continue;
            }
            else 
            {
                for (int j=0;j<(int)Edg.size();j++) 
                {
                    
                    errorlst[i][j]=err_[j];
                }
               } 
            delete [] VP;
             }
          delete [] Random_list;
        }
#endif
    //    ////        for (int j=0;j<Maxima;j++)
    //    ////    {
    //    ////        std::cout<<errorlst[j][0]<<endl;
    //    ////    }
    //    ////    std::cout<<errorlst.size()<<endl;;
    //    ////    std::cout<<errorlst[0].size()<<endl;
    //    ////    for(int i=0;i<150;i++)
    //    ////    {
    //    ////        std::cout<<errorlst[0][i]<<endl;
    //    ////    }
    //    ////    std::cout<<errorlst.size()<<endl;
    //    ////    std::cout<<Edg(3,0)<<" "<<Edg(3,1)<<" "<<Edg(3,2)<<endl;
    //    ////    //std::cout<<Edg(3,0)<<" "<<Edg(1,1)<<" "<<Edg(1,2);
    //    
    //    


    

      
};

bool Gen_Hyp(int &i, int &j, double *VP)
{    
    
    /*   
     double *error = new double[1];
     //Indx1=4; Indx2=104;
     //int temp=0;
     //std::cout<<VP[0]<<" "<<VP[1]<<" "<<VP[2]<<endl; 
     //std::cout<<j<<endl;
     V_hyperror(j, VP, error);
     //std::cout<<error[0]<<endl;
     //cout<<i<<j<<endl;
     if (abs(error[0])>error_deviation)
     {
     //std::cout<<error[0]<<endl;
     errorlst[i][j]=0;
     }
     else
     {
     //std::cout<<error[0]<<endl;
     errorlst[i][j]=1;
     temp++;
     }
     
     delete[] VP;
     */
    double *error = new double[1];
    bool re_error;
    V_hyperror(j, VP, error);
    double ErrorDeviation;
#ifdef New_version_Test
    ErrorDeviation=error_deviation1; 
#endif  
#ifdef Old_version
    ErrorDeviation=0.009; 
#endif
    if (fabs(error[0])>ErrorDeviation)
    {
        //std::cout<<error[0]<<endl;
        re_error=0;
        
        //errorlst[i][j]=0;
    }
    else
    {
        re_error=1;
        //std::cout<<error[0]<<endl;
        //errorlst[i][j]=1;
        //   temp++;
    }
    return(re_error);
}
void  V_hyperror(int &location, double *VP, double *error)
{
    double *skmat= new double [9];
    double *vL =new double [3];
    double  *vP=new double  [2];
    //int  *vP1=new int  [2];
    Edg.GetPoint(location, vP);
    Edg.GetskMat(location, skmat);
    //matrix_print(3,3,skmat);
    vL[0] = skmat[0]*VP[0] + skmat[3]*VP[1] + skmat[6]*VP[2];
    vL[1] = skmat[1]*VP[0] + skmat[4]*VP[1] + skmat[7]*VP[2];
    vL[2] = skmat[2]*VP[0] + skmat[5]*VP[1] + skmat[8]*VP[2];  
    //std::cout<<vL[0]<<" "<<vL[1]<<" "<<vL[2]<<endl;
    double nrm12 = sqrt(vL[0]*vL[0] + vL[1]*vL[1]);
    
    vL[0] = vL[0] / nrm12;
    vL[1] = vL[1] / nrm12;
    vL[2] = vL[2] / nrm12;
    //Edg.GetPoint(location, vP1);
    
    //std::cout<<VP[0]<<" "<<VP[1]<<" "<<VP[2]<<endl; 
    //std::cout<<"LINE "<<vL[0]<<" "<<vL[1]<<" "<<vL[2]<<endl;
    //std::cout<<"end_point"<<vP[0]<<"  "<<vP[1]<<endl;
    
    error[0] = fabs(vL[0]*vP[0] + vL[1]*vP[1] +  vL[2]);
    
    //double err = vL[0]*VP[0] + vL[1]*VP[1] +  vL[2]*VP[2];
    //std::cout<<" error "<<error[0]<<endl;
    //double vMaxErr = fabs_(err);
    delete [] skmat;
    delete [] vL;
    delete [] vP;
    
}
void  VP_pairs(int &Indx1, int &Indx2, double *VP)
{
    
    double *vL1= new double [3];
    double *vL2= new double [3];
    double *vVP= new double [3];
    vL1[0]=Edg(Indx1,0);vL1[1]=Edg(Indx1,1);vL1[2]=Edg(Indx1,2);
    vL2[0]=Edg(Indx2,0);vL2[1]=Edg(Indx2,1);vL2[2]=Edg(Indx2,2);
    
    vVP[0] = -vL1[2]*vL2[1] + vL1[1]*vL2[2];
    vVP[1] =  vL1[2]*vL2[0] - vL1[0]*vL2[2];
    vVP[2] = -vL1[1]*vL2[0] + vL1[0]*vL2[1];
    
    double nrm = sqrt(vVP[0]*vVP[0]+ vVP[1]*vVP[1] + vVP[2]*vVP[2]) ;
    VP[0]=vVP[0]/nrm;
    VP[1]=vVP[1]/nrm;
    VP[2]=vVP[2]/nrm;
    //int *Vpoint= new int[2];
    //std::cout<<(vL1[0]*VP[0]+vL1[1]*VP[1]+vL1[2]*VP[2])<<endl;
    //std::cout<<(vL2[0]*VP[0]+vL2[1]*VP[1]+vL2[2]*VP[2])<<endl;
    delete[]vL1;
    delete[]vL2;
    delete[]vVP;
}
void Random_selection(int &Indx1, int &Indx2, int size)
{
    //srand(time(NULL));
    //# ifdef Uniform_selection
    Indx1=0;
    Indx2=0;
    int reselect = 1;
    while  (reselect) 
    {
        Indx1 = rand() % size; 
        Indx2 = rand() % size;
        //cout<<Indx1<<" "<<Indx2<<endl;
        
        if (Indx2 != Indx1)
            break;
    }    
    //#endif
}

int *NonUniform(std::vector<std::vector<CvPoint> >&edgelist,int &size_random)
{
    int index = (int)edgelist.size();
    int size_=0;
    int i;
    for (i=0;i<index;i++)
    {
        size_+= edgelist[i].size();
    }
    //std::cout<<size_<<endl; 
    size_random= size_;
    int  * random_list = new int [size_];
    int j=0;
    int k=0;
    for (i=0;i<index;i++)
    {
        j=0;
        while (j<(int)edgelist[i].size())
        {
            //
            random_list[j+k]=i;
            j++;
        }
        k=k+j;
        //(int)edgelist[i].size();
    }
    return (random_list);
}
int *NonUniform_N(std::vector<std::vector<CvPoint> >&edgelist,int &size_random,int *Candidacylist_Partial,int NumberofCandidacy )
{
    //int index = NumberofCandidacy;
    
    int Totalsize_=0;
    int i;
    int index;
    for (i=0;i<NumberofCandidacy;i++)
    {
        index=Candidacylist_Partial[i]; 
        //cout<<"size___: "<<index<<endl;
        Totalsize_+= edgelist[index].size();
    }
    //std::cout<<size_<<endl; 
    size_random= Totalsize_;
    int  * random_list = new int [Totalsize_];
    int j=0;
    int k=0;
    for (i=0;i<NumberofCandidacy;i++)
    {
        j=0;
        index= Candidacylist_Partial[i]; 
        // cout<<"partial list "<<index<<endl;
        while (j<(int)edgelist[index].size())
        {
            //
            //random_list[j+k]=i;
            random_list[j+k]=index;
            j++;
        }
        k=k+j;
        //(int)edgelist[i].size();
    }
//    for(int i=0;i<Totalsize_;i++)
//    {
//       cout<<random_list[i]<<" ";
//    }
    
    return (random_list);
}

void Random_selection_Non(int &Indx1, int &Indx2, int &size_)
{
    
    Indx1=0;
    Indx2=0;
    int reselect = 1;
    while  (reselect) 
    {
        Indx1 = rand() % (size_); 
        Indx2 = rand() % (size_);
        //std::cout<<Indx1<<" "<<Indx2<<endl;
        //if (Indx2 != Indx1)
        if (Indx2 !=Indx1 )
            break;
    }    
}
vector<vector <bool> >Hypothesis_Generation(vector<vector<CvPoint> >&edgelist,int NumEdges /*numEdges*/ , int NumTriasl /* trials*/, bool Uniform,int *CandidacyList, int NumberofCandidacy )
{ 
    int Maxima =NumTriasl; 
    //////  errlist initialization /////
    vector<vector<bool> > errlist; 
    for (int i=0;i<NumTriasl;i++)
    { 
        errlist.push_back(vector<bool>());
        for (int j=0;j<(int)Edg.size();j++)
        {
            errlist[i].push_back(bool());
        }
    }
    
    //for(int i=0;i<NumEdges;i++)
    //{
    //   cout<< CandidacyList[i]<<" ";
    //}
    
    ///////////////////////////////////
    // std::cout<<"errlst hypo size "<<errlist.size()<<endl;
    
    if(Uniform==1)
    {
        double *VP= new double[3];
        bool err_[NumEdges];
        for (int i=0;i<Maxima;i++)
        { 
            int Indx1, Indx2;  
            int Indx_t1, Indx_t2;
            //  # ifdef  Uniform_selection        
            Random_selection(Indx1,Indx2,NumEdges); 
            
            //cout<< Indx1<<" "<<Indx2<<endl;
            VP_pairs(Indx1,Indx2,VP); 
            
            // std::cout<<VP[0]<<" "<<VP[1]<<" "<<VP[2]<<endl;
            int Temp=0;
            
            // for (int j=0;j<(int)Edg.size();j++) 
            for (int j=0;j<NumEdges;j++) 
            {
                err_[j]  = Gen_Hyp(i,j,VP);
                //test= Gen_Hyp(i,j,VP);
                //if (test==true)
                if (err_[j]==true)
                { 
                    Temp++;
                }
                
            }
            if (Temp<5)
            {
                //std::cout<<"NOT"<<" "<<i<<endl;
                i=i-1;
                continue;
            }
            else 
            {
                for (int j=0;j<NumEdges;j++) 
                {
                    
                    errlist[i][j]=err_[j];
                }
            }
        }
        delete [] VP;
        
    }
    
    else   /// non uniform //// 
    {
        
        
        
        int  * Random_list;
        int  size_random;
        //Random_list= NonUniform(edgelist,size_random);
        Random_list= NonUniform_N(edgelist,size_random, CandidacyList, NumberofCandidacy);
    
        
        for (int i=0;i<Maxima;i++)
            
        { 
            
            int Indx1, Indx2;  
            int Indx_t1, Indx_t2;
            Random_selection_Non(Indx_t1, Indx_t2, size_random);
            Indx1= Random_list[Indx_t1];
            Indx2= Random_list[Indx_t2];
            
            if (Indx1==Indx2)
            {
                i=i-1;
                continue;
            }
            // std::cout<<Indx1<<" "<<Indx2<<endl;
            double *VP= new double[3];
            bool err_[NumEdges];
            //bool test;
            Edg.Print_VL(Indx1);
            Edg.Print_VL(Indx2);
            
            VP_pairs(Indx1,Indx2,VP); 
            
            // std::cout<<VP[0]<<" "<<VP[1]<<" "<<VP[2]<<endl;
            int Temp=0;
            
            // for (int j=0;j<(int)Edg.size();j++) 
            for (int j=0;j<NumEdges;j++) 
            {
                int index_ini= CandidacyList[j];
                err_[j]= Gen_Hyp(i,index_ini,VP);
                //err_[j]  = Gen_Hyp(i,j,VP);
                //test= Gen_Hyp(i,j,VP);
                //if (test==true)
                if (err_[j]==true)
                { 
                    Temp++;
                }
                
            }
            if (Temp<5)
            {
                //std::cout<<"NOT"<<" "<<i<<endl;
                i=i-1;
                continue;
            }
            else 
            {
                for (int j=0;j<NumEdges;j++) 
                {
                    
                    errlist[i][j]=err_[j];
                }
            }
            delete [] VP;
        }
        delete [] Random_list;
    }
    
    return errlist;
}
void CandidacySelection( vector<vector<CvPoint> >&edgelist, int NumofCandidacy, int*CandidacyList ) 
{
    
    int Size_=(int)edgelist.size();
    double* SizeList= new double [Size_]; 
    std::vector<int> Candidate;
    std::vector<int> myshuffle;
    // int * SortedList= new int[Size_]; 
    for(int i=0;i<Size_;i++)
    {
        SizeList[i]= edgelist[i].size() ; 
  //      Candidate.push_back(i);
  //      CandidacyList[i]=i;
    }

 //   random_shuffle(Candidate.begin(),Candidate.end());
//    
//    for(int i=0;i<Size_;i++)
//    {
    
//        CandidacyList[i]=Candidate[i];
//    }
    //candidacy_sorting 
     qsort_perm(Size_, SizeList, CandidacyList); 
// for(int i=0;i<Size_;i++) 
// {
//    cout<< CandidacyList[i]<<endl;
//   
//}
    
    delete[] SizeList;
    //Candidate.clear();
}
