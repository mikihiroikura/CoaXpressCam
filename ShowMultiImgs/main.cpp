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
const int multicnt = 1;
uint8_t* in_img_multi_src, *in_img_multi_src0;

//時間に関する変数
LARGE_INTEGER start, stop, freq;
LARGE_INTEGER hscstart, hscend;
double taketime = 0, taketime0 = 0, hsctime = 0;
double timer = 0;


//ログに関するパラメータ
struct Logs
{
	vector<cv::Mat> in_imgs_log;
	uint8_t* in_imgs_log_ptr;
	LARGE_INTEGER take_start, take_end;
	double taketime;
	int takepic_id, in_imgs_id;
};

//プロトタイプ宣言
void TakePicture(kayacoaxpress* cam, bool* flg, Logs *log);

using namespace std;

#define SHOW_PROCESSING_TIME_


int main() {
	//パラメータ
	bool flg = true;
	if (!QueryPerformanceFrequency(&freq)) { return 0; }// 単位習得

	//カメラ0の初期化
	kayacoaxpress cam0;
	Logs log0;
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
	Logs log;
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
		log0.in_imgs_log.push_back(zeromulti.clone());
		log.in_imgs_log.push_back(zeromulti.clone());
	}

	//Logの初期化
	log.in_imgs_id = 0;
	log.takepic_id = 0;
	log0.in_imgs_id = 0;
	log0.takepic_id = 0;


	//カメラ起動
	cout << "Camera Start!" << endl;
	cam.start();
	cam0.start();

	//スレッド作成
	thread TakePictureThread0(TakePicture, &cam0, &flg, &log0);
	thread TakePictureThread1(TakePicture, &cam, &flg, &log);

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
		cv::cvtColor(log.in_imgs_log[(log.in_imgs_id - 2 + ringbuffersize) % ringbuffersize], show_img, CV_RGB2BGR);
		cv::imshow("img", show_img);
		int key = cv::waitKey(1);
		if (key == 'q') flg = false;

		cv::cvtColor(log0.in_imgs_log[(log0.in_imgs_id - 2 + ringbuffersize) % ringbuffersize], show_img0, CV_RGB2BGR);
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
	if (TakePictureThread0.joinable())TakePictureThread0.join();
	if (TakePictureThread1.joinable())TakePictureThread1.join();



	//カメラの停止
	cam.stop();
	cam.disconnect();
	cam0.stop();
	cam0.disconnect();

	return 0;
}

void TakePicture(kayacoaxpress* cam, bool* flg, Logs *log) {
	while (*flg)
	{
		QueryPerformanceCounter(&log->take_start);
		log->takepic_id = log->in_imgs_id % ringbuffersize;
		log->in_imgs_log_ptr = log->in_imgs_log[log->takepic_id].ptr<uint8_t>(0);

		cam->captureFrame2(log->in_imgs_log_ptr, multicnt);
		QueryPerformanceCounter(&log->take_end);
		log->taketime= (double)(log->take_end.QuadPart - log->take_start.QuadPart) / freq.QuadPart;
		while (log->taketime < takepic_time)
		{
			QueryPerformanceCounter(&log->take_end);
			log->taketime = (double)(log->take_end.QuadPart - log->take_start.QuadPart) / freq.QuadPart;
		}
		log->in_imgs_id = (log->in_imgs_id + 1) % ringbuffersize;

#ifdef SHOW_PROCESSING_TIME_
		std::cout << "TakePicture() time: " << log->taketime << endl;
#endif // SHOW_PROCESSING_TIME_
	}
}