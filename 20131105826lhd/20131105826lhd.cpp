// 20131105826lhd.cpp : 定义控制台应用程序的入口点。
//1.opencv读取图片
#include "stdafx.h"
 #include <opencv2\opencv.hpp>

/* #include <iostream>
 #include <string>
 using namespace cv;
 using namespace std;
 int main()
 {
     Mat img = imread("E:\\pp");
      if(img.empty())
     {
         cout<<"error";
         return -1;

     }
     imshow("xx的靓照",img);
     waitKey();
 
     return 0;
 }*/






/*2、检测图片里的直线*/

 /*#include <cv.h>

#include <highgui.h>

#include <math.h>

 

int main(int argc, char** argv)

{

    const char* filename = argc >= 2 ? argv[1] : "77.jpg";

    IplImage* src = cvLoadImage( filename, 0 );

    IplImage* dst;

    IplImage* color_dst;

    CvMemStorage* storage = cvCreateMemStorage(0);

    CvSeq* lines = 0;

    int i;

 

    if( !src )

        return -1;

   

    dst = cvCreateImage( cvGetSize(src), 8, 1 );

    color_dst = cvCreateImage( cvGetSize(src), 8, 3 );

   

    cvCanny( src, dst, 50, 200, 3 );

    cvCvtColor( dst, color_dst, CV_GRAY2BGR );

#if 0

    lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );

 

    for( i = 0; i < MIN(lines->total,100); i++ )

    {

        float* line = (float*)cvGetSeqElem(lines,i);

        float rho = line[0];

        float theta = line[1];

        CvPoint pt1, pt2;

        double a = cos(theta), b = sin(theta);

        double x0 = a*rho, y0 = b*rho;

        pt1.x = cvRound(x0 + 1000*(-b));

        pt1.y = cvRound(y0 + 1000*(a));

        pt2.x = cvRound(x0 - 1000*(-b));

        pt2.y = cvRound(y0 - 1000*(a));

        cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, CV_AA, 0 );

    }

#else

    lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10 );

    for( i = 0; i < lines->total; i++ )

    {

        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);

        cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );

    }

#endif

    cvNamedWindow( "Source", 1 );

    cvShowImage( "Source", src );

 

    cvNamedWindow( "Hough", 1 );

    cvShowImage( "Hough", color_dst );

 

    cvWaitKey(0);

 

    return 0;

}*/


//3.利用OpenCV给图像添加标注

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

//全局变量
bool is_drawing=false;
vector<CvRect> biaozhu_boxs;
CvRect drawing_box;
IplImage *img,*img1;


static void help();
static void onMouse( int event, int x, int y, int, void* );

int _tmain(int argc, _TCHAR* argv[])
{
 CvFont font;
 CvScalar scalar;
 char text[10];

 // 初始化字体
 double hScale=1;  
 double vScale=1;    
 int lineWidth=1;// 相当于写字的线条
 scalar=CV_RGB(255,289,78);
 cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//初始化字体，准备写到图片上的 

 int frame_counter = 0;
 int obj_id = 0;

 CvCapture *capture=cvCreateFileCapture("5.png");
 img = cvQueryFrame(capture);
 img1 = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,3);
 cvCopy(img,img1);

 ofstream outfile("a.txt");
 help();

 for(vector<CvRect>::iterator it=biaozhu_boxs.begin();it!=biaozhu_boxs.end();++it)
 {
  cvRectangle(img1,cvPoint((*it).x,(*it).y),cvPoint((*it).x + (*it).width,(*it).y + (*it).height),CV_RGB(0,255,0));
 }
 cvShowImage("5",img);

 cvSetMouseCallback( "5", onMouse, 0 );

 while (1)
 {
  int c=cvWaitKey(0);
  if( (c & 255) == 27 )
  {
   cout << "Exiting ...\n";
   break;
  }

  switch((char)c)
  {
  case 'n':
   //read the next frame
   ++frame_counter;
   img = cvQueryFrame(capture);
   cvCopy(img,img1);
   if(!img){
    cout<<"\nVideo Finished!"<<endl;
    return 0;
   }

   //save all of the labeling rects
   for(vector<CvRect>::iterator it=biaozhu_boxs.begin();it!=biaozhu_boxs.end();++it)
   {
    cvRectangle(img1,cvPoint((*it).x,(*it).y),cvPoint((*it).x + (*it).width,(*it).y + (*it).height),CV_RGB(0,255,0));
    itoa(obj_id,text,10);
    cvPutText(img1,text,cvPoint((*it).x,(*it).y),&font,CV_RGB(255,255,255));//在图片中输出字符
    outfile<<frame_counter<<" "<<obj_id<<" "<<(*it).x<<" "
     <<(*it).y<<" "<<(*it).width<<" "
     <<(*it).height<<endl;
    obj_id++;
   }
   obj_id = 0;
   break;
  case 'c':
   //clear all the rects on the image
   biaozhu_boxs.clear();
   cvCopy(img,img1);
  }
  cvShowImage("5",img1);
 }

 cvNamedWindow("5",0);
 cvReleaseCapture(&capture);
 cvDestroyWindow("5");
 return 0;
}

static void help()
{
 cout << "This program designed for labeling video \n"
  <<"Coded by L. Wei on 9/4/2013\n"<<endl;

 cout<<"Use the mouse to draw rectangle on the image for labeling.\n"<<endl;

 cout << "Hot keys: \n"
  "\tESC - quit the program\n"
  "\tn - next frame of the video\n"
  "\tc - clear all the labels\n"
  <<endl;
}

static void onMouse( int event, int x, int y, int, void* )
{
 switch(event)
 {
 case CV_EVENT_LBUTTONDOWN: 
  //the left up point of the rect
  is_drawing=true;
  drawing_box.x=x;
  drawing_box.y=y;
  break;
 case CV_EVENT_MOUSEMOVE:
  //adjust the rect (use color blue for moving)
  if(is_drawing){
   drawing_box.width=x-drawing_box.x;
   drawing_box.height=y-drawing_box.y;
   cvCopy(img,img1);
   for(vector<CvRect>::iterator it=biaozhu_boxs.begin();it!=biaozhu_boxs.end();++it)
   {
    cvRectangle(img1,cvPoint((*it).x,(*it).y),cvPoint((*it).x + (*it).width,(*it).y + (*it).height),CV_RGB(0,255,0));
   }
   cvRectangle(img1,cvPoint(drawing_box.x,drawing_box.y),cvPoint(drawing_box.x+drawing_box.width,drawing_box.y+drawing_box.height),CV_RGB(255,0,0));
  }
  break;
 case CV_EVENT_LBUTTONUP:
  //finish drawing the rect (use color green for finish)
  if(is_drawing){
   drawing_box.width=x-drawing_box.x;
   drawing_box.height=y-drawing_box.y;
   cvCopy(img,img1);
   for(vector<CvRect>::iterator it=biaozhu_boxs.begin();
    it!=biaozhu_boxs.end();++it){
     cvRectangle(img1,cvPoint((*it).x,(*it).y),cvPoint((*it).x + (*it).width,(*it).y + (*it).height),CV_RGB(0,255,0));
   }
   cvRectangle(img1,cvPoint(drawing_box.x,drawing_box.y),cvPoint(drawing_box.x+drawing_box.width,drawing_box.y+drawing_box.height),CV_RGB(255,0,0));
   biaozhu_boxs.push_back(drawing_box);
  }
  is_drawing=false;
  break;
 }
 cvShowImage("吕欢达",img1);
 return;
}
