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

/*#include "stdafx.h"
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
}*/



//4.利用OpenCV在图像上画矩形，并在文档上显示矩形坐标

/*#include <opencv2/opencv.hpp>
#include <iostream> 
#include <cmath>
#include <cstring> 
using namespace std; 
void my_mouse_callback(int event,int x,int y,int flags,void* param);//回调函数CvMouseCallback,当回调函数被调用时，opencv会传入合适的值
//event为鼠标事件类型,x,y表示事件发生时鼠标位置的x,y坐标值。 
//flags的值表示了别的键是否被触发。 Param可以以任何结构方式传递的参数信息。 
CvRect box; 
bool drawing_box=false; 
void draw_box(IplImage* img,CvRect rect) 
{ 
	cvRectangle(img,cvPoint(box.x,box.y),cvPoint(box.x+box.width,box.y+box.height),cvScalar(0x00,0x00,0xff)); 
} 
CvFont font;
int main()
{  

	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,0.4,0.4,0,1,8); //字体函数，第一个0.4是高，第二个0.4是宽。0是Shear=0，字体不倾斜
	box=cvRect(-1,-1,0,0); 
	IplImage* image=cvLoadImage("3.jpg"); 
	cout<<"**"<<image->width<<" "<<image->height<<"**"<<endl;  //cvZero(image); 
	IplImage* temp=cvCloneImage(image); 
	cvNamedWindow("Box Example"); //产生事件的窗口
	cvSetMouseCallback("Box Example",my_mouse_callback,(void*)image); //my_mouse_callback是回调函数
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
		   //文字函数，可以在图像上输出一些文本。s1所指向的文本将打印到图像上。
		   //若将点的坐标打印到图像上，就需要把点的坐标变成字符串，要用到sprintf函数。 
		   //box.x,box.y是矩形的左下角的x,y轴坐标
       }   
	   break; 
 }
} 
*/

//5.利用OpenCV在图像上画矩形，并截图
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



//6.opencv运动物体检测
 #include "stdafx.h" 
 #include <stdio.h> 
 #include <time.h> 
 #include <cv.h> 
 #include <cxcore.h> 
 #include <highgui.h> 
 
 
 int main( int argc, char** argv ) 
{ 
 //声明IplImage指针 
   IplImage* pFrame = NULL;     //pFrame为视频截取的一帧 
   IplImage* pFrame1 = NULL;    //第一帧 
   IplImage* pFrame2 = NULL;    //第二帧 
   IplImage* pFrame3 = NULL;    //第三帧 
 
 
   IplImage* pFrImg = NULL;     //pFrImg为当前帧的灰度图 
   IplImage* pBkImg = NULL;     //pBkImg为当前背景的灰度图 
   IplImage* pBkImgTran = NULL;//pBkImgTran为当前背景处理过的图像 
   IplImage* pFrImgTran = NULL;//pFrImgTran为当前背景处理过的图像 
 
 
   CvMat* pFrameMat = NULL;     //pFrameMat为当前灰度矩阵 
   CvMat* pFrMat = NULL;      //pFrMat为当前前景图矩阵，当前帧减去背景图 
   CvMat* bg1 = NULL; 
   CvMat* bg2 = NULL; 
   CvMat* bg3 = NULL; 
   CvMat* pFrMatB = NULL;     //pFrMatB为二值化（0,1）的前景图 
   CvMat* pBkMat = NULL; 
  CvMat* pZeroMat = NULL;               //用于计算bg1 - bg2 的值 
   CvMat* pZeroMatB = NULL;//用于计算 pZeroMat阈值化后来判断有多少个零的临时矩阵 
 
 
   CvCapture* pCapture = NULL; 
 
 
   int warningNum = 0;      //检测到有异物入侵的次数 
   int nFrmNum = 0;//帧计数 
   int status = 0;        //状态标志位 
 
 
 //创建窗口 
   cvNamedWindow("video", 1); 
   cvNamedWindow("background",1);//背景 
   cvNamedWindow("foreground",1);//前景 
 //使窗口有序排列 
   cvMoveWindow("video", 30, 0); 
  cvMoveWindow("background", 360, 0); 
   cvMoveWindow("foreground", 690, 0); 
 
 
   if ( argc > 2 ) 
     { 
       fprintf(stderr, "Usage: bkgrd [video_file_name]\n"); 
       return -1; 
     } 
 
 
 //打开摄像头     从摄像头取出码流可以使模拟摄像头 
   if (argc ==1) 
     if ( !(pCapture = cvCaptureFromCAM(0))) 
       { 
         fprintf(stderr, "Can not open camera.\n"); 
         return -2; 
       } 
 
 
 //打开视频文件 
   if (argc == 2) 
     if ( !(pCapture = cvCaptureFromFile(argv[1]))) 
       { 
         fprintf(stderr, "Can not open video file %s\n", argv[1]); 
         return -2; 
       } 
 
 
 
 
 //开始计时 
   time_t start,end; 
   time(&start);        //time() 返回从1970年1月1号00：00：00开始以来到现在的秒数（有10为数字）。 
   printf("%d\n",start); 
 //逐帧读取视频 
   while (pFrame = cvQueryFrame( pCapture )) 
     { 
       nFrmNum++; 
       //如果是第一帧，需要申请内存，并初始化 
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
           //转化成单通道图像再处理 
           cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY); 
           //转换为矩阵 
           cvConvert(pFrImg, pBkMat); 
         } 
       else //不是第一帧的就这样处理 
         { 
           //pFrImg为当前帧的灰度图 
           cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY); 
 
 
           //pFrameMat为当前灰度矩阵 
           cvConvert(pFrImg, pFrameMat); 
 
 
           //pFrMat为前景图矩阵，当前帧减去背景图 
           cvAbsDiff(pFrameMat, pBkMat, pFrMat); 
 
 
           //pFrMatB为二值化（0,1）的前景图 
           cvThreshold(pFrMat,pFrMatB, 60, 1, CV_THRESH_BINARY); 
 
 
           //将图像矩阵转化为图像格式，用以显示 
          cvConvert(pBkMat, pBkImgTran);    
           cvConvert(pFrMat, pFrImgTran);   
 
 
           //显示图像 
           cvShowImage("video", pFrame); 
           cvShowImage("background", pBkImgTran); //显示背景 
           cvShowImage("foreground", pFrImgTran); //显示前景 
 
 
           //以上是每抓取一帧都要做的工作，下面进行危险检测 
 		   
           if (cvCountNonZero(pFrMatB) > 10000 && status == 0) //表示是第一帧的异物大于1W个像数点 
             { 
			  /* 则需要将当前帧存储为第一帧 */ 
               pFrame1 = cvCloneImage(pFrame); 
              bg1 = cvCloneMat(pFrMat); 
               status = 1;      //继续采集第2帧 
             } 
           else if (cvCountNonZero(pFrMatB) < 10000 && status == 1) // 表示第一帧的异物大于1W个像数点，而第二帧没有,则报警 
             { 
               printf("NO.%d warning!!!!\n\n",warningNum++); 
              status = 0; 
            } 
           else if (cvCountNonZero(pFrMatB) > 10000 && status == 1)// 表示第一帧和第二帧的异物都大于1W个像数点 
            { 
             pFrame2 = cvCloneImage(pFrame); 
               bg2 = cvCloneMat(pFrMat); 

 
               cvAbsDiff(bg1, bg2, pZeroMat); 
               cvThreshold(pZeroMat,pZeroMatB, 20, 1, CV_THRESH_BINARY); 
               if (cvCountNonZero(pZeroMatB) > 3000 ) //表示他们不连续，这样的话要报警 
                { 
                 printf("NO.%d warning!!!!\n\n",warningNum++); 
                   status = 0; 
                } 
               else 
                 { 
                   status = 2;                   //继续采集第3帧 
                } 
           } 
           else if (cvCountNonZero(pFrMatB) < 10000 && status == 2)//表示第一帧和第二帧的异物都大于1W个像数点,而第三帧没有 
             { 
               //报警 
               printf("NO.%d warning!!!!\n\n",warningNum++); 
               status = 0; 
             } 
          else if (cvCountNonZero(pFrMatB) > 10000 && status == 2)//表示连续3帧的异物都大于1W个像数点 
             { 
               pFrame3 = cvCloneImage(pFrame); 
               bg3 = cvCloneMat(pFrMat); 

 
              cvAbsDiff(bg2, bg3, pZeroMat); 
              cvThreshold(pZeroMat,pZeroMatB, 20, 1, CV_THRESH_BINARY); 
               if (cvCountNonZero(pZeroMatB) > 3000 ) //表示他们不连续，这样的话要报警 
                 { 
                  printf("NO.%d warning!!!!\n\n",warningNum++); 
                } 
              else //表示bg2,bg3连续 
                { 
                  cvReleaseMat(&pBkMat); 
                   pBkMat = cvCloneMat(pFrameMat); //更新背景 
               } 
                status = 0;                //进入下一次采集过程 
             } 

 
          //如果有按键事件，则跳出循环 
          //此等待也为cvShowImage函数提供时间完成显示 
          //等待时间可以根据CPU速度调整 
          if ( cvWaitKey(2) >= 0 ) 
            break; 
        }/* The End of the else */ 
    }/* The End of th while */  
 	 
 
 
 //销毁窗口 
     cvDestroyWindow("video"); 
     cvDestroyWindow("background"); 
    cvDestroyWindow("foreground"); 

 
//释放图像和矩阵 
     cvReleaseImage(&pFrImg); 
     cvReleaseImage(&pBkImg); 

 
     cvReleaseMat(&pFrameMat); 
     cvReleaseMat(&pFrMat); 
     cvReleaseMat(&pBkMat); 

 
    cvReleaseCapture(&pCapture); 

 
   return 0; 
 } 
 