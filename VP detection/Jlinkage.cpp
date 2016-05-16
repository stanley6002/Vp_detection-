

//
//  Jlinkage.cpp
//  VP detection
//
//  Created by chih-hsiang chang on 1/28/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//
#include <cstdio>
#include "Jlinkage.h"
//Edge2 Edg;
//using namespace cluster_list;
//const int _col = maxima_trial;
//const int _row = (int)Edg.size();

std::vector< Cluster > vCluster;
vector<vector<int> > cluster_result; 
std::vector<Cluster> NvCluster;

void Jlinkage()
{
    
    Cluster_Initilization();
    Compute_Initial_Distance();
    int c1=0;
    int c2=0;
    //double dis= getClusterJaccardDist(vCluster[89], vCluster[132]);
    //while(true)
    //{
    for(int i=0;i<500;i++)
    {
        cout<<"iterations i "<<i<<endl;    
        double d= getClosestClusters(vCluster, c1, c2);
        //std::cout<<"distance d"<<" "<<d <<" "<<c1<<" "<<c2<<endl; 
        if (d>=1)
        {
            break;
            cout<<"merge_iteration "<<" "<<i<<endl;
        }
        //std::cout<<c1<<" "<<c2<<endl;
        mergeClusters(vCluster, c1,c2);
        //}
    }
    
    for(int j=0;j<vCluster.size();j++)
    {
        if(vCluster[j].vData.size()>0)
        {
            vector<int> cluster_Temp;
            //int *v_temp= new int[(int)vCluster[j].vData.size()];
            for (int i=0;i<(int)vCluster[j].vData.size();i++)
            {
                //v_temp[i]= vCluster[j].vData[i];
                cluster_Temp.push_back(vCluster[j].vData[i]);
                
            }
            cluster_result.push_back(cluster_Temp);
            cluster_Temp.clear();
        }        
    }   
    //    
    ////    std::cout<<cluster_result.size()<<endl;
    ////    std::cout<<cluster_result[0].size()<<endl;
    ////    std::cout<<cluster_result[1].size()<<endl;
    ////    std::cout<<cluster_result[2].size()<<endl;
    ////    std::cout<<cluster_result[3].size()<<endl;
    //
    //// free cluster space here///
}
void Cluster_Initilization()
{
    int _row  =(int)Edg.size();  // row= number of edges 
    int _col  = maxima_trial; 
    
    //    for(int i=0;i<_col;i++)
    //    {
    //        std::cout<<errorlst[0][i]<<endl;    // col= number trials
    //     }
    // Parameter_Initilization(_row);
    char buff[10];
    sprintf(buff, "%d", _col);
    char*ext =".txt";
    char filename[100]= "/matrix_print" ;
    strcat(filename,buff);
    strcat(filename,ext);
    //FILE *f = fopen("/matrix_print.txt", "w");
    FILE *f = fopen(filename, "w");
    fprintf(f, "%s ","["); 
   
    for (int i=0;i<_row;i++)
    {
        Cluster c;
        c.disable = false;
        c.vData.push_back(i);                           //cluster contains only one point
        //c.vDist = (double*) malloc(nPts*sizeof(double));
        c.vDist = new double[_row];
        c.pBin =  new bool[_col];
        //   //memcpy(c.pBin, totbin+i*nMod, nMod*sizeof(bool) ); //copy point in model space
             
        //std::cout<<endl;    
        for(int inx=0;inx<_col;inx++)
        {
             c.pBin[inx]=  errorlst[inx][i];
             fprintf(f, "%i ",(int) errorlst[inx][i]);
            //std::cout<<errorlst[inx][i]<<" ";
        }
         fprintf(f, "%s \n",";");        
        //std::cout<<errorlst[0].size()<<endl;    
        c.pLength = _col;
        vCluster.push_back(c);
    }
     fprintf(f, "%s ","]"); 
    fclose(f);
    Distance_Initilization(_row);
    ////    std::cout<<endl;
    ////    for (int i=0;i<_col;i++)
    ////    {
    ////        std::cout<<vCluster[0].pBin[i]<<endl;
    ////    
    ////    }
}
static void Compute_Initial_Distance()
{
    std::cout<<"distance"<<endl;
    for (int i=0; i< (int)vCluster.size(); i++)
        //for (int i=0; i<1; i++)
    { 
        //minimal distance info to speed up computation
        double  minDist    = DBL_MAX;
        int    minDistIdx =  INT_MAX;
        
        for (int j=i+1; j< (int)vCluster.size() ; j++)
        {
            double d = getClusterJaccardDist(vCluster[i], vCluster[j]);
            //std::cout<<j<<endl;
            vCluster[i].vDist[j] = d;
            
            if (d<minDist)
            {
                minDist = d;
                minDistIdx = j;
            }
            //std::cout<< vCluster[i].vDist[j]<<endl;
        }
        //min distance to speed up computation later
        vCluster[i].minDist    = minDist;
        vCluster[i].minDistIdx = minDistIdx;
    }
}
static double getClusterJaccardDist(Cluster c1, Cluster c2)
{
    double p=0;
    double s=0;
    
    bool* b1 = c1.pBin;
    bool* b2 = c2.pBin;
    
    for (int i = 0; i < c1.pLength; i++)
    {
        //Jlinkage
        p+= (double) (b1[i] & b2[i]);
        s+= (double) (b1[i] | b2[i]);
    }
    
    if (s>0)
        p/= s;
    else
        p = 0;
    
    return (1.0 - p);  
}
static double getClosestClusters(std::vector<Cluster> &vCluster, int &c1, int &c2)
{
    //#ifdef DEBUG
    // MPRINTF("getting closest clusters (%d)...", vCluster.size() );
    //int nbDisable =0;
    //for (int i=0; i < vCluster.size(); i++)
    //     if (vCluster[i].disable) nbDisable++;
    // MPRINTF("nb disable= %d\n", nbDisable);
    //#endif
    double minDist = DBL_MAX;
    
    
    for (int i=0; i < vCluster.size(); i++)
    {
        Cluster c = vCluster[i];
        //cout<<i<<endl;
        if (c.disable) 
        {
            continue;
        }
        
        
        if (c.minDist >= minDist) 
        {
            continue;
        } //nothing closer to that cluster, skipping check
        
        minDist = c.minDist;
        c1 = i;
        c2 = c.minDistIdx;
        //std::cout<<c1<<" "<<c2<<" "<<endl;
        //below is useless
        // 	{
        // 	  //MPRINTF("\t(%d) %lf\n", j, c.vDist[j]);
        // 	  if (c.vDist[j] < minDist)
        // 	    {
        // 	      c1 = i;
        // 	      c2 = j;
        // 	      minDist = c.vDist[j];
        // 	    }	  
        // 	}
    }
    
    assert(c1<c2);
    
    
    //#ifdef DEBUG
    //    printf("%d %d -> %lf\n", c1,c2, minDist);
    //#endif
    return minDist;
    
}

void Distance_Initilization(int & row_)
{
    for (int i=0; i< row_; i++)
    {
        for (int j=0; j< row_; j++)
        {
            vCluster[i].vDist[j] = DBL_MAX;
        }
    }
}
void intersectiontwoCluster (Cluster &c1, Cluster &c2)
{
    
    for (int j = 0; j < c1.pLength; j++)
    {
        c1.pBin[j] &= c2.pBin[j]; 
    }
    
}
void mergeClusters(std::vector<Cluster> &vCluster, int c1, int c2)
{
    //insert data of c2 into c1
    vCluster[c1].vData.insert(vCluster[c1].vData.end(), 
                              vCluster[c2].vData.begin(), 
                              vCluster[c2].vData.end()  );
    
    for(int i=0;i<(int)vCluster[c1].vData.size();i++)
    {
        cout<< vCluster[c1].vData[i]<<endl;
    }
    cout<<endl;
    
    //erase c2's data and disable this cluster
    //we don't remove from vCluster for efficiency
    vCluster[c2].vData.clear();
    vCluster[c2].disable = true;
    
    //Compute new value of c1
    intersectiontwoCluster( vCluster[c1],  vCluster[c2] );
    
    //min Dist of c1 is now invalid, recompute below
    vCluster[c1].minDist    = DBL_MAX;
    vCluster[c1].minDistIdx = INT_MAX;
    
    //recompute distance of all cluster to c1
    for (int i=0; i< vCluster.size(); i++)
    {
        //clear distance for removed (merged into) cluster
        vCluster[i].vDist[c2] = DBL_MAX;
        vCluster[c2].vDist[i] = DBL_MAX;
        
        if (i==c1) continue;
        if (vCluster[i].disable) continue;
        
        
        //distance is saved in "upper diagonal"
        int imin = std::min(i,c1);
        int imax = std::max(i,c1);
        
        //get distance
        double d = getClusterJaccardDist(vCluster[imin], vCluster[imax]);
        vCluster[imin].vDist[imax] = d;
        
        //save as min distance if it smaller than the current one
        if (d< vCluster[imin].minDist)
      	{
            vCluster[imin].minDist = d;
            vCluster[imin].minDistIdx = imax;
      	}
    }
    
    //clusters with minimal distance to c2 and c1, min dist must be reinitialized
    for (int i=0; i< vCluster.size(); i++)
    {
        if (vCluster[i].minDistIdx != c2 &&
            vCluster[i].minDistIdx != c1) continue; //nothing to do
        
        //recompute minimal distance
        vCluster[i].minDist    =  DBL_MAX;
        for (int j=i+1;j<vCluster.size(); j++)
        {
            double d = vCluster[i].vDist[j];// = d;
            if (d< vCluster[i].minDist)
            {
                vCluster[i].minDist = d;
                vCluster[i].minDistIdx = j;
            }
        }
        
    }
}
void Cluster_Initilization_N(int NumberofCandidacy,int NumbeofTrial, vector<vector<bool> > errorlst)
{
    
    int _row  = NumberofCandidacy;  // row= number of edges 
    int _col  = NumbeofTrial; 
   
    char buff[10];
    sprintf(buff, "%d", _col);
    char*ext =".txt";
    char filename[100]= "/matrix_print" ;
    strcat(filename,buff);
    strcat(filename,ext);
    //FILE *f = fopen("/matrix_print.txt", "w");
    FILE *f = fopen(filename, "w");
    fprintf(f, "%s ","["); 

    
    for (int i=0;i<_row;i++)
    {
        Cluster c;
        c.disable = false;
        c.vData.push_back(i);          //cluster contains only one point
        //c.vDist = (double*) malloc(nPts*sizeof(double));
        c.vDist = new double[_row];
        c.pBin =  new bool[_col];
        //   memcpy(c.pBin, totbin+i*nMod, nMod*sizeof(bool) ); //copy point in model space
        //   std::cout<<endl;    
        for(int inx=0;inx<_col;inx++)
        {
            c.pBin[inx]=  errorlst[inx][i];
                 fprintf(f, "%i ",(int) errorlst[inx][i]);
            //cout<<errorlst[inx][i]<<" ";
        }
          fprintf(f, "%s \n",";");        
        //cout<<endl;
        c.pLength = _col;
        NvCluster.push_back(c);
    }
    fprintf(f, "%s ","]"); 
    fclose(f);
    
    
    for (int i=0; i< _row; i++)
    {
        for (int j=0; j< _row; j++)
        {
            
            NvCluster[i].vDist[j] = DBL_MAX;
        }
    }
}
void Compute_Initial_Distance_N()
{
    for (int i=0; i< (int)NvCluster.size(); i++)
        
    { 
        double  minDist    = DBL_MAX;
        int     minDistIdx =  INT_MAX;
        
        for (int j=i+1; j< (int)NvCluster.size() ; j++)
        {
            double d = getClusterJaccardDist(NvCluster[i], NvCluster[j]);
            // std::cout<<"i= "<<i<<" j= "<<j<<" Initial_Distance_ "<<d<<endl;
            NvCluster[i].vDist[j] = d; 
            if (d<minDist)
            {
                minDist = d;
                minDistIdx = j;
            }
        }
        NvCluster[i].minDist    = minDist;
        NvCluster[i].minDistIdx = minDistIdx;
    }
}
double CalculateClusterJaccardDist_N(int in1 , int in2, vector<Cluster> NvCluster)
{
    
    Cluster c1 = NvCluster[in1];
    Cluster c2 = NvCluster[in2];
    
    double p=0;
    double s=0;
    
    bool* b1 = c1.pBin;
    bool* b2 = c2.pBin;
    
    for (int i = 0; i < c1.pLength; i++)
    {
        //Jlinkage
        p+= (double) (b1[i] & b2[i]);
        s+= (double) (b1[i] | b2[i]);
    }
    
    if (s>0)
        p/= s;
    else
        p = 0;
    
    return (1.0 - p);  
}
double getClosestClusters_N(std::vector<Cluster> &NvCluster, int &c1, int &c2)
{
    double minDist = DBL_MAX;    
    for (int i=0; i < NvCluster.size(); i++)
    {
        Cluster c = NvCluster[i];
        if (c.disable) 
        {
            continue;
        }
        if (c.minDist >= minDist) 
        {
            continue;
        } 
        minDist = c.minDist;
        c1 = i;
        c2 = c.minDistIdx;
    }
    
    assert(c1<c2);
    return minDist;
}

void mergeClusters_N(std::vector<Cluster> &NvCluster, int c1, int c2)
{
    NvCluster[c1].vData.insert(NvCluster[c1].vData.end(), 
                               NvCluster[c2].vData.begin(), 
                               NvCluster[c2].vData.end()  );
    
    //erase c2's data and disable this cluster
    //we don't remove from vCluster for efficiency
    NvCluster[c2].vData.clear();
    NvCluster[c2].disable = true;
    
    //Compute new value of c1
    intersectiontwoCluster( NvCluster[c1],  NvCluster[c2] );
    
    //min Dist of c1 is now invalid, recompute below
    NvCluster[c1].minDist    = DBL_MAX;
    NvCluster[c1].minDistIdx = INT_MAX;
    
    //recompute distance of all cluster to c1
    for (int i=0; i< NvCluster.size(); i++)
    {
        //clear distance for removed (merged into) cluster
        NvCluster[i].vDist[c2] = DBL_MAX;
        NvCluster[c2].vDist[i] = DBL_MAX;
        
        if (i==c1) continue;
        if (NvCluster[i].disable) continue;
        
        
        //distance is saved in "upper diagonal"
        int imin = std::min(i,c1);
        int imax = std::max(i,c1);
        
        //get distance
        double d = getClusterJaccardDist(NvCluster[imin], NvCluster[imax]);
        NvCluster[imin].vDist[imax] = d;
        
        //save as min distance if it smaller than the current one
        if (d< NvCluster[imin].minDist)
      	{
            NvCluster[imin].minDist = d;
            NvCluster[imin].minDistIdx = imax;
      	}
    }
    
    //clusters with minimal distance to c2 and c1, min dist must be reinitialized
    for (int i=0; i< NvCluster.size(); i++)
    {
        if (NvCluster[i].minDistIdx != c2 &&
            NvCluster[i].minDistIdx != c1) continue; //nothing to do
        
        //recompute minimal distance
        NvCluster[i].minDist    =  DBL_MAX;
        for (int j=i+1;j<NvCluster.size(); j++)
        {
            double d = NvCluster[i].vDist[j];// = d;
            if (d< NvCluster[i].minDist)
            {
                NvCluster[i].minDist = d;
                NvCluster[i].minDistIdx = j;
            }
        }
        
    }
}