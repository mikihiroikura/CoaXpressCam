#pragma once
///
/// @file		idpExpressClass.hpp
/// @brief		idpExpress �J����
/// @author		Satoshi Tabata
/// @date		2017/6/9 �쐬
/// @date		2017/8/22 �ŏI�X�V
/// @version	1.1.2
///
///
#include <HSC/Camera.hpp>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <IdpExpress/PDCLIB.h>
#pragma comment(lib, "PDCLIB.lib")
#define USE_SSE 1

/// Camera.hpp �ֈړ�
/*
namespace paramTypeIdpExpress
{
	enum class AcquisitionMode
	{
		EnableAcquisitionFrameRate = 0,
		TriggerMode = 1
	};

	enum class CameraHead
	{
		CameraHead1 = 0,
		CameraHead2 = 1,
	};
};
*/

class idpExpress : public Camera
{
private:
	static constexpr int CAM_WIDTH = 512;
	static constexpr int CAM_HEIGHT = 512;
	static constexpr float CAM_FPS = 1000;
	/** �t���[�����[�g[fps](50, 60, 125, 250, 500, 1000����I���\) */
	//static constexpr int FRAME_PER_SEC = 1000;
	/** �J�����w�b�hID(2��ڑ�����1�䂾���g�p����ꍇ, 0or1) */
	//static constexpr int USC_CAM_HEAD_ID = 0;
	/** �X�e���I�ڑ�����SSE��p���������������邩�ǂ���(1�͎g��, 0�͎g��Ȃ�) */
	//static constexpr int USE_SSE = 1;

	/** �ڑ����ꂽ�f�o�C�X�̌������� */
	static PDC_DETECT_NUM_INFO DetectNumInfo;
	static int instanceCount;

	/** �f�o�C�X�ԍ� */
	unsigned long nDeviceNo;
	/** �f�o�C�X�̏�� */
	unsigned long nStatus;
	/** �q�f�o�C�X�̐� */
	unsigned long nCount;
	/** �q�f�o�C�X�ԍ� */
	unsigned long ChildNo[PDC_MAX_DEVICE];
	/** �擾�����摜�̔ԍ� */
	unsigned long nFrameNo;
	/** ��O�̉摜�̔ԍ� */
	unsigned long nOldFrameNo;
	/** �֐����۔���ϐ� */
	unsigned long nRet;
	/** �G���[�ԍ� */
	unsigned long nErrorCode;


	unsigned long maxWidth;
	unsigned long maxHeight;
	unsigned long maxFPS;

	/// �g���K���[�h��ON/OFF
	paramTypeIdpExpress::AcquisitionMode triggerMode;
	paramTypeIdpExpress::CameraHead useCameraHeadId;

	int showError(std::string str);
	int idpExpressMessage(std::string str);
	void parameter_all_print();

	void devideSSE(unsigned char *data, unsigned char *dataDst);
	int getFrame(unsigned char *data);
	void devideSSEStereo(unsigned char *data, unsigned char *dataDst1, unsigned char *dataDst2);
	int getFrameStereo(unsigned char *data1, unsigned char *data2);

	//int getFrameROI(cv::Mat image, int x_start, int x_end, int y_start, int y_end);
	//int getFrameStereoROI(cv::Mat image1, cv::Mat image2, int x_start, int x_end, int y_start, int y_end);
	//int getFrameStereoNumber(cv::Mat image1, cv::Mat image2);

public:


	idpExpress();

	virtual void connect(int id);
	virtual void disconnect();

	virtual void start();
	virtual void stop();

	// virtual void setParam(const paramType &pT, void* param);

	virtual void setParam(const paramTypeCamera::paramInt &pT, const int param);
	virtual void setParam(const paramTypeCamera::paramFloat &pT, const float param);

	virtual void setParam(const paramTypeIdpExpress::AcquisitionMode &pT);
	virtual void setParam(const paramTypeIdpExpress::CameraHead &pT);

	// virtual void getParam(const paramType &pT, void* param);

	virtual int getParam(const paramTypeCamera::paramInt &pT);
	virtual float getParam(const paramTypeCamera::paramFloat &pT);

	virtual void captureFrame(void* data);
	virtual void captureFrameStereo(void* dataL, void* dataR);

};

