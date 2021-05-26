#define _USE_MATH_DEFINES

#include <opencv2/opencv.hpp>
#include <HSC/KAYACoaXpressClass.hpp>
#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <cstdio>
#include <iostream>
#include <direct.h>
#include <sys/stat.h>

#ifdef _DEBUG
#define LIB_EXT "d.lib"
#else
#define LIB_EXT ".lib"
#endif


#pragma comment(lib,"KAYACoaXpressLib" LIB_EXT)
#pragma warning(disable:4996)
using namespace std;

//スレッドの処理時間
double takepic_time = 0.001;
double showhsc_time = 0.0333;


//カメラパラメータ
const int width = 896;
const int height = 896;
const float fps = 1000.0;
const float exposuretime = 912.0;
const int offsetx = 512;
const int offsety = 92;
double map_coeff[4], stretch_mat[4], det, distort[4];

/// 画像に関するパラメータ
const int ringbuffersize = 10;
vector<cv::Mat> in_imgs, in_imgs0;
cv::Mat zero, full, zeromulti, show_img, show_img0;
int takepicid, in_imgs_saveid, takepicid0, in_imgs_saveid0;
const int multicnt = 1;
uint8_t* in_img_multi_src, *in_img_multi_src0;

//時間に関する変数
LARGE_INTEGER start, stop, freq;
LARGE_INTEGER takestart, takeend;
LARGE_INTEGER takestart0, takeend0;
LARGE_INTEGER hscstart, hscend;
double taketime = 0, taketime0 = 0, hsctime = 0;
double timer = 0;


//ログに関するパラメータ
struct Logs
{
	vector<cv::Mat> in_imgs_log;
	cv::Mat* in_imgs_log_ptr;
};

//プロトタイプ宣言
void TakePicture(kayacoaxpress* cam, kayacoaxpress* cam0, bool* flg);
void TakePicture0(kayacoaxpress* cam, bool* flg);

using namespace std;

#define GETPOINTSREALSENSE_THREAD_
#define GETRELPOSE_THREAD_
#define SHOW_PROCESSING_TIME_
#define SHOW_IMGS_THREAD_
#define SHOW_OPENGL_THREAD_
//#define DEBUG_
#define ROI_MODE_

#define SAVE_IMGS_
//#define SAVE_IMGS_HSC_
//#define SAVE_IMGS_AT_HIGHSPEED_
//#define SAVE_IMGS_REALSENSE_
#define SAVE_HSC2MK_POSE_
//#define MOVE_AXISROBOT_

int main() {
	//パラメータ
	bool flg = true;
	if (!QueryPerformanceFrequency(&freq)) { return 0; }// 単位習得

	//カメラ0の初期化
	kayacoaxpress cam0;
	cam0.connect(0);

	//カメラパラメータの設定
	cout << "Set Camera Params..." << endl;
	cam0.setParam(paramTypeKAYACoaXpress::AcquisitionMode::EnableAcquisitionFrameRate, 0); //トリガーモードで起動
	cam0.setParam(paramTypeCamera::paramInt::WIDTH, width);
	cam0.setParam(paramTypeCamera::paramInt::HEIGHT, height);
	cam0.setParam(paramTypeKAYACoaXpress::paramInt::OffsetX, offsetx);
	cam0.setParam(paramTypeKAYACoaXpress::paramInt::OffsetY, offsety);
	cam0.setParam(paramTypeCamera::paramFloat::FPS, fps);
	cam0.setParam(paramTypeKAYACoaXpress::paramFloat::ExposureTime, exposuretime);
	cam0.setParam(paramTypeKAYACoaXpress::Gain::x1);
	cam0.setParam(paramTypeKAYACoaXpress::CaptureType::BayerGRGrab);
	cam0.parameter_all_print();


	//カメラの初期化
	kayacoaxpress cam;
	cam.connect(1);

	//カメラパラメータの設定
	cout << "Set Camera Params..." << endl;
	cam.setParam(paramTypeKAYACoaXpress::AcquisitionMode::EnableAcquisitionFrameRate, 1); //トリガーモードで起動
	cam.setParam(paramTypeCamera::paramInt::WIDTH, width);
	cam.setParam(paramTypeCamera::paramInt::HEIGHT, height);
	cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetX, offsetx);
	cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetY, offsety);
	cam.setParam(paramTypeCamera::paramFloat::FPS, fps);
	cam.setParam(paramTypeKAYACoaXpress::paramFloat::ExposureTime, exposuretime);
	cam.setParam(paramTypeKAYACoaXpress::Gain::x1);
	cam.setParam(paramTypeKAYACoaXpress::CaptureType::BayerGRGrab);
	cam.parameter_all_print();

	//画像取得用のリングバッファの作成
	full = cv::Mat(cam.getParam(paramTypeCamera::paramInt::HEIGHT), cam.getParam(paramTypeCamera::paramInt::WIDTH), CV_8UC3, cv::Scalar::all(255));
	zero = cv::Mat(cam.getParam(paramTypeCamera::paramInt::HEIGHT), cam.getParam(paramTypeCamera::paramInt::WIDTH), CV_8UC3, cv::Scalar::all(0));
	zeromulti = cv::Mat(cam.getParam(paramTypeCamera::paramInt::HEIGHT) * multicnt, cam.getParam(paramTypeCamera::paramInt::WIDTH), CV_8UC3, cv::Scalar::all(0));
	cout << "Set Mat Cycle Buffer..." << endl;
	for (size_t i = 0; i < ringbuffersize; i++)
	{
		in_imgs.push_back(zeromulti.clone());
		in_imgs0.push_back(zeromulti.clone());
	}


	//
	Logs logs;


	//カメラ起動
	cout << "Camera Start!" << endl;
	cam.start();
	cam0.start();

	//スレッド作成
	thread TakePictureThread(TakePicture, &cam, &cam0, &flg);
	//thread TakePictureThread0(TakePicture0, &cam0, &flg);






	//メインループ
	cout << "Main loop start!" << endl;
	QueryPerformanceCounter(&start);

	QueryPerformanceCounter(&hscstart);
	while (hsctime < 1.5)
	{
		QueryPerformanceCounter(&hscend);
		hsctime = (double)(hscend.QuadPart - hscstart.QuadPart) / freq.QuadPart;
	}
	while (flg)
	{
		QueryPerformanceCounter(&hscstart);

		//OpenCVで画像表示
		cv::cvtColor(in_imgs[(in_imgs_saveid - 2 + ringbuffersize) % ringbuffersize], show_img, CV_RGB2BGR);
		cv::imshow("img", show_img);
		int key = cv::waitKey(1);
		if (key == 'q') flg = false;

		cv::cvtColor(in_imgs0[(in_imgs_saveid0 - 2 + ringbuffersize) % ringbuffersize], show_img0, CV_RGB2BGR);
		cv::imshow("img0", show_img0);
		int key0 = cv::waitKey(1);
		if (key0 == 'q') flg = false;

		QueryPerformanceCounter(&hscend);
		hsctime = (double)(hscend.QuadPart - hscstart.QuadPart) / freq.QuadPart;
		while (hsctime < showhsc_time)
		{
			QueryPerformanceCounter(&hscend);
			hsctime = (double)(hscend.QuadPart - hscstart.QuadPart) / freq.QuadPart;
		}

#ifdef SHOW_PROCESSING_TIME_
		std::cout << "ShowImgsHSC() time: " << hsctime << endl;
#endif // SHOW_PROCESSING_TIME_
	}

	//スレッド削除
	if (TakePictureThread.joinable())TakePictureThread.join();
	//if (TakePictureThread0.joinable())TakePictureThread0.join();



	//カメラの停止
	cam.stop();
	cam.disconnect();
	cam0.stop();
	cam0.disconnect();

	return 0;
}

//画像を格納する
void TakePicture(kayacoaxpress* cam, kayacoaxpress* cam0, bool* flg) {
	while (*flg)
	{
		QueryPerformanceCounter(&takestart);
		takepicid = in_imgs_saveid % ringbuffersize;
		in_img_multi_src = in_imgs[takepicid].ptr<uint8_t>(0);

		cam->captureFrame2(in_img_multi_src, multicnt);

		takepicid0 = in_imgs_saveid0 % ringbuffersize;
		in_img_multi_src0 = in_imgs0[takepicid0].ptr<uint8_t>(0);

		cam0->captureFrame2(in_img_multi_src0, multicnt);
		QueryPerformanceCounter(&takeend);
		taketime = (double)(takeend.QuadPart - takestart.QuadPart) / freq.QuadPart;
		while (taketime < takepic_time)
		{
			QueryPerformanceCounter(&takeend);
			taketime = (double)(takeend.QuadPart - takestart.QuadPart) / freq.QuadPart;
		}
		in_imgs_saveid = (in_imgs_saveid + 1) % ringbuffersize;
		in_imgs_saveid0 = (in_imgs_saveid0 + 1) % ringbuffersize;

#ifdef SHOW_PROCESSING_TIME_
		std::cout << "TakePicture() time: " << taketime << endl;
#endif // SHOW_PROCESSING_TIME_
	}
}

void TakePicture0(kayacoaxpress* cam, bool* flg) {
	while (*flg)
	{
		QueryPerformanceCounter(&takestart0);
		takepicid0 = in_imgs_saveid0 % ringbuffersize;
		in_img_multi_src0 = in_imgs0[takepicid0].ptr<uint8_t>(0);

		cam->captureFrame2(in_img_multi_src0, multicnt);
		QueryPerformanceCounter(&takeend0);
		taketime0 = (double)(takeend0.QuadPart - takestart0.QuadPart) / freq.QuadPart;
		while (taketime < takepic_time)
		{
			QueryPerformanceCounter(&takeend0);
			taketime0 = (double)(takeend0.QuadPart - takestart0.QuadPart) / freq.QuadPart;
		}
		in_imgs_saveid0 = (in_imgs_saveid0 + 1) % ringbuffersize;

#ifdef SHOW_PROCESSING_TIME_
		std::cout << "TakePicture() time: " << taketime << endl;
#endif // SHOW_PROCESSING_TIME_
	}
}