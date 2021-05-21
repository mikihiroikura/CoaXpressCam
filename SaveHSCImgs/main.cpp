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
#include "RS232c.h"

#ifdef _DEBUG
#define LIB_EXT "d.lib"
#else
#define LIB_EXT ".lib"
#endif

#pragma comment(lib,"KAYACoaXpressLib" LIB_EXT)
#pragma warning(disable:4996)
using namespace std;

//�X���b�h�̏�������
double takepic_time = 0.0004;
double showgl_time = 0.01667;
double showhsc_time = 0.0333;
double calcpose_time = 0.002;

cv::Mat in_img;
cv::Mat zero, full, zeromulti;
//�J�����p�����[�^
const int width = 896;
const int height = 896;
const float fps = 2000.0;
const float exposuretime = 912.0;
const int offsetx = 512;
const int offsety = 92;
double map_coeff[4], stretch_mat[4], det, distort[4];
const int multicnt = 1;

//���ԂɊւ���ϐ�
LARGE_INTEGER start, stop, freq;
LARGE_INTEGER takestart, takeend;
LARGE_INTEGER savehscstart, savehscend;
LARGE_INTEGER hscstart, hscend;
LARGE_INTEGER glstart, glend;
LARGE_INTEGER logstart, logend, glrslogend, hsclogend;
LARGE_INTEGER detectstart, detectstartdebug, detectend;
double taketime = 0, hsctime = 0, logtime = 0, gltime = 0, glrslogtime = 0, hsclogtime = 0, savehsctime = 0;
double timer = 0, gltimer = 0;
double detecttimea = 0, detecttimeb = 0, detecttimec = 0, detecttimed = 0, detecttimee = 0, detecttimef = 0, detecttime = 0;

//���O�Ɋւ���p�����[�^
const int timeout = 5;
const int log_img_fps = 70;
const int log_img_fps_hs = 500;
const int log_led_fps = 500;
const int log_img_finish_cnt = log_img_fps * timeout + 100;
const int log_img_finish_cnt_hs = log_img_fps_hs * timeout + 100;
const int log_pose_finish_cnt = log_led_fps * timeout + 100;
long long log_glrsimg_cnt = 0, log_hscimg_cnt = 0, log_pose_cnt = 0;
bool showsavehscimg = false, showsaveglimg = false;
uint8_t* save_img_on_src;
bool saveimgsflg = false, saveimgsstartflg = false;

/// �P�����{�b�g�Ɋւ���ϐ�
RS232c axisrobot;
#define READBUFFERSIZE 256
char replybuf[READBUFFERSIZE];
char axisrobmodes[][10] = { "@SRVO", "@START", "@ORG" };
char axisrobcommand[READBUFFERSIZE] = "";
const int initaxisstart = 100, initaxisend = 500;
const int posunits = 100, speedunits = 10;

//#define MOVE_AXISROBOT_
//#define SAVE_IMGS_

struct Logs
{
	vector<cv::Mat> in_imgs_log;
	vector<cv::Mat> gl_imgs_log;
	vector<vector<cv::Mat>> rs_imgs_log;
	cv::Mat* in_imgs_log_ptr;
	cv::Mat* gl_imgs_log_ptr;
	double* LED_times;
	double* log_times;
	double* glrslog_times;
	double* glrslog_times_diff;
	double* hsclog_times;
	double* hsclog_times_diff;
	int* LED_results;
};

void Read_Reply_toEND(RS232c* robot);
void wait_QueryPerformance(double finishtime, LARGE_INTEGER freq);
void ControlAxisRobot(RS232c* robot, bool* flg);

int main() {
	//�p�����[�^
	bool flg = true;
	if (!QueryPerformanceFrequency(&freq)) { return 0; }// �P�ʏK��

	//�J�����̏�����
	kayacoaxpress cam;
	cam.connect(1);

	//�J�����p�����[�^�̐ݒ�
	cout << "Set Camera Params..." << endl;
	cam.setParam(paramTypeKAYACoaXpress::AcquisitionMode::TriggerMode, 1); //�g���K�[���[�h�ŋN��
	cam.setParam(paramTypeKAYACoaXpress::CaptureType::BayerGRGrab);
	cam.setParam(paramTypeCamera::paramInt::WIDTH, width);
	cam.setParam(paramTypeCamera::paramInt::HEIGHT, height);
	cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetX, offsetx);
	cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetY, offsety);
	cam.setParam(paramTypeCamera::paramFloat::FPS, fps);
	cam.setParam(paramTypeKAYACoaXpress::paramFloat::ExposureTime, exposuretime);
	
	cam.setParam(paramTypeKAYACoaXpress::Gain::x1);
	
	cam.parameter_all_print();
	cam.parameter_all_print_debug();

	zero = cv::Mat(cam.getParam(paramTypeCamera::paramInt::HEIGHT), cam.getParam(paramTypeCamera::paramInt::WIDTH), CV_8UC3, cv::Scalar::all(0));
	zeromulti = cv::Mat(cam.getParam(paramTypeCamera::paramInt::HEIGHT) * multicnt, cam.getParam(paramTypeCamera::paramInt::WIDTH), CV_8UC3, cv::Scalar::all(0));
	in_img = zeromulti.clone();

	Logs logs;
	for (size_t i = 0; i < log_img_finish_cnt_hs; i++) { logs.in_imgs_log.push_back(zero.clone()); }
	logs.in_imgs_log_ptr = logs.in_imgs_log.data();

	//�P�����{�b�g
#ifdef MOVE_AXISROBOT_
	std::cout << "Set commection to AXIS ROBOT...............";
	if (!axisrobot.Connect("COM6", 38400, 8, ODDPARITY, 0, 0, 0, 20000, 20000)) {
		cout << "No connect" << endl;
		return 1;
	}
	std::cout << "OK!" << endl;
	snprintf(axisrobcommand, READBUFFERSIZE, "%s%d.1\r\n", axisrobmodes[0], 1);
	axisrobot.Send(axisrobcommand);
	memset(axisrobcommand, '\0', READBUFFERSIZE);
	Read_Reply_toEND(&axisrobot);
	std::cout << "SERVO ON" << endl;
	wait_QueryPerformance(0.1, freq);
	snprintf(axisrobcommand, READBUFFERSIZE, "%s.1\r\n", axisrobmodes[2]);
	axisrobot.Send(axisrobcommand);
	cout << "ORG START" << endl;
	memset(axisrobcommand, '\0', READBUFFERSIZE);
	Read_Reply_toEND(&axisrobot);
	std::cout << "ORG STOP" << endl;
	thread MoveAxisRobotThread(ControlAxisRobot, &axisrobot, &flg);
#endif // MOVE_AXISROBOT_

	//�J�����N��
	cout << "Camera Start!" << endl;
	cam.start();

	while (flg)
	{
		QueryPerformanceCounter(&takestart);
		cam.captureFrame2(in_img.data, multicnt);
		QueryPerformanceCounter(&takeend);
		taketime = (double)(takeend.QuadPart - takestart.QuadPart) / freq.QuadPart;
		while (taketime < takepic_time)
		{
			QueryPerformanceCounter(&takeend);
			taketime = (double)(takeend.QuadPart - takestart.QuadPart) / freq.QuadPart;
		}
		std::cout << "TakePicture() time: " << taketime << endl;

		//LED�摜�̕ۑ�
#ifdef SAVE_IMGS_
		memcpy((logs.in_imgs_log_ptr + log_hscimg_cnt)->data, in_img.data, height * width * 3);


		log_hscimg_cnt++;
		if (log_hscimg_cnt >= log_img_finish_cnt_hs) flg = false;
#endif // SAVE_IMGS_
	}

#ifdef MOVE_AXISROBOT_
	if (MoveAxisRobotThread.joinable())MoveAxisRobotThread.join();
	//�P�����{�b�g�̒�~
	snprintf(axisrobcommand, READBUFFERSIZE, "%s%d.1\r\n", axisrobmodes[0], 0);
	axisrobot.Send(axisrobcommand);
	Read_Reply_toEND(&axisrobot);
	memset(axisrobcommand, '\0', READBUFFERSIZE);
	cout << "SERVO OFF" << endl;
#endif // MOVE_AXISROBOT_

	//�J�����̒�~
	cam.stop();
	cam.disconnect();


	FILE* fr;
	time_t timer;
	struct tm now;
	timer = time(NULL);
	localtime_s(&now, &timer);
	char dir[256];
	char picdir[256];
	char logfile[256];
	struct stat statBuf;

#ifdef SAVE_IMGS_
	std::cout << "Saving imgs..." << endl;
	//HSC�̉摜�ۑ�
	strftime(picdir, 256, "D:/Github_output/SuperImposition/SaveHSCImgs/%y%m%d", &now);
	if (stat(picdir, &statBuf) != 0) {
		if (_mkdir(picdir) != 0) { return 0; }
	}
	strftime(picdir, 256, "D:/Github_output/SuperImposition/SaveHSCImgs/%y%m%d/%H%M%S", &now);
	if (stat(picdir, &statBuf) != 0) {
		if (_mkdir(picdir) != 0) { return 0; }
	}
	strftime(picdir, 256, "D:/Github_output/SuperImposition/SaveHSCImgs/%y%m%d/%H%M%S/HSC", &now);
	if (stat(picdir, &statBuf) != 0) {
		if (_mkdir(picdir) != 0) { return 0; }
	}
	char picturename[256];
	char picsubname[256];
	strftime(picsubname, 256, "D:/Github_output/SuperImposition/SaveHSCImgs/%y%m%d/%H%M%S/HSC/frame", &now);
	for (int i = 0; i < log_hscimg_cnt; i++)
	{
		sprintf(picturename, "%s%05d.png", picsubname, i);//png�t���k
		cv::imwrite(picturename, logs.in_imgs_log[i]);
	}
#endif // SAVE_IMGS_

	

	return 0;
}

void Read_Reply_toEND(RS232c* robot) {
	char bufreply[256];
	while (true)
	{
		robot->Read_CRLF(bufreply, 256);
		if (bufreply[0] == 'E' || bufreply[0] == 'O') {
			break;
		}
	}
}


void ControlAxisRobot(RS232c* robot, bool* flg) {
	srand(time(NULL));
	int axisspeed = 100;
	int axisposition = 200000;
	int initaxispos = 600;
	char controlcommand[READBUFFERSIZE];
	while (*flg)
	{
		//�ʒu�Ƒ��x�̃����_���ݒ�
		if (initaxispos == initaxisend) initaxispos = initaxisstart;
		else if (initaxispos == initaxisstart) initaxispos = initaxisend;
		else initaxispos = initaxisstart;
		axisposition = (initaxispos + rand() % posunits + 1) * 100; //0~100 or 600~700
		axisspeed = 80; //10~100��10����

		//�R�}���h���M
		snprintf(controlcommand, READBUFFERSIZE, "@S_17.1=%d\r\n", axisspeed);
		robot->Send(controlcommand);
		memset(controlcommand, '\0', READBUFFERSIZE);
		Read_Reply_toEND(robot);
		snprintf(controlcommand, READBUFFERSIZE, "@START17#P%d.1\r\n", axisposition);
		robot->Send(controlcommand);
		memset(controlcommand, '\0', READBUFFERSIZE);
		Read_Reply_toEND(robot);
	}
}

void wait_QueryPerformance(double finishtime, LARGE_INTEGER freq) {
	LARGE_INTEGER waitstart, waitstop;
	double waittime = 0;
	QueryPerformanceCounter(&waitstart);
	while (waittime < finishtime)
	{
		QueryPerformanceCounter(&waitstop);
		waittime = (double)(waitstop.QuadPart - waitstart.QuadPart) / freq.QuadPart;
	}
}