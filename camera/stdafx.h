// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <math.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>


using namespace std;
using namespace cv;

const int c_CircleRadius = 255;

#define VERSION_ID CVAUX_STR(CV_VERSION_MAJOR) CVAUX_STR(CV_VERSION_MINOR) CVAUX_STR(CV_VERSION_REVISION)

#ifdef _DEBUG
#define cvLib(name) "opencv_" name VERSION_ID "d.lib"
#else
#define cvLib(name) "opencv_" name VERSION_ID ".lib"
#endif

#pragma comment(lib, cvLib("core"))
#pragma comment(lib, cvLib("highgui"))
#pragma comment(lib, cvLib("imgproc"))
#pragma comment(lib, cvLib("imgcodecs"))
#pragma comment(lib, cvLib("video"))
#pragma comment(lib, cvLib("videoio"))