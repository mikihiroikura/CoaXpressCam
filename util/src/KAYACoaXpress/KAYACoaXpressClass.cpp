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
	kayacoaxpressMessage("Frame Grabber Board Closed.");
}

void kayacoaxpress::parameter_all_print()
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
	std::cout << std::endl;
	KYFG_CameraClose(cam_handle);//�J���������
	status = KYFG_Close(fg_handle);//�{�[�h�����
	kayacoaxpressMessage("Camera disconnected.");
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
	switch (pT)
	{
	case paramTypeCamera::paramInt::WIDTH:
		if (KYFG_GetCameraValueInt(cam_handle, "WidthMax") >= param&& param%64==0) width = param;
		else if (param % 64 != 0)
		{
			kayacoaxpressMessage(" WIDTH : 64�̔{���𖞂����Ă��Ȃ��̂œ��͒l�ȉ��̍ő�l�ɐݒ肵�܂�");
			width = param/64*64;
		}
		else
		{
			kayacoaxpressMessage(" WIDTH : Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
			width = KYFG_GetCameraValueInt(cam_handle, "WidthMax");
		}
		KYFG_SetCameraValueInt(cam_handle, "Width", width);
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		if (KYFG_GetCameraValueInt(cam_handle, "HeightMax") >= param && param % 8 == 0) height = param;
		else if (param % 8 != 0)
		{
			kayacoaxpressMessage(" HEIGHT : 8�̔{���𖞂����Ă��Ȃ��̂œ��͒l�ȉ��̍ő�l�ɐݒ肵�܂�");
			height = param / 8 * 8;
		}
		else
		{
			kayacoaxpressMessage(" HEIGHT : Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
			height = KYFG_GetCameraValueInt(cam_handle, "HeightMax");
		}
		KYFG_SetCameraValueInt(cam_handle, "Height", height);
		break;
	default:
		break;
	}
}

void kayacoaxpress::setParam(const paramTypeCamera::paramFloat& pT, const float param)
{
	switch (pT)
	{
	case paramTypeCamera::paramFloat::FPS:
		if (KYFG_GetCameraValueFloat(cam_handle, "pFrameRateRegMax") >= param) fps = param;
		else
		{
			kayacoaxpressMessage(" FPS : Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
			fps = 1087.0;
		}
		KYFG_SetCameraValueInt(cam_handle, "AcquisitionFrameRate", fps);
		break;
	case paramTypeCamera::paramFloat::GAIN:
		gain = param;
		KYFG_SetCameraValueInt(cam_handle, "Gain", gain);
		break;
	default:
		break;
	}
}

int kayacoaxpress::getParam(const paramTypeCamera::paramInt& pT)
{
	int tmp=0;
	switch (pT)
	{
	case paramTypeCamera::paramInt::WIDTH:
		tmp = width;
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		tmp = height;
		break;
	}
	return tmp;
}

float kayacoaxpress::getParam(const paramTypeCamera::paramFloat& pT)
{
	float tmp=0;
	switch (pT)
	{
	case paramTypeCamera::paramFloat::FPS:
		tmp = fps;
		break;
	case paramTypeCamera::paramFloat::GAIN:
		tmp = gain;
		break;
	}
	return tmp;
}

void kayacoaxpress::kayacoaxpressMessage(std::string str)
{
	std::cout << "[KAYACoaXpress] " << str << std::endl;
}

