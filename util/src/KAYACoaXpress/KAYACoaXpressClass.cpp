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
int kayacoaxpress::offsetx = 0;
int kayacoaxpress::offsety = 0;
float kayacoaxpress::exposuretime = 912;

static std::vector<cv::Mat> cycle_buffer_imgs;
cv::Mat cvt_img;
cv::Mat cvt_img_after;
const char* format_callback;
double fps;

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
		//printf("\rGood callback buffer handle:%X, current index:%d, total frames:%lld        ", streamHandle, buffIndex, totalFrames); //\r�͓����s�̐擪�ɖ߂邱�Ƃ��Ӗ�����
		if (format_callback == "BayerGR8")
		{
			memcpy(cvt_img.data, buffData, buffSize);
			cv::cvtColor(cvt_img, cycle_buffer_imgs[buffIndex], CV_BGR2RGB);
		}
		else if (format_callback == "Mono8")
		{
			memcpy(cycle_buffer_imgs[buffIndex].data, buffData, buffSize);
		}
		else
		{
			memcpy(cycle_buffer_imgs[buffIndex].data, buffData, buffSize);
		}
		//... Show Image with data ...
		//cv::imshow("img", in_img);
		copyingDataFlag = KYFALSE;
	}
}


//�������CFGB�̐��C�J�������̊m�F
kayacoaxpress::kayacoaxpress() : Camera(CAM_WIDTH, CAM_HEIGHT, CAM_FPS)
{
	//�ϐ��̏�����
	gain = "x1";
	offsetx = 0;
	offsety = 0;
	stream_handle = 0;
	
	//FGB�̔F���C�ڑ�
	status = KY_DeviceScan(&pcie_device_num);
	if (pcie_device_num == 0) std::runtime_error("No FGB device presrent.");
	for (size_t i = 0; i < pcie_device_num; i++)
	{
		std::cout << i << " " << KY_DeviceDisplayName(i) << std::endl;
	}
	fg_handle = KYFG_Open(0);
	cam_detect_cnt = 2;

	//�J�������̊m�F
	status = KYFG_UpdateCameraList(fg_handle, &allcam_handle[0], &cam_detect_cnt);//�{�[�h�ڑ����Ă���J�������ׂĔF��
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
	kayacoaxpressMessage("Gain : " + std::string(gain));
	kayacoaxpressMessage("OffsetX : " + std::to_string(offsetx));
	kayacoaxpressMessage("OffsetY : " + std::to_string(offsety));
	kayacoaxpressMessage("ExposureTime : " + std::to_string(exposuretime));
	kayacoaxpressMessage("PixelFormat : " + std::string(format));
	kayacoaxpressMessage("CycleBufferSize : " + std::to_string(cycle_buffer_size));
}

void kayacoaxpress::parameter_all_print_debug()
{
	kayacoaxpressMessage("Width : " + std::to_string(KYFG_GetCameraValueInt(cam_handle, "Width")));
	kayacoaxpressMessage("Height : " + std::to_string(KYFG_GetCameraValueInt(cam_handle, "Height")));
	kayacoaxpressMessage("Fps : " + std::to_string(KYFG_GetCameraValueFloat(cam_handle, "AcquisitionFrameRate")));
	kayacoaxpressMessage("Gain : " + std::to_string(KYFG_GetCameraValueEnum(cam_handle, "Gain")));
	kayacoaxpressMessage("OffsetX : " + std::to_string(KYFG_GetCameraValueInt(cam_handle, "OffsetX")));
	kayacoaxpressMessage("OffsetY : " + std::to_string(KYFG_GetCameraValueInt(cam_handle, "OffsetY")));
	kayacoaxpressMessage("ExposureTime : " + std::to_string(KYFG_GetCameraValueFloat(cam_handle, "ExposureTime")));
	kayacoaxpressMessage("PixelFormat :" + std::to_string(KYFG_GetGrabberValueEnum(fg_handle, "PixelFormat")));
	kayacoaxpressMessage("CycleBufferSize : " + std::to_string(cycle_buffer_size));
}

//�ʃJ�����ڑ�
void kayacoaxpress::connect(int id)
{
	//�J�����ڑ�
	cam_handle = allcam_handle[id];
	status = KYFG_CameraOpen2(cam_handle, NULL);
	if (status != FGSTATUS_OK) kayacoaxpressMessage("Camera cannot be opened.");

	//�J�����p�����[�^�̐ݒ�
	KYFG_SetCameraValueInt(cam_handle, "Width", CAM_WIDTH);
	KYFG_SetCameraValueInt(cam_handle, "Height", CAM_HEIGHT);
	KYFG_SetCameraValueFloat(cam_handle, "AcquisitionFrameRate", CAM_FPS);
}

void kayacoaxpress::disconnect()
{
	std::cout << std::endl;
	KYFG_CameraClose(cam_handle);//�J���������
	status = KYFG_Close(fg_handle);//�{�[�h�����
	kayacoaxpressMessage("Camera disconnected.");
}

//�p�����^�ݒ��CCycle_buffer���m�����ĎB���J�n
void kayacoaxpress::start()
{
	//Buffer�̃Z�b�g
	cv::Mat in_img;
	if (format == "Mono8") { in_img = cv::Mat(height, width, CV_8UC1, cv::Scalar::all(255)); }
	else if (format == "BayerGR8") { in_img = cv::Mat(height, width, CV_8UC3, cv::Scalar::all(255)); }
	else { in_img = cv::Mat(height, width, CV_8UC1, cv::Scalar::all(255)); }
	for (size_t i = 0; i < kayacoaxpress::cycle_buffer_size; i++)
	{
		cycle_buffer_imgs.push_back(in_img.clone());
	}
	cvt_img = cv::Mat(height, width, CV_8UC3, cv::Scalar::all(255));

	//Callback�֐��̃Z�b�g
	//status = KYFG_CameraCallbackRegister(cam_handle, Stream_callback_func, 0);
	status = KYFG_StreamCreateAndAlloc(cam_handle, &stream_handle, cycle_buffer_size, 0);//Cyclic frame buffer��Stream�̐ݒ�

	//�J�����̓���J�n�CFrames��0�ɂ���ƘA�����ĉ摜����葱����
	KYFG_CameraStart(cam_handle, stream_handle, 0);
}

void kayacoaxpress::stop()
{
	KYFG_CameraStop(cam_handle);
}

void kayacoaxpress::captureFrame(void* data)
{
	int callno;
	callno = KYFG_StreamGetFrameIndex(stream_handle) - 1;
	if (callno < 0) callno += kayacoaxpress::cycle_buffer_size;
	memcpy(data, cycle_buffer_imgs[callno].data, KYFG_StreamGetSize(stream_handle));
}

void kayacoaxpress::captureFrame2(void* data)
{
	long long buffSize = 0;
	int buffIndex;
	void* buffData;
	buffSize = KYFG_StreamGetSize(stream_handle);			// get buffer size 1920x1080
	buffIndex = KYFG_StreamGetFrameIndex(stream_handle);
	buffData = KYFG_StreamGetPtr(stream_handle, buffIndex);		// get pointer of buffer data
	if (format_callback == "BayerGR8")
	{
		memcpy(cvt_img.data, buffData, buffSize);
		cv::cvtColor(cvt_img, cvt_img_after, CV_BGR2RGB);
		memcpy(data, cvt_img_after.data, buffSize);
	}
	else if (format_callback == "Mono8")
	{
		memcpy(data, buffData, buffSize);
	}
	else
	{
		memcpy(data, buffData, buffSize);
	}
}

void kayacoaxpress::captureFrame(uint8_t* data, int num)
{
	int callno = KYFG_StreamGetFrameIndex(stream_handle) - 1;
	int outno;
	for (size_t i = 0; i < num; i++)
	{
		outno = callno - i;
		if (outno < 0) outno += kayacoaxpress::cycle_buffer_size;
		memcpy(data + KYFG_StreamGetSize(stream_handle) * i, cycle_buffer_imgs[outno].data, KYFG_StreamGetSize(stream_handle));
	}
}

void kayacoaxpress::captureFrame2(uint8_t* data, int num) {
	long long buffSize = 0;
	int buffIndexInit, buffIndex;
	void* buffData;
	buffSize = KYFG_StreamGetSize(stream_handle);			// get buffer size 1920x1080
	buffIndexInit = KYFG_StreamGetFrameIndex(stream_handle);
	for (size_t i = 0; i < num; i++)
	{
		buffIndex = (buffIndexInit - i + cycle_buffer_size) % cycle_buffer_size;
		buffData = KYFG_StreamGetPtr(stream_handle, buffIndex);		// get pointer of buffer data
		if (format_callback == "BayerGR8")
		{
			/*memcpy(cvt_img.data, buffData, buffSize);
			cv::cvtColor(cvt_img, cvt_img_after, CV_RGB2BGR);
			memcpy(data + buffSize, cvt_img_after.data, buffSize);*/

			memcpy(data + buffSize * i, buffData, buffSize);
		}
		else if (format_callback == "Mono8")
		{
			memcpy(data + buffSize * i, buffData, buffSize);
		}
		else
		{
			memcpy(data + buffSize * i, buffData, buffSize);
		}
	}	
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
		offsetx = KYFG_GetCameraValueInt(cam_handle, "OffsetX");
		fps = KYFG_GetCameraValueFloat(cam_handle, "AcquisitionFrameRate");
		exposuretime = KYFG_GetCameraValueFloat(cam_handle, "ExposureTime");
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
		offsety = KYFG_GetCameraValueInt(cam_handle, "OffsetY");
		fps = KYFG_GetCameraValueFloat(cam_handle, "AcquisitionFrameRate");
		exposuretime = KYFG_GetCameraValueFloat(cam_handle, "ExposureTime");
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
		KYFG_SetCameraValueFloat(cam_handle, "AcquisitionFrameRate", fps);
		break;
	default:
		break;
	}
}

void kayacoaxpress::setParam(const paramTypeKAYACoaXpress::Gain &pT)
{
	if (pT == paramTypeKAYACoaXpress::Gain::x1)
	{
		gain = "x1";
		KYFG_SetCameraValueEnum_ByValueName(cam_handle, "Gain", "x1");
	}
	else if (pT == paramTypeKAYACoaXpress::Gain::x2)
	{
		gain = "x2";
		KYFG_SetCameraValueEnum_ByValueName(cam_handle, "Gain", "x2");
	}
	else if (pT == paramTypeKAYACoaXpress::Gain::x4)
	{
		gain = "x4";
		KYFG_SetCameraValueEnum_ByValueName(cam_handle, "Gain", "x4");
	}
	else if (pT == paramTypeKAYACoaXpress::Gain::x8)
	{
		gain = "x8";
		KYFG_SetCameraValueEnum_ByValueName(cam_handle, "Gain", "x8");
	}
	else
	{
		gain = "x1";
		KYFG_SetCameraValueEnum_ByValueName(cam_handle, "Gain", "x1");
	}
}

void kayacoaxpress::setParam(const paramTypeKAYACoaXpress::paramInt& pT, const int param)
{
	switch (pT)
	{
	case paramTypeKAYACoaXpress::paramInt::OffsetX:
		if (KYFG_GetCameraValueInt(cam_handle, "OffsetXMax") >= param && param % 32 == 0) offsetx = param;
		else if (param % 32 != 0)
		{
			kayacoaxpressMessage(" OFFSETX : 32�̔{���𖞂����Ă��Ȃ��̂œ��͒l�ȉ��̍ő�l�ɐݒ肵�܂�");
			offsetx = param / 32 * 32;
		}
		else
		{
			kayacoaxpressMessage(" OFFSETX : Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
			offsetx = KYFG_GetCameraValueInt(cam_handle, "OffsetXMax");
		}
		KYFG_SetCameraValueInt(cam_handle, "OffsetX", offsetx);
		break;
	case paramTypeKAYACoaXpress::paramInt::OffsetY:
		if (KYFG_GetCameraValueInt(cam_handle, "OffsetYMax") >= param && param % 4 == 0) offsety = param;
		else if (param % 4 != 0)
		{
			kayacoaxpressMessage(" OFFSETY : 4�̔{���𖞂����Ă��Ȃ��̂œ��͒l�ȉ��̍ő�l�ɐݒ肵�܂�");
			offsetx = param / 4 * 4;
		}
		else
		{
			kayacoaxpressMessage(" OFFSETY : Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
			offsety = KYFG_GetCameraValueInt(cam_handle, "OffsetYMax");
		}
		KYFG_SetCameraValueInt(cam_handle, "OffsetY", offsety);
		break;
	case paramTypeKAYACoaXpress::paramInt::CycleBufferSize:
		kayacoaxpress::cycle_buffer_size = param;
		break;
	default:
		break;
	}
}

void kayacoaxpress::setParam(const paramTypeKAYACoaXpress::paramFloat& pT, const float param)
{
	switch (pT)
	{
	case paramTypeKAYACoaXpress::paramFloat::ExposureTime:
		if (KYFG_GetCameraValueFloat(cam_handle, "pExposureTimeRegMax") >= param) exposuretime = param;
		else
		{
			kayacoaxpressMessage(" EXPOSURETIME : Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
			exposuretime = KYFG_GetCameraValueFloat(cam_handle, "pExposureTimeRegMax");
		}
		KYFG_SetCameraValueFloat(cam_handle, "ExposureTime", exposuretime);
		break;
	default:
		break;
	}
}

void kayacoaxpress::setParam(const paramTypeKAYACoaXpress::CaptureType& pt)
{
	switch (pt)
	{
	case paramTypeKAYACoaXpress::CaptureType::Monocro8Grab:
		format = "Mono8";
		KYFG_SetCameraValueEnum_ByValueName(cam_handle, "PixelFormat", "Mono8");
		KYFG_SetGrabberValueEnum_ByValueName(fg_handle, "PixelFormat", "Mono8");
		break;
	case paramTypeKAYACoaXpress::CaptureType::BayerGRGrab:
		format = "BayerGR8";
		KYFG_SetCameraValueEnum_ByValueName(cam_handle, "PixelFormat", "BayerGR8");
		KYFG_SetGrabberValueEnum_ByValueName(fg_handle, "PixelFormat", "RGB8");
		break;
	default:
		break;
	}
	format_callback = format;
}

void kayacoaxpress::setParam(const paramTypeKAYACoaXpress::AcquisitionMode& pt, int id) {
	switch (pt)
	{
	case paramTypeKAYACoaXpress::AcquisitionMode::EnableAcquisitionFrameRate:
		acquisition_mode = "ContinuousMode";
		KYFG_SetCameraValueEnum_ByValueName(cam_handle, "AcquisitionMode", "Continuous");
		KYFG_SetGrabberValueEnum(fg_handle, "CameraSelector", id);
		KYFG_SetGrabberValueEnum_ByValueName(fg_handle, "CameraTriggerMode", "Off");
		break;
	case paramTypeKAYACoaXpress::AcquisitionMode::TriggerMode:
		acquisition_mode = "TriggerMode";
		KYFG_SetCameraValueEnum_ByValueName(cam_handle, "AcquisitionMode", "SingleFrame");
		KYFG_SetGrabberValueEnum(fg_handle, "CameraSelector", id);
		KYFG_SetGrabberValueEnum_ByValueName(fg_handle, "CameraTriggerMode", "On");
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
	}
	return tmp;
}

const char* kayacoaxpress::getParam(const paramTypeKAYACoaXpress::Gain& pT)
{
	const char* tmp;
	tmp = gain;
	return tmp;
}

int kayacoaxpress::getParam(const paramTypeKAYACoaXpress::paramInt& pT)
{
	int tmp = 0;
	switch (pT)
	{
	case paramTypeKAYACoaXpress::paramInt::CycleBufferSize:
		tmp = cycle_buffer_size;
		break;
	case paramTypeKAYACoaXpress::paramInt::OffsetX:
		tmp = offsetx;
		break;
	case paramTypeKAYACoaXpress::paramInt::OffsetY:
		tmp = offsety;
		break;
	}
	return tmp;
}

float kayacoaxpress::getParam(const paramTypeKAYACoaXpress::paramFloat& pT)
{
	float tmp = 0;
	switch (pT)
	{
	case paramTypeKAYACoaXpress::paramFloat::ExposureTime:
		tmp = exposuretime;
		break;
	}
	return tmp;
}

const char* kayacoaxpress::getParam(const paramTypeKAYACoaXpress::CaptureType& pT)
{
	const char* tmp;
	tmp = format;
	return tmp;
}

const char* kayacoaxpress::getParam(const paramTypeKAYACoaXpress::AcquisitionMode& pT) {
	const char* tmp;
	tmp = acquisition_mode;
	return tmp;
}


void kayacoaxpress::kayacoaxpressMessage(std::string str)
{
	std::cout << "[KAYACoaXpress] " << str << std::endl;
}

void kayacoaxpress::debug_func()
{
	KYFG_SetGrabberValueEnum_ByValueName(fg_handle, "PixelFormat", "RGB8");
}

