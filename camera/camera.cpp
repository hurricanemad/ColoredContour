// camera.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
void DrawCircle(const Mat , const Mat , Mat , Point , int );
void DrawBackGround(Mat);
int main()
{
	VideoCapture capture(0); // 开启摄像头
	capture.set(CAP_PROP_FRAME_WIDTH, 1280); // 设置摄像头参数、大小
	capture.set(CAP_PROP_FRAME_HEIGHT, 720);
	Mat Background;
	int frameRad = 12; // 确定渐变圆初始半径
	capture >> Background;
	DrawBackGround(Background);
	if (!capture.isOpened())        // 判断摄像头是否开启
	{   
		cout << "No Camera Input! " << endl;
		exit(-1);
	}

    int nKey = '\0'; // 循环停止条件
	while ('\r' != nKey)
	{   
		double dProcessTime = static_cast<double>(getTickCount());
	
		Mat Frame; //摄像头采集的图像
		Mat	FrameGray; // 灰度图像
		Mat	FrameContour; // 边缘二值化图像
		Mat	FrameNew;  // 原图与边缘图结合后得到的图像

        capture >> Frame; // 采集一帧图像
		
		cvtColor(Frame, FrameGray, COLOR_BGR2GRAY); // 转化为灰度图

		GaussianBlur(FrameGray, FrameGray, Size(7, 7), 1.3, 1.3); //高斯滤波去噪

		Canny(FrameGray, FrameContour, 30,90, 3); // Canney算法

		cvtColor(FrameContour, FrameContour, CV_GRAY2BGR); // 1通道转化为三通道以便结合

		addWeighted(Frame, 1.0, FrameContour, 1.0, 0.0, FrameNew); //边缘与原图融合

		Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0); // 锐化处理矩阵
	    filter2D(FrameNew, FrameNew, FrameNew.depth(), kernel);  //锐化处理

		if (frameRad > sqrt(FrameNew.cols*FrameNew.cols+ FrameNew.rows*FrameNew.rows)/2+4) // 判断渐变图的圆半径
			frameRad = 12;
		DrawCircle(FrameContour, Background, FrameNew, Point(FrameNew.cols / 2, FrameNew.rows / 2), frameRad); // 渐变图画圆
		frameRad = frameRad + 12; //渐变圆速率
		dProcessTime = (static_cast<double>(getTickCount()) - dProcessTime) / getTickFrequency();

		imshow("photo", Frame);
		imshow("photonew", FrameNew);

		nKey = waitKey(1);

		cout << dProcessTime << endl;
		cout << "Frame Width:" << Frame.cols << "\tFrame Height:" << Frame.rows << endl;

	}

    return 0;
}

//--------------------------------画圆函数----------------------------------

void DrawCircle(const Mat src, const Mat background, Mat dst, Point center,int radius)
{   
	Mat BackGroundWhite; //白色背景图定义
	Mat	DrawBackGround;  //黑色背景图定义
	BackGroundWhite.create(dst.rows, dst.cols, dst.type());  // 背景图初始化
	DrawBackGround.create(dst.rows, dst.cols, dst.type());
	DrawBackGround = Scalar::all(255);  //背景图全白
	BackGroundWhite = Scalar::all(0);//背景图全黑
	cvtColor(BackGroundWhite, BackGroundWhite, CV_BGR2GRAY); // 将背景图由3通道变为1通道
	circle(BackGroundWhite, center, radius, Scalar(255), -1, 8);//在黑色背景图上画一个白色实心圆
	background.copyTo(DrawBackGround, BackGroundWhite); // 将渐变图上白色实心圆区域的渐变色复制到白色背景图上
	DrawBackGround.copyTo(dst,src);//将白色背景图上的渐变色与Canney部分结合成渐变色Canney边缘检测
}  
//--------------------------- 画渐变图函数----------------------
void DrawBackGround( Mat BackGround)
{   
	double diag = sqrt(BackGround.cols*BackGround.cols + BackGround.rows*BackGround.rows) / 2; // 设定对角线
	int add = diag / 255 + 1; // 设定增量
	int radius = add; // 设定半径
	for (int i = 0;radius<diag;)  
	{
		if (i != 127)
			i++;
		circle(BackGround, Point(BackGround.cols / 2, BackGround.rows / 2), radius, Scalar(255, i * 2, 0), add, 8);
		radius = radius + add; // 每次循环半径增加
	}
}