//
//  main.cpp
//  VP detection
//
//  Created by chih-hsiang chang on 1/15/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include "edge_detection .h"
#include "fit.h"
#include "Jlinkage.h"
#include "VP_linear.h"
#include <time.h>
//#include <omp.h>
# define demo
#include <map>
 int ImgWidth;
 int ImgHeight;

//using namespace std;
string convertInt(int number)
{
    stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}

int main (int argc, const char * argv[])
{
    
    //std::multimap<char,int> mymultimap;
    //mymultimap.insert ( std::pair<char,int>('a',100) );
    //mymultimap.insert ( std::pair<char,int>('a',200) );
    
    //for( map<char,int>::iterator ii=mymultimap.begin(); ii!=mymultimap.end(); ++ii)
    //{
    //    cout << (*ii).first << ": " << (*ii).second << endl;
    //}

    
     IplImage *img=cvLoadImage("/Users/chih-hsiangchang/Google Drive/ICCE/Cluster/inic_P1.jpg",1);
     ImgWidth =(int) img->width;
     ImgHeight=(int) img->height;
     Image<uint8> imgT(img);
     int DN1;
     DN1 = imgT(10,0);
    //std::vector<CvPoint> vPt;
    //std::cout<<(DN1)<<endl;
    //std::cout<<ImgWidth<<" "<<ImgHeight<<endl;
    IplImage* IGray  = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvCvtColor(img,  IGray, CV_RGB2GRAY);   
    // cvShowImage("example", IGray);
    vector<vector<CvPoint> > edgelist;
    edgelist=edge_detection(IGray);
    fit(edgelist);
    Jlinkage(); 
    linear_estimation();    
    //end = clock();
    //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    // Cluster_Initilization();
    // hoogeneous_coor(edgelist);
    // IplImage* iplDebug=cvCreateI mage(cvSize(800, 600),IPL_DEPTH_8U,1);
    Image<uint8> iplT(IGray);
    CvFont font;
    int size1_=(int) cluster_result.size();
# ifdef demo  
   
    int green_INDEX[52]={99,87,90,53,58,70,71,79,65,73,82,100,23,43,0,1,80,101,102,103,105,74,104,81,78,85,106,76,83,84,4,42,45,18,20,54,29,55,46,41,27,40,52,44,51};
    
    int blue_INDEX[38]={61,67,68,9,8,13,37,48,62,72,5,11,21,22,24,95,26,39,28,30,19,14,16,69,91,3,10,77,86,63,64,89,92,32,35,50,47,31};
    
    int red_INDEX[13]={15,97,98,33,36,34,49,66,93,96,94,56,60};
    
    for (int j=0;j<5;j++)
    {
        int index =red_INDEX[j];
        //std::cout<<"cluster"<<i<<" "<<index<<endl;               
        int edgesize=  edgelist[index].size();
        //int index1 =1; 
        CvPoint end_pt;
        end_pt.x = edgelist[index][edgesize-1].x;
        end_pt.y = edgelist[index][edgesize-1].y;
        CvPoint start_pt;
        start_pt.x = edgelist[index][0].x;
        start_pt.y = edgelist[index][0].y;
        
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, CV_AA);
        char cntr_s[1];
        char cntr_[3];
        
        sprintf(cntr_, "%d", index);
        cvPutText(img, cntr_,start_pt , &font, CV_RGB(255,255,255));
        cvLine(img,start_pt,end_pt,CV_RGB(255,0,0),4,CV_AA);
        
    }

    
     for (int j=0;j<20;j++)
    
    {
        
               int index =blue_INDEX[j];
        //std::cout<<"cluster"<<i<<" "<<index<<endl;               
        int edgesize=  edgelist[index].size();
        //int index1 =1; 
        CvPoint end_pt;
        end_pt.x = edgelist[index][edgesize-1].x;
        end_pt.y = edgelist[index][edgesize-1].y;
        CvPoint start_pt;
        start_pt.x = edgelist[index][0].x;
        start_pt.y = edgelist[index][0].y;
        
            cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, CV_AA);
            char cntr_s[1];
            char cntr_[3];
            
            sprintf(cntr_, "%d", index);
            cvPutText(img, cntr_,start_pt , &font, CV_RGB(255,255,255));
            cvLine(img,start_pt,end_pt,CV_RGB(0,0,255),4,CV_AA);
        
    }
    
    for (int j=0;j<40;j++)
        
    {
        
        int index = green_INDEX[j];
        //std::cout<<"cluster"<<i<<" "<<index<<endl;               
        int edgesize=  edgelist[index].size();
        //int index1 =1; 
        CvPoint end_pt;
        end_pt.x = edgelist[index][edgesize-1].x;
        end_pt.y = edgelist[index][edgesize-1].y;
        CvPoint start_pt;
        start_pt.x = edgelist[index][0].x;
        start_pt.y = edgelist[index][0].y;
        
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, CV_AA);
        char cntr_s[1];
        char cntr_[3];
        
        sprintf(cntr_, "%d", index);
        cvPutText(img, cntr_,start_pt , &font, CV_RGB(255,255,255));
        cvLine(img,start_pt,end_pt,CV_RGB(0,255,0),4,CV_AA);
        
    }
 
 /*  
    for (int i=0;i<size1_;i++)
    //for (int i=0;i<1;i++)
    {
        int size= (int)cluster_result[i].size();
        //  std::cout<< "cluster "<<i<<" cluster size "<<  size<<endl;
  if (size >1 )
      {
        for (int j=0;j<size;j++)
         // for (int j=0;j<3;j++)

          {
           
           //     int test[3]={42,10,15};               
               int index = cluster_result[i][j];
               
               // std::cout<<index<<",";
               //int index =test[j];
               //std::cout<<"cluster"<<i<<" "<<index<<endl;               
               int edgesize=  edgelist[index].size();
               //int index1 =1; 
               CvPoint end_pt;
               end_pt.x = edgelist[index][edgesize-1].x;
               end_pt.y = edgelist[index][edgesize-1].y;
               CvPoint start_pt;
               start_pt.x = edgelist[index][0].x;
               start_pt.y = edgelist[index][0].y;
              
               // std::cout <<endl;
               // std::cout<<  end_pt.x<<" "<<end_pt.y<<" "<<start_pt.x<<" "<<start_pt.y<<endl; 
               // int x = edgelist[j][0].x;
               // int y = edgelist[j][0].y;
               // iplT[y][x]=0;
               //}
               //std::cout<<(255*((i%4)==0))<<" "<<(255*((i%4)==1))<<" "<<(255*((i%4)==2))<< (255*((i%4)==3))<<endl;
          
          if (i<6)
            {
                //cvLine(img,start_pt,end_pt,CV_RGB(255*((i%3)==0),255*((i%3)==1),255*((i%3)==2)),1,CV_AA);
                cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, CV_AA);
                char cntr_s[1];
                char cntr_[3];
                //string i_number=convertInt(i);
                if (i==0)
                {
                    if ( index ==148 )
                    {
                    }
                    else{
                    //sprintf(cntr_s, "%d", i);
                    //cvPutText(img, cntr_s,start_pt , &font, CV_RGB(255,255,255));
                    sprintf(cntr_, "%d", index);
                    cout<<i<<":"<<index <<" ";
                    cvLine(img,start_pt,end_pt,CV_RGB(0,255,0),4,CV_AA);
                    cvPutText(img, cntr_, start_pt , &font, CV_RGB(255,255,255));
                        }
                 }
                if (i==1)
                {
                   if (index==7)
                    {
                    }
                    else{
                    sprintf(cntr_, "%d", index);
                    cvPutText(img,cntr_,start_pt , &font, CV_RGB(255,255,255));
                    cout<<i<<":"<<index <<" ";
                    //Van_pt.x=-10;
                    //Van_pt.y=-2000;
                    cvLine(img,start_pt,end_pt,CV_RGB(0,0,255),4,CV_AA);
                    cvPutText(img,cntr_,start_pt , &font, CV_RGB(255,255,255));
                    }
                }
                if (i==2)
                {
                    if ( index ==187 ||index==186)
                    {
                    }
                    else{
                     cout<<i<<":"<<index <<" ";
                     //sprintf(cntr_, "%d", index);
                     //cout<<"index_Blue "<<index<<" "<<endl;
                     cvPutText(img,cntr_,start_pt , &font, CV_RGB(255,255,255));
                     cvLine(img,start_pt,end_pt,CV_RGB(255,0,0),4,CV_AA);
                    }
                 }
                
            }
              
            if (i>=3)
            {
                 char cntr_[3];
                if(i%3==0 && i!= 0)
                {
                    sprintf(cntr_, "%d", index);
                    cvPutText(img, cntr_,start_pt , &font, CV_RGB(255,255,255));
                    cvLine(img,start_pt,end_pt,CV_RGB(255,0,0),1,CV_AA);
                   // cvLine(img,start_pt,end_pt,CV_RGB(255,255,0),1,CV_AA);
                }
                if(i%4==0 && i!= 0)
                {
                    //std::cout<<"C "<<i<<" Edge_index "<<index<<endl;
                    //cvLine(img,start_pt,end_pt,CV_RGB(0,255,0),1,CV_AA);
                    cvLine(img,start_pt,end_pt,CV_RGB(255,0,0),1,CV_AA);
                    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.3, 0.3, 0, 1, CV_AA);
                    sprintf(cntr_, "%d", index);
                    //cvPutText(img, cntr_,start_pt , &font, CV_RGB(255,255,255));
                }
                if(i%5==0 && i!= 0)
                {
                    //std::cout<<"C "<<i<<" Edge_index "<<index<<endl;
                    cvLine(img,start_pt,end_pt,CV_RGB(255,255,128),1,CV_AA);
                    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.3, 0.3, 0, 1, CV_AA);
                    sprintf(cntr_, "%d", index);
                    //cvPutText(img, cntr_,start_pt , &font, CV_RGB(255,255,255));
                }
           }
               
            //   cvLine(img,start_pt,end_pt,CV_RGB(128,128,128),1,CV_AA);
            //   cvLine(img,start_pt,end_pt,CV_RGB(255,0,0),1,CV_AA);
           //  }
       }   
        //  cout<<endl;
    }   
}*/
    
#endif
 
    
    cvSaveImage("/Users/chih-hsiangchang/Desktop/YUDB5.jpg", img);
    cvShowImage("example", img); 
    cvWaitKey(0);
    cvReleaseImage(&img);
    cvReleaseImage(&IGray);
    //  std::cout<<"test"<<endl;
 
   
}

