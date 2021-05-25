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

//カメラパラメータ
const int width = 896;
const int height = 896;
const float fps = 1000.0;
const float exposuretime = 912.0;
const int offsetx = 512;
const int offsety = 92;

//画像パラメータ
const int ringbuffersize = 10;
cv::Mat zero, showimg0, showimg1;

//時間に関するパラメータ
LARGE_INTEGER freq;
LARGE_INTEGER showstart, showend;
LARGE_INTEGER take_end;
double takepic_time = 0.001;
double showimg_time = 0.033;
double showtime;

struct Logs
{
	vector<cv::Mat> in_imgs_log;
	uint8_t* in_imgs_log_ptr;
	int in_imgs_id, takepicid;
	LARGE_INTEGER take_start, take_end;
	double taketime;
};

//プロトタイプ宣言
void TakePicture(kayacoaxpress* cam, bool* flg, Logs* logs);


int main() {
	//パラメータ
	bool flg = true;
	if (!QueryPerformanceFrequency(&freq)) { return 0; }// 単位習得

	//カメラ0の初期化
	kayacoaxpress cam0, cam1;
	cam0.connect(0);

	//カメラ0のパラメータ設定
	cout << "Set Camera0 Params..." << endl;
	cam0.setParam(paramTypeKAYACoaXpress::CaptureType::BayerGRGrab);
	cam0.setParam(paramTypeCamera::paramInt::WIDTH, width);
	cam0.setParam(paramTypeCamera::paramInt::HEIGHT, height);
	cam0.setParam(paramTypeKAYACoaXpress::paramInt::OffsetX, offsetx);
	cam0.setParam(paramTypeKAYACoaXpress::paramInt::OffsetY, offsety);
	cam0.setParam(paramTypeCamera::paramFloat::FPS, fps);
	cam0.setParam(paramTypeKAYACoaXpress::paramFloat::ExposureTime, exposuretime);
	cam0.setParam(paramTypeKAYACoaXpress::AcquisitionMode::EnableAcquisitionFrameRate, 0);
	cam0.setParam(paramTypeKAYACoaXpress::Gain::x1);
	cam0.parameter_all_print();

	//カメラ1の初期化
	cam1.connect(1);

	//カメラ1のパラメータ設定
	cout << "Set Camera1 Params..." << endl;
	cam1.setParam(paramTypeKAYACoaXpress::CaptureType::BayerGRGrab);
	cam1.setParam(paramTypeCamera::paramInt::WIDTH, width);
	cam1.setParam(paramTypeCamera::paramInt::HEIGHT, height);
	cam1.setParam(paramTypeKAYACoaXpress::paramInt::OffsetX, offsetx);
	cam1.setParam(paramTypeKAYACoaXpress::paramInt::OffsetY, offsety);
	cam1.setParam(paramTypeCamera::paramFloat::FPS, fps);
	cam1.setParam(paramTypeKAYACoaXpress::paramFloat::ExposureTime, exposuretime);
	cam1.setParam(paramTypeKAYACoaXpress::AcquisitionMode::EnableAcquisitionFrameRate, 0);
	cam1.setParam(paramTypeKAYACoaXpress::Gain::x1);
	cam1.parameter_all_print();

	//ログ初期化
	Logs log0, log1;
	log0.in_imgs_id = 0, log1.in_imgs_id = 0;

	//画像保存用のリングバッファの作成
	zero = cv::Mat(cam0.getParam(paramTypeCamera::paramInt::HEIGHT), cam0.getParam(paramTypeCamera::paramInt::WIDTH), CV_8UC3, cv::Scalar::all(0));
	cout << "Set Mat Ring Buffer..." << endl;
	for (size_t i = 0; i < ringbuffersize; i++)
	{
		log0.in_imgs_log.push_back(zero.clone());
		log1.in_imgs_log.push_back(zero.clone());
	}

	//画像取得スレッド作成
	thread TakePictureThread0(TakePicture, &cam0, &flg, &log0);
	//thread TakePictureThread1(TakePicture, &cam1, &flg, &log1);


	QueryPerformanceCounter(&showstart);
	while (showtime < 1.5)
	{
		QueryPerformanceCounter(&showend);
		showtime = (double)(showend.QuadPart - showstart.QuadPart) / freq.QuadPart;
	}
	//画像表示
	while (flg)
	{
		QueryPerformanceCounter(&showstart);
		cv::cvtColor(log0.in_imgs_log[(log0.in_imgs_id - 1 + ringbuffersize) % ringbuffersize], showimg0, CV_RGB2BGR);
		cv::imshow("img0",showimg0);
		int key = cv::waitKey(1);
		if (key == 'q') flg = false;
		QueryPerformanceCounter(&showend);
		showtime = (double)(showend.QuadPart - showstart.QuadPart) / freq.QuadPart;
		while (showtime < showimg_time)
		{
			QueryPerformanceCounter(&showend);
			showtime = (double)(showend.QuadPart - showstart.QuadPart) / freq.QuadPart;
		}
		std::cout << "ShowImgs() time: " << showtime << endl;
	}

	//スレッド停止
	if (TakePictureThread0.joinable()) TakePictureThread0.join();
	//if (TakePictureThread1.joinable()) TakePictureThread1.join();

	//カメラ停止
	cam0.stop();
	cam0.disconnect();
	cam1.stop();
	cam1.disconnect();

	return 0;
}

//画像を格納する
void TakePicture(kayacoaxpress* cam, bool* flg, Logs* logs) {
	while (*flg)
	{
		QueryPerformanceCounter(&logs->take_start);
		logs->takepicid = logs->in_imgs_id % ringbuffersize;
		logs->in_imgs_log_ptr = logs->in_imgs_log[logs->takepicid].ptr<uint8_t>(0);

		cam->captureFrame2(logs->in_imgs_log_ptr, 1);
		/*cam->captureFrame2(logs->in_imgs_log[logs->takepicid].data);*/
		QueryPerformanceCounter(&take_end);
		logs->taketime= (double)(take_end.QuadPart - logs->take_start.QuadPart) / freq.QuadPart;
		while (logs->taketime < takepic_time)
		{
			QueryPerformanceCounter(&take_end);
			logs->taketime= (double)(take_end.QuadPart - logs->take_start.QuadPart) / freq.QuadPart;
		}
		logs->in_imgs_id= (logs->in_imgs_id+ 1) % ringbuffersize;

		std::cout << "TakePicture() time: " << logs->taketime << endl;
	}
}