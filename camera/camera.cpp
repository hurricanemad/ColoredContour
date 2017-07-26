// camera.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
void bold(const Mat &src, Mat & dst);
void DrawCircle(const Mat src, const Mat background, Mat dst, Point center, int radius);

int main()
{
	VideoCapture capture(0);
	Mat Backgound;
	capture >> Backgound;
	double diag = sqrt(capture.get(CAP_PROP_FRAME_WIDTH)*capture.get(CAP_PROP_FRAME_WIDTH) + capture.get(CAP_PROP_FRAME_HEIGHT)*capture.get(CAP_PROP_FRAME_HEIGHT)) / 2;
	int add = diag / 255 + 1;
	int radius = add;
	for (int i = 0;radius<diag;)
	{
		if (i != 127)
			i++;
		circle(Backgound, Point(Backgound.cols / 2, Backgound.rows / 2), radius, Scalar(255, i * 2, 0), add, 8);
		radius = radius + add;
	}
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
		DrawCircle(framemakewide, Backgound, framenew, Point(framenew.cols / 2, framenew.rows / 2), frameRad);
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
void DrawCircle(const Mat src, const Mat background, Mat dst, Point center,int radius)
{   
	Mat backGround,drawbackGround;
	backGround.create(dst.rows, dst.cols, dst.type());
	drawbackGround.create(dst.rows, dst.cols, dst.type());
	drawbackGround = Scalar::all(255);
	backGround = Scalar::all(0);
	cvtColor(backGround, backGround, CV_BGR2GRAY);
	circle(backGround, center, radius, Scalar(255), -1, 8);
	background.copyTo(drawbackGround, backGround);
	drawbackGround.copyTo(dst,src);
}  