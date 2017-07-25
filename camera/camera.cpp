// camera.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
void bold(const Mat &src, Mat & dst);
void DrawCircle(const Mat src, Mat dst, Point center, int radius, int thickness);

int main()
{
	VideoCapture capture(0);
	int frameRad = 12;
	//capture.set(CAP_PROP_FRAME_WIDTH, 1280);
	//capture.set(CAP_PROP_FRAME_HEIGHT, 720);

	if (!capture.isOpened())
	{   
		cout << "No Camera Input! " << endl;
		exit(-1);
	}

	int nKey = '\0';

	while ('\r' != nKey)
	{   
		double dProcessTime = static_cast<double>(getTickCount());
	
		Mat frame,framegray,framemake,framenew, framemakewide;
		Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		//int min = 100;
		//int max = 500;

		vector<vector<Point>> frameContours;
		vector<Vec4i> frameHierarchy;

        capture >> frame;
		
		cvtColor(frame, framegray, COLOR_BGR2GRAY);
		GaussianBlur(framegray, framegray, Size(7, 7), 1.3, 1.3);

		Canny(framegray, framemake, 30,90, 3);

//		bold(framemake, framemakewide);

		cvtColor(framemake, framemakewide, CV_GRAY2BGR);

		addWeighted(frame, 1.0, framemakewide, 1.0, 0.0, framenew);

	    filter2D(framenew, framenew, framenew.depth(), kernel);  //锐化处理

		if (frameRad > sqrt(framenew.cols*framenew.cols+ framenew.rows*framenew.rows)/2+4)
			frameRad = 12;
		DrawCircle(framemakewide, framenew, Point(framenew.cols / 2, framenew.rows / 2), frameRad, -1);
		frameRad = frameRad + 12;
		dProcessTime = (static_cast<double>(getTickCount()) - dProcessTime) / getTickFrequency();

		imshow("photo", frame);
		imshow("photonew", framenew);

		nKey = waitKey(1);
		cout << dProcessTime << endl;
		cout << "Frame Width:" << frame.cols << "\tFrame Height:" << frame.rows << endl;

	}

    return 0;
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