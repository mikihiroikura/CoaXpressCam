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

//�X���b�h�̏�������
double takepic_time = 0.001;
double showhsc_time = 0.0333;


//�J�����p�����[�^
const int width = 896;
const int height = 896;
const float fps = 1000.0;
const float exposuretime = 912.0;
const int offsetx = 512;
const int offsety = 92;
double map_coeff[4], stretch_mat[4], det, distort[4];

/// �摜�Ɋւ���p�����[�^
const int ringbuffersize = 10;
vector<cv::Mat> in_imgs, in_imgs0;
cv::Mat zero, full, zeromulti, show_img, show_img0;
int takepicid, in_imgs_saveid, takepicid0, in_imgs_saveid0;
const int multicnt = 1;
uint8_t* in_img_multi_src, *in_img_multi_src0;

//���ԂɊւ���ϐ�
LARGE_INTEGER start, stop, freq;
LARGE_INTEGER takestart, takeend1;
LARGE_INTEGER takestart0, takeend0;
LARGE_INTEGER hscstart, hscend;
double taketime = 0, taketime0 = 0, hsctime = 0;
double timer = 0;


//���O�Ɋւ���p�����[�^
struct Logs
{
	vector<cv::Mat> in_imgs_log;
	uint8_t* in_imgs_log_ptr;
	LARGE_INTEGER take_start, take_end;
	double taketime;
	int takepic_id, in_imgs_id;
};

//�v���g�^�C�v�錾
void TakePicture0(kayacoaxpress* cam, bool* flg);
void TakePicture1(kayacoaxpress* cam, bool* flg);
void TakePicture(kayacoaxpress* cam, bool* flg, Logs *log);

using namespace std;

#define SHOW_PROCESSING_TIME_


int main() {
	//�p�����[�^
	bool flg = true;
	if (!QueryPerformanceFrequency(&freq)) { return 0; }// �P�ʏK��

	//�J����0�̏�����
	kayacoaxpress cam0;
	Logs log0;
	cam0.connect(0);

	//�J�����p�����[�^�̐ݒ�
	cout << "Set Camera Params..." << endl;
	cam0.setParam(paramTypeKAYACoaXpress::AcquisitionMode::EnableAcquisitionFrameRate, 0); //�g���K�[���[�h�ŋN��
	cam0.setParam(paramTypeCamera::paramInt::WIDTH, width);
	cam0.setParam(paramTypeCamera::paramInt::HEIGHT, height);
	cam0.setParam(paramTypeKAYACoaXpress::paramInt::OffsetX, offsetx);
	cam0.setParam(paramTypeKAYACoaXpress::paramInt::OffsetY, offsety);
	cam0.setParam(paramTypeCamera::paramFloat::FPS, fps);
	cam0.setParam(paramTypeKAYACoaXpress::paramFloat::ExposureTime, exposuretime);
	cam0.setParam(paramTypeKAYACoaXpress::Gain::x1);
	cam0.setParam(paramTypeKAYACoaXpress::CaptureType::BayerGRGrab);
	cam0.parameter_all_print();


	//�J�����̏�����
	kayacoaxpress cam;
	Logs log;
	cam.connect(1);

	//�J�����p�����[�^�̐ݒ�
	cout << "Set Camera Params..." << endl;
	cam.setParam(paramTypeKAYACoaXpress::AcquisitionMode::EnableAcquisitionFrameRate, 1); //�g���K�[���[�h�ŋN��
	cam.setParam(paramTypeCamera::paramInt::WIDTH, width);
	cam.setParam(paramTypeCamera::paramInt::HEIGHT, height);
	cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetX, offsetx);
	cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetY, offsety);
	cam.setParam(paramTypeCamera::paramFloat::FPS, fps);
	cam.setParam(paramTypeKAYACoaXpress::paramFloat::ExposureTime, exposuretime);
	cam.setParam(paramTypeKAYACoaXpress::Gain::x1);
	cam.setParam(paramTypeKAYACoaXpress::CaptureType::BayerGRGrab);
	cam.parameter_all_print();

	//�摜�擾�p�̃����O�o�b�t�@�̍쐬
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

	//Log�̏�����
	log.in_imgs_id = 0;
	log.takepic_id = 0;
	log0.in_imgs_id = 0;
	log0.takepic_id = 0;


	//�J�����N��
	cout << "Camera Start!" << endl;
	cam.start();
	cam0.start();

	//�X���b�h�쐬
	//thread TakePictureThread0(TakePicture0, &cam0, &flg);
	//thread TakePictureThread1(TakePicture1, &cam, &flg);

	thread TakePictureThread0(TakePicture, &cam0, &flg, &log0);
	thread TakePictureThread1(TakePicture, &cam, &flg, &log);





	//���C�����[�v
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

		//OpenCV�ŉ摜�\��
		//cv::cvtColor(in_imgs[(in_imgs_saveid - 2 + ringbuffersize) % ringbuffersize], show_img, CV_RGB2BGR);
		cv::cvtColor(log.in_imgs_log[(log.in_imgs_id - 2 + ringbuffersize) % ringbuffersize], show_img, CV_RGB2BGR);
		cv::imshow("img", show_img);
		int key = cv::waitKey(1);
		if (key == 'q') flg = false;

		//cv::cvtColor(in_imgs0[(in_imgs_saveid0 - 2 + ringbuffersize) % ringbuffersize], show_img0, CV_RGB2BGR);
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

	//�X���b�h�폜
	if (TakePictureThread0.joinable())TakePictureThread0.join();
	if (TakePictureThread1.joinable())TakePictureThread1.join();



	//�J�����̒�~
	cam.stop();
	cam.disconnect();
	cam0.stop();
	cam0.disconnect();

	return 0;
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
		while (taketime0 < takepic_time)
		{
			QueryPerformanceCounter(&takeend0);
			taketime0 = (double)(takeend0.QuadPart - takestart0.QuadPart) / freq.QuadPart;
		}
		in_imgs_saveid0 = (in_imgs_saveid0 + 1) % ringbuffersize;

#ifdef SHOW_PROCESSING_TIME_
		std::cout << "TakePicture() time: " << taketime0 << endl;
#endif // SHOW_PROCESSING_TIME_
	}
}

void TakePicture1(kayacoaxpress* cam, bool* flg) {
	while (*flg)
	{
		QueryPerformanceCounter(&takestart);
		takepicid = in_imgs_saveid % ringbuffersize;
		in_img_multi_src = in_imgs[takepicid].ptr<uint8_t>(0);

		cam->captureFrame2(in_img_multi_src, multicnt);
		QueryPerformanceCounter(&takeend1);
		taketime = (double)(takeend1.QuadPart - takestart.QuadPart) / freq.QuadPart;
		while (taketime < takepic_time)
		{
			QueryPerformanceCounter(&takeend1);
			taketime = (double)(takeend1.QuadPart - takestart.QuadPart) / freq.QuadPart;
		}
		in_imgs_saveid = (in_imgs_saveid + 1) % ringbuffersize;

#ifdef SHOW_PROCESSING_TIME_
		std::cout << "TakePicture() time: " << taketime << endl;
#endif // SHOW_PROCESSING_TIME_
	}
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