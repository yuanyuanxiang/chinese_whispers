
#pragma once

// �Ƿ�ʹ��STL vector, map ����
#define USING_STL 1

// ���Խ�������Ƿ�ʹ��STL��Ч������

// ��ע�⡿�뽫OpenCV�İ���Ŀ¼��python����Ŀ¼��ӵ���Ŀ���Ӱ���Ŀ¼
// ��python�Ŀ�Ŀ¼��ӵ���Ŀ�ĸ��ӿ�Ŀ¼
// Ȼ�����ʵ������޸ı��ļ���


//////////////////////////////////////////////////////////////////////////
// OpenCV ����
#define CV_VER "320"

#ifdef _DEBUG
#define CV_LIB_PATH "D:/opencv/opencv32/x64/lib/Debug/"
#define CV_LIB_X(LIB, VER) CV_LIB_PATH##"opencv_"##LIB##VER##"d.lib"
#else
#define CV_LIB_PATH "D:/opencv/opencv32/x64/lib/Release/"
#define CV_LIB_X(LIB, VER) CV_LIB_PATH##"opencv_"##LIB##VER##".lib"
#endif

#define USING_CV_LIB(LIB) CV_LIB_X(LIB, CV_VER) // ʹ��CV�Ŀ�


//////////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"

#pragma comment(lib, USING_CV_LIB("core"))
#pragma comment(lib, USING_CV_LIB("highgui"))
#pragma comment(lib, USING_CV_LIB("imgproc"))
#pragma comment(lib, USING_CV_LIB("video"))
#pragma comment(lib, USING_CV_LIB("videoio"))
#pragma comment(lib, USING_CV_LIB("imgcodecs"))
#pragma comment(lib, USING_CV_LIB("photo"))

#ifdef _AFX
#define OUTPUT TRACE
#else
#define OUTPUT printf
#endif


// �Ƿ�֧��IPC
#define SUPPORT_IPC 1

// �Ƿ�ʹ��⺯��Ϊstatic(�ܱ���Ƶ������map)
#define STATIC_DETECTING 1
