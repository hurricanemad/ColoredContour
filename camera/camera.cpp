// camera.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
void bold(const Mat &, Mat & );
void DrawCircle(const Mat , Mat , Point , int , int );
void RampCircle(const Mat&, Mat&, const Mat&, int);

int main()
{
	VideoCapture vcCameraCapture(0);
	int nFrameCount = 0;
	//int nFrameRad = 12;
	vcCameraCapture.set(CAP_PROP_FRAME_WIDTH, 1280);
	vcCameraCapture.set(CAP_PROP_FRAME_HEIGHT, 720);

	if (!vcCameraCapture.isOpened())
	{   
		cout << "No Camera Input! " << endl;
		exit(-1);
	}

	uchar nKey = '\0';

	while ('\r' != nKey)
	{   
		double dProcessTime = static_cast<double>(getTickCount());
	
		Mat matSrcImage;	//Origin input image.
		Mat matGrayImage;	//Gray image
		Mat matBlurImage;		//Image after denoice
		Mat matCannyImage;	//Canny image
		Mat matSobelImage0, matSobelImage1;	//Sobel Image
		Mat matRampImage;	//Ramp colored image

		//Mat framemake, framenew, framemakewide;
		Mat matkernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
		//Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		//int min = 100;
		//int max = 500;

		vcCameraCapture >> matSrcImage;

		if (matSrcImage.empty())
		{
			cerr << "The capture image is empty!" <<endl;
			exit(-1);
		}
		
		//vector<vector<Point>> vvptContours;
		//vector<Vec4i> frameHierarchy;

		cvtColor(matSrcImage, matGrayImage, COLOR_BGR2GRAY);

		GaussianBlur(matGrayImage, matBlurImage, Size(3, 3), 0.8, 0.8);

		//Canny(matBlurImage, matCannyImag, 30,90, 3);
		Sobel(matBlurImage, matSobelImage0, CV_32F, 1, 0, 3, 1.0, 0);
		Sobel(matBlurImage, matSobelImage1, CV_32F, 0, 1, 3, 1.0, 0);
		matCannyImage = matSobelImage0 + matSobelImage1;

		matCannyImage = abs(matCannyImage) > 45.0f;
		//morphologyEx(matCannyImage, matCannyImage, MORPH_OPEN, matkernel);

		RampCircle(matSrcImage, matRampImage, matCannyImage, nFrameCount);

		Mat matColorCannyImage;
		cvtColor(matCannyImage, matColorCannyImage, CV_GRAY2RGB);
		//matSrcImage = matSrcImage - matColorCannyImage;
		matSrcImage = 0.9 * matSrcImage +  0.3 * matRampImage;
		//matCannyImage.convertTo(matCannyImage, CV_8UC1);

//		bold(framemake, framemakewide);

		//cvtColor(framemake, framemakewide, CV_GRAY2BGR);

		//addWeighted(frame, 1.0, framemakewide, 1.0, 0.0, framenew);

	 //   //filter2D(framenew, framenew, framenew.depth(), kernel);  //锐化处理

		//if (frameRad > sqrt(framenew.cols*framenew.cols+ framenew.rows*framenew.rows)/2+4)
		//	frameRad = 12;
		//DrawCircle(framemakewide, framenew, Point(framenew.cols / 2, framenew.rows / 2), frameRad, -1);
		//frameRad = frameRad + 12;
		//dProcessTime = (static_cast<double>(getTickCount()) - dProcessTime) / getTickFrequency();

		imshow("photo", matCannyImage);
		imshow("photonew", matSrcImage);

		nKey = waitKey(1);
		//cout << dProcessTime << endl;
		//cout << "Frame Width:" << frame.cols << "\tFrame Height:" << frame.rows << endl;
		nFrameCount++;
	}

    return 0;
}

void RampCircle(const Mat& matSrcMat, Mat& matRampCircleMat, const Mat& matCannyMat, int nFrameCount)
{
	if (matSrcMat.empty())
	{
		cerr << "RampCircle function input error!" <<endl;
		exit(-1);
	}

	if (matRampCircleMat.empty())
	{
		matRampCircleMat = Mat::zeros(matSrcMat.rows, matSrcMat.cols, CV_8UC3);
	}

	Mat matTempMat = Mat::zeros(matSrcMat.rows, matSrcMat.cols, CV_8UC3);
	int nFrameRadius = sqrt(matSrcMat.rows * matSrcMat.rows + matSrcMat.cols * matSrcMat.cols);
	int nLoop = nFrameRadius / c_CircleRadius ;
	float fStep = 255.0f / c_CircleRadius;

	float fGL = static_cast<int>(nFrameCount * fStep) % 255;
	for (int n = 0, nR = 0; n < nLoop &&  nR< nFrameRadius;  nR++)
	{
		double dProcessTime = static_cast<double>(getTickCount());
		circle(matRampCircleMat, Point(matSrcMat.cols / 2, matSrcMat.rows / 2), nR, Scalar(250, 250, fGL ), 1, 1);
		dProcessTime = static_cast<double>(getTickCount() - dProcessTime) / getTickFrequency();
		fGL = static_cast<int>((fGL + fStep))% 255;

		if (nR % c_CircleRadius == c_CircleRadius - 1)
		{
			n++;
			fGL = static_cast<int>(nFrameCount * fStep) % 255;
		}
	}

	Mat matThreshCannyMat;
	threshold(matCannyMat, matThreshCannyMat, 100, 255, THRESH_BINARY_INV);
	matTempMat.copyTo(matRampCircleMat, matThreshCannyMat);
	//imshow("matRampCircleMat", matRampCircleMat);
	int i = 0;
}

//--------------------------------融合函数----------------------------------

void bold(const Mat &src, Mat & dst)
{
	 dst.create(src.size(), src.type());

	for (int j = 0; j < src.rows; j++) {
		const uchar* data1 = src.ptr<uchar>(j);
	    uchar* data2 = dst.ptr<uchar>(j);
		for (int i = 0; i < src.cols; i++) {
			if (255 ==data1[i] )
			{
				data2[i - 1] = 255;
				data2[i] = 255;
				data2[i + 1] = 255;
			}
			else
			{
				data2[i] = data1[i];
			}

		}
	}
}
void DrawCircle(const Mat src, Mat dst, Point center,int radius,int thickness)
{   
	Mat backgound;
	backgound.create(dst.rows, dst.cols, dst.type());
	circle(backgound, center, radius+100, Scalar(0,0,255), thickness,8);
	circle(backgound, center, radius+50, Scalar(255, 0, 0), thickness, 8);
	circle(backgound, center, radius, Scalar(0, 255, 0), thickness, 8);
	backgound.copyTo(dst,src);
}  