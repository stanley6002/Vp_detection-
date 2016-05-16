//
//  edge_detection .cpp
//  VP detection
//
//  Created by chih-hsiang chang on 1/15/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//
#include <cstdio>
//# define Canny_thd 700
//# define Canny_mask 5
//# define BLACK 0
//# define WHITE 255
//# define line_threshold 10
//# define minLength 20
//# define tolerance 2.0   //  line tolerance
//# define segmentation 45
#include "edge_detection .h"
//#define debug 
using namespace std;


vector<vector<CvPoint> > edge_detection(IplImage* Iplimage)
{

    IplImage* iplEdge=cvCreateImage(cvSize(Iplimage->width, Iplimage->height),IPL_DEPTH_8U,1);
    cvCanny(Iplimage,iplEdge,Canny_thd,2*Canny_thd,Canny_mask);
    eraseBorder(iplEdge);
    eliminateJunction(iplEdge, WHITE, BLACK);
    Image<uint8> iplEdgeT(iplEdge);
    IplImage* iplEdgeCpy = cvCloneImage(iplEdge);
    std::vector< std::vector <CvPoint> > vComp;
    for (int x = 1; x < iplEdge->width-1; x++)
    {
        for (int y = 1; y < iplEdge->height-1; y++)
        {
            uint8 s = iplEdgeT[y][x]; //cvGet2D(iplEdge,p.y,p.x);
            if (s!=WHITE) continue;   //already included in a list
            std::vector<CvPoint> vPt = floodfill(cvPoint(x,y), WHITE,  BLACK, iplEdgeCpy, line_threshold);
            if (vPt.size() >= minLength)
                vComp.push_back(vPt);
        }
    }
# ifdef debug
    IplImage* iplDebug=cvCreateImage(cvSize(iplEdge->width, iplEdge->height),IPL_DEPTH_8U,1);
    Image<uint8> iplT(iplDebug);
    
    for (int x =  0; x < iplEdge->width; x++)
    {
        for (int y = 0; y < iplEdge->height; y++)
        {
            iplT[y][x]=0;
        }
    }
    
  
    for (int j=0;j<vComp.size();j++)
    {
       for (int i=0;i<vComp[j].size();i++)
            {
        int x = vComp[j][i].x;
        int y = vComp[j][i].y;
        //std::cout<<x<<" "<<y<<endl;
        iplT[y][x]=255;
           }
     }
    cvShowImage("example", iplDebug); 
    cvWaitKey(0);
#endif   
# ifdef debug 
    cvShowImage("example", iplEdge);    
#endif
    int tor=2;
    std::vector< std::vector <CvPoint> > seg;
    std::vector< std::vector <CvPoint> > seg_result;
    seg=line_segmentation(vComp, tor);
    for(int i=0;i<seg.size();i++)
    {
     if (seg[i].size()>segmentation)
    //    if (seg[i].size()>50)
     {
         seg_result.push_back(seg[i]);
     }
        //std::cout<<seg[i].size()<<endl;
    }
    
    return(seg_result);
    
    
}
void  eraseBorder(IplImage* iplEdge)
{
    //cout<<iplEdge<<endl;
    Image<unsigned char> iplEdgeT(iplEdge);
    for (int y = 0; y < iplEdge->height; y++)
    {
        iplEdgeT[y][0              ] = BLACK;
        iplEdgeT[y][iplEdge->width-1] = BLACK;
    }
    for (int x = 0; x < iplEdge->width; x++)
    {
        iplEdgeT[0               ][x] = BLACK;
        iplEdgeT[iplEdge->height-1][x] = BLACK;
    }
  
}
void eliminateJunction(IplImage* iplEdge, uint8 edgC, uint8 backC)
{
    Image<uint8> iplEdgeT(iplEdge); 
    for (int x = 1; x < iplEdge->width-1; x++)
    {
        for (int y = 1; y < iplEdge->height-1; y++)
        {	  
            uint nbEdge=0;
            
            nbEdge = ( iplEdgeT[y-1][x] +
                      iplEdgeT[y+1][x] +
                      iplEdgeT[y][x+1] +
                      iplEdgeT[y][x-1]);    
            if (nbEdge>=3*255)
            {
                iplEdgeT[y][x]   = backC;
                iplEdgeT[y-1][x] = backC;
                iplEdgeT[y+1][x] = backC;
                iplEdgeT[y][x-1] = backC;
                iplEdgeT[y][x+1] = backC;
            }
            
        }
    }
}
std::vector< CvPoint> floodfill(CvPoint seed, uint8 edgC, uint8 bckV, IplImage* img, int line_th)
{
    Image<uint8> imgT(img);
    
    std::vector<CvPoint> vPt;
    uint8 bckV2 = bckV+10;
    
    uint8 s = imgT[seed.y][seed.x];
    if (s != edgC)  //not an edge
        return vPt; 
    
    vPt.push_back(seed);
    imgT[seed.y][seed.x] = bckV2;
    
    uint i =0;
    uint lastPush=0;
    uint lastPushPrev=0;
    //pass one: this is just a naive floodfill
    while (true)
    {
        CvPoint p_curr = vPt[i];
        CvPoint p = p_curr;
        
        for (int x=-1;x<=1;x++)
        {
            for (int y=-1;y<=1;y++)
            {
                p.x=p_curr.x+x;
                p.y=p_curr.y+y;
                //printf("%d %d\n", p.x,p.y);
                //s = cvGet2D(img,p.y,p.x);
                s = imgT[p.y][p.x];
                if (s==edgC)  {
                    vPt.push_back(p);		//cvSet2D(img,p.y,p.x, bckV2);
                    imgT[p.y][p.x] = bckV2;
                    lastPushPrev = i;
                    lastPush     = vPt.size()-1;
                }
            }
        }
        i++;
        
        if (i==vPt.size())
            break;
        
    }
    if (vPt.size() < line_th)
        return vPt;
    
    CvPoint p1 = vPt[lastPush]; //this the last push on the stack
    CvPoint p2 = vPt[lastPushPrev]; //this the connected pixel to this last push
    imgT[p1.y][p1.x] = bckV;
    imgT[p2.y][p2.x] = bckV;
    vPt.clear(); //reset list 
    vPt.push_back(p1); //going reverse, this is the new beginning
    vPt.push_back(p2); //go to next and make sure we go in the right direction
    i=1;
    //pass two start from the ending: that will make sure the data are sorted
    while (true)
    {
        CvPoint p_curr = vPt[i];
        CvPoint p = p_curr;
        
        for (int x=-1;x<=1;x++)
        {
            for (int y=-1;y<=1;y++)
            {
                p.x=p_curr.x+x;
                p.y=p_curr.y+y;
                s = imgT[p.y][p.x]; // cvGet2D(img,p.y,p.x);
                if (s==bckV2)  
                {
                    vPt.push_back(p);
                    imgT[p.y][p.x] = bckV; //cvSet2D(img,p.y,p.x, bckV);
                }
            }
        }
        i++;
        
        if (i>=vPt.size())
            break;
    }
    // printf("%d \n-------------\n", vPt.size() );
    
    // for (int i=0;i<vPt.size()-1; i++)
    //   {
    //     CvPoint p1 = vPt[i];
    //     CvPoint p2 = vPt[i+1];
    //     double dist = sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
    //     //if (dist > 2)
    // 	printf("%d %d -> %f\n", i,i+1,dist);
    //   }
    return vPt;    
}
vector<vector<CvPoint> > line_segmentation(vector<vector <CvPoint> > vComp , int &tol)
{
    int Num_lines = vComp.size();
     vector<vector<CvPoint> > result;
    
    for (int i=0; i<Num_lines;i++)
     {
     vector<CvPoint> line;
     line = vComp[i];
         
//   std::cout<<i<<endl;
//        for (int i=0;i<line.size();i++)
//        {
//         std::cout<<line[i].x<<" "<<line[i].y<<endl;
//        
//        }

         int fst =0;
        int lst = vComp[i].size();
        float max=0.0; 
        int index=0;
        bool sw=0; 
       
        while (fst<lst)
        {
            bool sw1=0;
            vector<CvPoint> line_sub1;
            vector<CvPoint> line_sub2;
            for (int i=fst;i<lst;i++)
            {
                line_sub1.push_back(line[i]);
            } 
          linemaxdev(line_sub1, max, index);
             while (max > tolerance)
            {
                vector<CvPoint> line_sub;
                lst = index+fst+1;  
                //std::cout<<""<<endl;
            for (int i=fst;i<lst;i++)
                {
                    line_sub.push_back(line[i]);
                }
            //std::cout<<" "<<line_sub.size()<<endl;
            linemaxdev(line_sub, max, index); 
            if (max< 2)
            {
                for (int i=0;i<line_sub.size();i++)
                {
                 line_sub2.push_back(line_sub[i]);
                }
            }
            sw=1;
            sw1=1;
            }
         
        if (sw==1)
            {
               result.push_back(line_sub2);
               sw=0;
               fst = lst;
               lst = vComp[i].size();
            }

            if (sw1==0)
            {
        result.push_back(line_sub1);
        fst = lst;
        lst = vComp[i].size();
           }
        } 
        
    }
//    std::cout<<"result_size "<<result.size()<<endl;
//    for (int i=0;i<result.size();i++)
//    {
//        std::cout<<result[i].size()<<endl;
//    
//    }    
//    
//                for (int j=0;j<result.size();j++)
//                    {
//            for (int i=0;i<result[j].size();i++)
//            {
//                std::cout<<result[j][i].x<<" "<<result[j][i].y<<endl;
//                
//            }
//                std::cout<<" "<<endl;
//            }
//# ifdef debug
//    IplImage* iplDebug=cvCreateImage(cvSize(800, 600),IPL_DEPTH_8U,1);
//    Image<uint8> iplT(iplDebug);
//    
//    for (int x =  0; x < 800; x++)
//    {
//        for (int y = 0; y < 600; y++)
//        {
//            iplT[y][x]=0;
//        }
//    }
//    
//    
//    for (int j=0;j<result.size();j++)
//    {
//        
//        for (int i=0;i<result[j].size();i++)
//        {
//            if (result[j].size()>20)
//            {
//            int x = result[j][i].x;
//            int y = result[j][i].y;
//            //std::cout<<x<<" "<<y<<endl;
//            iplT[y][x]=255;
//        }
//        }
//    }
//    cvShowImage("example", iplDebug); 
//    cvWaitKey(0);
//#endif   


    return(result);
    
    
}

void linemaxdev(vector<CvPoint>line, float& max, int  &index)
{
    int Npts=line.size()-1;
    int Pt_size= line.size();
    if (Npts ==1)
    {
        //std::cout<<"length=1"<<endl;
        max=0;
        index=0;
        return;
    }
    float D_xy= sqrt(((line[0].x-line[Npts].x)*(line[0].x-line[Npts].x))+((line[0].y-line[Npts].y)*(line[0].y-line[Npts].y)));
    # define minimum_ -99999999
    if (D_xy>0.5)
    {
        float y1my2=(line[0].y-line[Npts].y);
        float x2mx1=(line[Npts].x-line[0].x);
        float C = (line[Npts].y*line[0].x)-(line[0].y*line[Npts].x);
        //std::cout<<"C_value "<<C<<endl;
        float *d= new float[Pt_size]; 
        float maximum =minimum_;
        int inx;
        for (int i=0;i<Npts;i++)
        {
         d[i]= abs((line[i].x*y1my2+line[i].y*x2mx1)+C)/D_xy;
        //  std::cout<<d[i]<<endl;
            if (d[i]>maximum)
            {
                maximum= d[i];
                inx =i;
            }
        }
        index = inx;
        max = maximum;
        index = inx;
    }
}




