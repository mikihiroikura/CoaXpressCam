///
/// @file		KAYACoaXpressClass.cpp
/// @brief		KAYACoaXpress Optronics�J����
/// @author		Mikihiro Ikura
/// @date		2020/7/21 �쐬
/// @date		2020/7/21 �ŏI�X�V
/// @version	1.0.0
///

#include <HSC/KAYACoaXpressClass.hpp>
#include <iostream>

int kayacoaxpress::status = 0;
int kayacoaxpress::pcie_device_num = 0;
int kayacoaxpress::cam_detect_cnt = 0;
int kayacoaxpress::cycle_buffer_size = 20;

static std::vector<cv::Mat> cycle_buffer_imgs;


//Callback�֐�
void Stream_callback_func(void* userContext, STREAM_HANDLE streamHandle)
{
	static KYBOOL copyingDataFlag = KYFALSE;
	long long totalFrames = 0, buffSize = 0;
	int buffIndex;
	void* buffData;

	if (0 == streamHandle)		// callback with indicator for acquisition stop
	{
		copyingDataFlag = KYFALSE;
		return;
	}

	totalFrames = KYFG_GetGrabberValueInt(streamHandle, "RXFrameCounter");
	buffSize = KYFG_StreamGetSize(streamHandle);			// get buffer size 1920x1080
	buffIndex = KYFG_StreamGetFrameIndex(streamHandle);
	buffData = KYFG_StreamGetPtr(streamHandle, buffIndex);		// get pointer of buffer data

	if (KYFALSE == copyingDataFlag)
	{
		copyingDataFlag = KYTRUE;
		printf("\rGood callback buffer handle:%X, current index:%d, total frames:%lld        ", streamHandle, buffIndex, totalFrames); //\r�͓����s�̐擪�ɖ߂邱�Ƃ��Ӗ�����
		memcpy(cycle_buffer_imgs[buffIndex].data, buffData, buffSize);			// copy data to local buffer
		//... Show Image with data ...
		//cv::imshow("img", in_img);
		copyingDataFlag = KYFALSE;
	}
}


//�������CFGB�̐��C�J�������̊m�F
kayacoaxpress::kayacoaxpress() : Camera(CAM_WIDTH, CAM_HEIGHT, CAM_FPS)
{
	//�ϐ��̏�����
	stream_handle = 0;
	cv::Mat in_img = cv::Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC1, cv::Scalar::all(255));
	for (size_t i = 0; i < kayacoaxpress::cycle_buffer_size; i++)
	{
		cycle_buffer_imgs.push_back(in_img.clone());
	}

	//FGB�̔F���C�ڑ�
	status = KY_DeviceScan(&pcie_device_num);
	if (pcie_device_num == 0) std::runtime_error("No FGB device presrent.");
	for (size_t i = 0; i < pcie_device_num; i++)
	{
		std::cout << i << " " << KY_DeviceDisplayName(i) << std::endl;
	}
	fg_handle = KYFG_Open(0);
	cam_detect_cnt = 1;

	//�J�������̊m�F
	status = KYFG_UpdateCameraList(fg_handle, &cam_handle, &cam_detect_cnt);//�{�[�h�ڑ����Ă���J�������ׂĔF��
}

kayacoaxpress::~kayacoaxpress()
{
	status = KYFG_Close(fg_handle);
	kayacoaxpressMessage("\nFrame Grabber Board Closed.");
}

void kayacoaxpress::params_all_print()
{
	kayacoaxpressMessage("Width : " + std::to_string(width));
	kayacoaxpressMessage("Height : " + std::to_string(height));
	kayacoaxpressMessage("Fps : " + std::to_string(fps));
	kayacoaxpressMessage("Gain : " + std::to_string(gain));
}

//�ʃJ�����ڑ�
void kayacoaxpress::connect(int id)
{
	//�J�����ڑ�
	status = KYFG_CameraOpen2(cam_handle, NULL);
	if (status != FGSTATUS_OK) std::runtime_error("Camera cannot be opened.");

	//�J�����p�����[�^�̐ݒ�
	KYFG_SetCameraValueInt(cam_handle, "Width", CAM_WIDTH);
	KYFG_SetCameraValueInt(cam_handle, "Height", CAM_HEIGHT);
	KYFG_SetCameraValueFloat(cam_handle, "AcquisitionFrameRate", CAM_FPS);
	KYFG_SetCameraValueEnum_ByValueName(cam_handle, "PixelFormat", "Mono8");

	//Callback�֐��̃Z�b�g
	status = KYFG_CameraCallbackRegister(cam_handle, Stream_callback_func, 0);
	status = KYFG_StreamCreateAndAlloc(cam_handle, &stream_handle, cycle_buffer_size, 0);//Cyclic frame buffer��Stream�̐ݒ�
}

void kayacoaxpress::disconnect()
{
	KYFG_CameraClose(cam_handle);//�J���������
	status = KYFG_Close(fg_handle);//�{�[�h�����
}

//�p�����^�ݒ��C�B���J�n
void kayacoaxpress::start()
{
	KYFG_CameraStart(cam_handle, stream_handle, 0);//�J�����̓���J�n�CFrames��0�ɂ���ƘA�����ĉ摜����葱����
}

void kayacoaxpress::stop()
{
	KYFG_CameraStop(cam_handle);
}

void kayacoaxpress::captureFrame(void* data)
{
	int callno = KYFG_StreamGetFrameIndex(stream_handle)-1;
	if (callno < 0) callno += kayacoaxpress::cycle_buffer_size;
	memcpy(data, cycle_buffer_imgs[callno].data, KYFG_StreamGetSize(stream_handle));
}

void kayacoaxpress::setParam(const paramTypeCamera::paramInt& pT, const int param)
{

}

void kayacoaxpress::setParam(const paramTypeCamera::paramFloat& pT, const float param)
{

}

int kayacoaxpress::getParam(const paramTypeCamera::paramInt& pT)
{
	return 0;
}

float kayacoaxpress::getParam(const paramTypeCamera::paramFloat& pT)
{
	return 0;
}

void kayacoaxpress::kayacoaxpressMessage(std::string str)
{
	std::cout << "[KAYACoaXpress] " << str << std::endl;
}

