//
//  Jlinkage.h
//  VP detection
//
//  Created by chih-hsiang chang on 1/28/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#ifndef __Jlinkage_h__
#define __Jlinkage_h__


# include "fit.h"
#include <cstdio>
typedef struct {
    std::vector<int>   vData;
    //std::vector<double> vDist;
    double* vDist;
    double minDist;
    int minDistIdx;
    bool*   pBin;
    int pLength;
    bool disable;
}  Cluster;

extern vector<vector<int> > cluster_result; 
extern std::vector< Cluster > vCluster;
extern std::vector< Cluster > NvCluster;
void Jlinkage();
void Cluster_Initilization();
void Distance_Initilization(int& _row);
static void Compute_Initial_Distance();
static double  getClusterJaccardDist(Cluster c1, Cluster c2);
static void Compute_Initial_Distance(int& row_);
static double getClosestClusters(std::vector<Cluster> &vCluster, int &c1, int &c2);
void intersectiontwoCluster (Cluster &c1, Cluster &c2);
void mergeClusters(std::vector<Cluster> &vCluster, int c1, int c2);
static double  CalculateClusterJaccardDist(int in1 , int in2);
static void ClusterInitialization_Ex(int*list, int NumofInitialCluster);
void Cluster_Initilization_N(int NumberofCandidacy,int NumbeofTrial, vector<vector<bool> > errorlst);
void Compute_Initial_Distance_N();
double CalculateClusterJaccardDist_N(int in1 , int in2, vector<Cluster> NvCluster);
double getClosestClusters_N(std::vector<Cluster> &vCluster, int &c1, int &c2);
void   mergeClusters_N(std::vector<Cluster> &vCluster, int c1, int c2);

#endif
