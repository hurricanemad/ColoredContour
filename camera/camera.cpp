// camera.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
void DrawCircle(const Mat , const Mat , Mat , Point , int );
void DrawBackGround(Mat);
int main()
{
	VideoCapture capture(0); // ��������ͷ
	capture.set(CAP_PROP_FRAME_WIDTH, 1280); // ��������ͷ��������С
	capture.set(CAP_PROP_FRAME_HEIGHT, 720);
	Mat Background;
	int frameRad = 12; // ȷ������Բ��ʼ�뾶
	capture >> Background;
	DrawBackGround(Background);
	if (!capture.isOpened())        // �ж�����ͷ�Ƿ���
	{   
		cout << "No Camera Input! " << endl;
		exit(-1);
	}

    int nKey = '\0'; // ѭ��ֹͣ����
	while ('\r' != nKey)
	{   
		double dProcessTime = static_cast<double>(getTickCount());
	
		Mat Frame; //����ͷ�ɼ���ͼ��
		Mat	FrameGray; // �Ҷ�ͼ��
		Mat	FrameContour; // ��Ե��ֵ��ͼ��
		Mat	FrameNew;  // ԭͼ���Եͼ��Ϻ�õ���ͼ��

        capture >> Frame; // �ɼ�һ֡ͼ��
		
		cvtColor(Frame, FrameGray, COLOR_BGR2GRAY); // ת��Ϊ�Ҷ�ͼ

		GaussianBlur(FrameGray, FrameGray, Size(7, 7), 1.3, 1.3); //��˹�˲�ȥ��

		Canny(FrameGray, FrameContour, 30,90, 3); // Canney�㷨

		cvtColor(FrameContour, FrameContour, CV_GRAY2BGR); // 1ͨ��ת��Ϊ��ͨ���Ա���

		addWeighted(Frame, 1.0, FrameContour, 1.0, 0.0, FrameNew); //��Ե��ԭͼ�ں�

		Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0); // �񻯴������
	    filter2D(FrameNew, FrameNew, FrameNew.depth(), kernel);  //�񻯴���

		if (frameRad > sqrt(FrameNew.cols*FrameNew.cols+ FrameNew.rows*FrameNew.rows)/2+4) // �жϽ���ͼ��Բ�뾶
			frameRad = 12;
		DrawCircle(FrameContour, Background, FrameNew, Point(FrameNew.cols / 2, FrameNew.rows / 2), frameRad); // ����ͼ��Բ
		frameRad = frameRad + 12; //����Բ����
		dProcessTime = (static_cast<double>(getTickCount()) - dProcessTime) / getTickFrequency();

		imshow("photo", Frame);
		imshow("photonew", FrameNew);

		nKey = waitKey(1);

		cout << dProcessTime << endl;
		cout << "Frame Width:" << Frame.cols << "\tFrame Height:" << Frame.rows << endl;

	}

    return 0;
}

//--------------------------------��Բ����----------------------------------

void DrawCircle(const Mat src, const Mat background, Mat dst, Point center,int radius)
{   
	Mat BackGroundWhite; //��ɫ����ͼ����
	Mat	DrawBackGround;  //��ɫ����ͼ����
	BackGroundWhite.create(dst.rows, dst.cols, dst.type());  // ����ͼ��ʼ��
	DrawBackGround.create(dst.rows, dst.cols, dst.type());
	DrawBackGround = Scalar::all(255);  //����ͼȫ��
	BackGroundWhite = Scalar::all(0);//����ͼȫ��
	cvtColor(BackGroundWhite, BackGroundWhite, CV_BGR2GRAY); // ������ͼ��3ͨ����Ϊ1ͨ��
	circle(BackGroundWhite, center, radius, Scalar(255), -1, 8);//�ں�ɫ����ͼ�ϻ�һ����ɫʵ��Բ
	background.copyTo(DrawBackGround, BackGroundWhite); // ������ͼ�ϰ�ɫʵ��Բ����Ľ���ɫ���Ƶ���ɫ����ͼ��
	DrawBackGround.copyTo(dst,src);//����ɫ����ͼ�ϵĽ���ɫ��Canney���ֽ�ϳɽ���ɫCanney��Ե���
}  
//--------------------------- ������ͼ����----------------------
void DrawBackGround( Mat BackGround)
{   
	double diag = sqrt(BackGround.cols*BackGround.cols + BackGround.rows*BackGround.rows) / 2; // �趨�Խ���
	int add = diag / 255 + 1; // �趨����
	int radius = add; // �趨�뾶
	for (int i = 0;radius<diag;)  
	{
		if (i != 127)
			i++;
		circle(BackGround, Point(BackGround.cols / 2, BackGround.rows / 2), radius, Scalar(255, i * 2, 0), add, 8);
		radius = radius + add; // ÿ��ѭ���뾶����
	}
}