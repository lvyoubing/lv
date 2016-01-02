// 20131105826lhd.cpp : �������̨Ӧ�ó������ڵ㡣
//1.opencv��ȡͼƬ
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
     imshow("xx������",img);
     waitKey();
 
     return 0;
 }*/






/*2�����ͼƬ���ֱ��*/

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


//3.����OpenCV��ͼ����ӱ�ע

/*#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

//ȫ�ֱ���
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

 // ��ʼ������
 double hScale=1;  
 double vScale=1;    
 int lineWidth=1;// �൱��д�ֵ�����
 scalar=CV_RGB(255,289,78);
 cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//��ʼ�����壬׼��д��ͼƬ�ϵ� 

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
    cvPutText(img1,text,cvPoint((*it).x,(*it).y),&font,CV_RGB(255,255,255));//��ͼƬ������ַ�
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
 cvShowImage("������",img1);
 return;
}*/



//4.����OpenCV��ͼ���ϻ����Σ������ĵ�����ʾ��������

/*#include <opencv2/opencv.hpp>
#include <iostream> 
#include <cmath>
#include <cstring> 
using namespace std; 
void my_mouse_callback(int event,int x,int y,int flags,void* param);//�ص�����CvMouseCallback,���ص�����������ʱ��opencv�ᴫ����ʵ�ֵ
//eventΪ����¼�����,x,y��ʾ�¼�����ʱ���λ�õ�x,y����ֵ�� 
//flags��ֵ��ʾ�˱�ļ��Ƿ񱻴����� Param�������κνṹ��ʽ���ݵĲ�����Ϣ�� 
CvRect box; 
bool drawing_box=false; 
void draw_box(IplImage* img,CvRect rect) 
{ 
	cvRectangle(img,cvPoint(box.x,box.y),cvPoint(box.x+box.width,box.y+box.height),cvScalar(0x00,0x00,0xff)); 
} 
CvFont font;
int main()
{  

	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,0.4,0.4,0,1,8); //���庯������һ��0.4�Ǹߣ��ڶ���0.4�ǿ�0��Shear=0�����岻��б
	box=cvRect(-1,-1,0,0); 
	IplImage* image=cvLoadImage("3.jpg"); 
	cout<<"**"<<image->width<<" "<<image->height<<"**"<<endl;  //cvZero(image); 
	IplImage* temp=cvCloneImage(image); 
	cvNamedWindow("Box Example"); //�����¼��Ĵ���
	cvSetMouseCallback("Box Example",my_mouse_callback,(void*)image); //my_mouse_callback�ǻص�����
	while(1) 
	{   
		cvCopy(image,temp); 
        if(drawing_box) draw_box(temp,box);  
		cvShowImage("Box Example",temp);   
		if(cvWaitKey(30)==27) break; 
    }   
    cvSaveImage("temp.jpg",temp);  
	cvReleaseImage(&image);  
	cvReleaseImage(&temp); 
    cvDestroyWindow("Box Example");  
	return 0; 
 } 
void my_mouse_callback(int event,int x,int y,int flags,void* param)
{  
	IplImage* image=(IplImage*)param;
	switch(event)  
	{ 
    case CV_EVENT_MOUSEMOVE:  
		{   
			if(drawing_box)   
			{    
				box.width=x-box.x;  
				box.height=y-box.y; 
            } 
        }  
		break; 
	case CV_EVENT_LBUTTONDOWN: 
        { 
             drawing_box=true;    
		     box=cvRect(x,y,0,0); 
             cout<<box.x<<" "<<box.y<<endl; 
        } 
        break;  
   case CV_EVENT_LBUTTONUP: 
       {   
		   drawing_box=false;  
		   draw_box(image,box); 
           cout<<box.x<<" "<<box.y<<"&&"<<box.width<<" "<<box.height<<endl;   
		   char s1[32];  
		   char s2[32];     
           sprintf(s1,"(%d,%d)",box.x,box.y);    
		   sprintf(s2,"%d",box.y); 
           cvPutText(image,s1,cvPoint(box.x,box.y),&font,CV_RGB(0,0,255));  
		   //���ֺ�����������ͼ�������һЩ�ı���s1��ָ����ı�����ӡ��ͼ���ϡ�
		   //������������ӡ��ͼ���ϣ�����Ҫ�ѵ���������ַ�����Ҫ�õ�sprintf������ 
		   //box.x,box.y�Ǿ��ε����½ǵ�x,y������
       }   
	   break; 
 }
} 
*/

//5.����OpenCV��ͼ���ϻ����Σ�����ͼ
/*#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

// global variable
static Mat g_img_src;
static Mat g_img_dst;
static Mat g_img_sub;
static bool g_is_rect_inited = false;
static Point g_rect_tl;
static string g_window_name = "image";

static void onMouse( int event, int x, int y, int, void* )
{   
    if(CV_EVENT_LBUTTONDOWN == event){
        g_is_rect_inited = true;
        g_rect_tl = Point(x, y);    
    }
    else if (CV_EVENT_MOUSEMOVE == event && g_is_rect_inited){      
        g_img_src.copyTo(g_img_dst);
        rectangle(g_img_dst, g_rect_tl, Point(x,y), Scalar_<uchar>::all(200), 3, 8);
        imshow(g_window_name, g_img_dst);   
    }
    else if (CV_EVENT_LBUTTONUP == event && g_rect_tl != Point(x,y)){   
        g_img_src(Rect(g_rect_tl, Point(x,y))).copyTo(g_img_sub);
        imshow("sub image", g_img_sub);
        g_is_rect_inited = false;
    }
}

int main(int argc, char** argv){
    string filename = argc >= 2 ? argv[1] : "lv.png";
    g_img_src = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    if (g_img_src.empty()){
        cerr << "[ERROR] : please check your image file name." << endl;
        return EXIT_FAILURE;
    }
    namedWindow(g_window_name, CV_WINDOW_KEEPRATIO);
    setMouseCallback(g_window_name, onMouse, 0);

    while(true){
        imshow(g_window_name, g_img_src);
        int c = waitKey(0);
        if( (c & 255) == 27 ){ // Esc
            destroyAllWindows();
            cout << "Exiting ...\n";
            break;
        }
    }
    return EXIT_SUCCESS;
}*/



//6.opencv�˶�������
 #include "stdafx.h" 
 #include <stdio.h> 
 #include <time.h> 
 #include <cv.h> 
 #include <cxcore.h> 
 #include <highgui.h> 
 
 
 int main( int argc, char** argv ) 
{ 
 //����IplImageָ�� 
   IplImage* pFrame = NULL;     //pFrameΪ��Ƶ��ȡ��һ֡ 
   IplImage* pFrame1 = NULL;    //��һ֡ 
   IplImage* pFrame2 = NULL;    //�ڶ�֡ 
   IplImage* pFrame3 = NULL;    //����֡ 
 
 
   IplImage* pFrImg = NULL;     //pFrImgΪ��ǰ֡�ĻҶ�ͼ 
   IplImage* pBkImg = NULL;     //pBkImgΪ��ǰ�����ĻҶ�ͼ 
   IplImage* pBkImgTran = NULL;//pBkImgTranΪ��ǰ�����������ͼ�� 
   IplImage* pFrImgTran = NULL;//pFrImgTranΪ��ǰ�����������ͼ�� 
 
 
   CvMat* pFrameMat = NULL;     //pFrameMatΪ��ǰ�ҶȾ��� 
   CvMat* pFrMat = NULL;      //pFrMatΪ��ǰǰ��ͼ���󣬵�ǰ֡��ȥ����ͼ 
   CvMat* bg1 = NULL; 
   CvMat* bg2 = NULL; 
   CvMat* bg3 = NULL; 
   CvMat* pFrMatB = NULL;     //pFrMatBΪ��ֵ����0,1����ǰ��ͼ 
   CvMat* pBkMat = NULL; 
  CvMat* pZeroMat = NULL;               //���ڼ���bg1 - bg2 ��ֵ 
   CvMat* pZeroMatB = NULL;//���ڼ��� pZeroMat��ֵ�������ж��ж��ٸ������ʱ���� 
 
 
   CvCapture* pCapture = NULL; 
 
 
   int warningNum = 0;      //��⵽���������ֵĴ��� 
   int nFrmNum = 0;//֡���� 
   int status = 0;        //״̬��־λ 
 
 
 //�������� 
   cvNamedWindow("video", 1); 
   cvNamedWindow("background",1);//���� 
   cvNamedWindow("foreground",1);//ǰ�� 
 //ʹ������������ 
   cvMoveWindow("video", 30, 0); 
  cvMoveWindow("background", 360, 0); 
   cvMoveWindow("foreground", 690, 0); 
 
 
   if ( argc > 2 ) 
     { 
       fprintf(stderr, "Usage: bkgrd [video_file_name]\n"); 
       return -1; 
     } 
 
 
 //������ͷ     ������ͷȡ����������ʹģ������ͷ 
   if (argc ==1) 
     if ( !(pCapture = cvCaptureFromCAM(0))) 
       { 
         fprintf(stderr, "Can not open camera.\n"); 
         return -2; 
       } 
 
 
 //����Ƶ�ļ� 
   if (argc == 2) 
     if ( !(pCapture = cvCaptureFromFile(argv[1]))) 
       { 
         fprintf(stderr, "Can not open video file %s\n", argv[1]); 
         return -2; 
       } 
 
 
 
 
 //��ʼ��ʱ 
   time_t start,end; 
   time(&start);        //time() ���ش�1970��1��1��00��00��00��ʼ���������ڵ���������10Ϊ���֣��� 
   printf("%d\n",start); 
 //��֡��ȡ��Ƶ 
   while (pFrame = cvQueryFrame( pCapture )) 
     { 
       nFrmNum++; 
       //����ǵ�һ֡����Ҫ�����ڴ棬����ʼ�� 
       if (nFrmNum == 1) 
         { 
 
 
           pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U,1); 
           pFrImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U,1); 
           pBkImgTran = cvCreateImage(cvSize(pFrame->width,pFrame->height), IPL_DEPTH_8U,1); 
           pFrImgTran = cvCreateImage(cvSize(pFrame->width,pFrame->height), IPL_DEPTH_8U,1); 
 
 
           pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1); 
           pZeroMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1); 
           pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1); 
           pFrMatB = cvCreateMat(pFrame->height, pFrame->width, CV_8UC1); 
           pZeroMatB = cvCreateMat(pFrame->height, pFrame->width, CV_8UC1); 
           pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1); 
           cvZero(pZeroMat); 
           //ת���ɵ�ͨ��ͼ���ٴ��� 
           cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY); 
           //ת��Ϊ���� 
           cvConvert(pFrImg, pBkMat); 
         } 
       else //���ǵ�һ֡�ľ��������� 
         { 
           //pFrImgΪ��ǰ֡�ĻҶ�ͼ 
           cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY); 
 
 
           //pFrameMatΪ��ǰ�ҶȾ��� 
           cvConvert(pFrImg, pFrameMat); 
 
 
           //pFrMatΪǰ��ͼ���󣬵�ǰ֡��ȥ����ͼ 
           cvAbsDiff(pFrameMat, pBkMat, pFrMat); 
 
 
           //pFrMatBΪ��ֵ����0,1����ǰ��ͼ 
           cvThreshold(pFrMat,pFrMatB, 60, 1, CV_THRESH_BINARY); 
 
 
           //��ͼ�����ת��Ϊͼ���ʽ��������ʾ 
          cvConvert(pBkMat, pBkImgTran);    
           cvConvert(pFrMat, pFrImgTran);   
 
 
           //��ʾͼ�� 
           cvShowImage("video", pFrame); 
           cvShowImage("background", pBkImgTran); //��ʾ���� 
           cvShowImage("foreground", pFrImgTran); //��ʾǰ�� 
 
 
           //������ÿץȡһ֡��Ҫ���Ĺ������������Σ�ռ�� 
 		   
           if (cvCountNonZero(pFrMatB) > 10000 && status == 0) //��ʾ�ǵ�һ֡���������1W�������� 
             { 
			  /* ����Ҫ����ǰ֡�洢Ϊ��һ֡ */ 
               pFrame1 = cvCloneImage(pFrame); 
              bg1 = cvCloneMat(pFrMat); 
               status = 1;      //�����ɼ���2֡ 
             } 
           else if (cvCountNonZero(pFrMatB) < 10000 && status == 1) // ��ʾ��һ֡���������1W�������㣬���ڶ�֡û��,�򱨾� 
             { 
               printf("NO.%d warning!!!!\n\n",warningNum++); 
              status = 0; 
            } 
           else if (cvCountNonZero(pFrMatB) > 10000 && status == 1)// ��ʾ��һ֡�͵ڶ�֡�����ﶼ����1W�������� 
            { 
             pFrame2 = cvCloneImage(pFrame); 
               bg2 = cvCloneMat(pFrMat); 

 
               cvAbsDiff(bg1, bg2, pZeroMat); 
               cvThreshold(pZeroMat,pZeroMatB, 20, 1, CV_THRESH_BINARY); 
               if (cvCountNonZero(pZeroMatB) > 3000 ) //��ʾ���ǲ������������Ļ�Ҫ���� 
                { 
                 printf("NO.%d warning!!!!\n\n",warningNum++); 
                   status = 0; 
                } 
               else 
                 { 
                   status = 2;                   //�����ɼ���3֡ 
                } 
           } 
           else if (cvCountNonZero(pFrMatB) < 10000 && status == 2)//��ʾ��һ֡�͵ڶ�֡�����ﶼ����1W��������,������֡û�� 
             { 
               //���� 
               printf("NO.%d warning!!!!\n\n",warningNum++); 
               status = 0; 
             } 
          else if (cvCountNonZero(pFrMatB) > 10000 && status == 2)//��ʾ����3֡�����ﶼ����1W�������� 
             { 
               pFrame3 = cvCloneImage(pFrame); 
               bg3 = cvCloneMat(pFrMat); 

 
              cvAbsDiff(bg2, bg3, pZeroMat); 
              cvThreshold(pZeroMat,pZeroMatB, 20, 1, CV_THRESH_BINARY); 
               if (cvCountNonZero(pZeroMatB) > 3000 ) //��ʾ���ǲ������������Ļ�Ҫ���� 
                 { 
                  printf("NO.%d warning!!!!\n\n",warningNum++); 
                } 
              else //��ʾbg2,bg3���� 
                { 
                  cvReleaseMat(&pBkMat); 
                   pBkMat = cvCloneMat(pFrameMat); //���±��� 
               } 
                status = 0;                //������һ�βɼ����� 
             } 

 
          //����а����¼���������ѭ�� 
          //�˵ȴ�ҲΪcvShowImage�����ṩʱ�������ʾ 
          //�ȴ�ʱ����Ը���CPU�ٶȵ��� 
          if ( cvWaitKey(2) >= 0 ) 
            break; 
        }/* The End of the else */ 
    }/* The End of th while */  
 	 
 
 
 //���ٴ��� 
     cvDestroyWindow("video"); 
     cvDestroyWindow("background"); 
    cvDestroyWindow("foreground"); 

 
//�ͷ�ͼ��;��� 
     cvReleaseImage(&pFrImg); 
     cvReleaseImage(&pBkImg); 

 
     cvReleaseMat(&pFrameMat); 
     cvReleaseMat(&pFrMat); 
     cvReleaseMat(&pBkMat); 

 
    cvReleaseCapture(&pCapture); 

 
   return 0; 
 } 
 