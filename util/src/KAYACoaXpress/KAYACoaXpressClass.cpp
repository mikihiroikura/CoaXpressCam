///
/// @file		KAYACoaXpressClass.cpp
/// @brief		KAYACoaXpress Optronicsカメラ
/// @author		Mikihiro Ikura
/// @date		2020/7/21 作成
/// @date		2020/7/21 最終更新
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
const char* format_callback;
double fps;

//Callback関数
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
		//printf("\rGood callback buffer handle:%X, current index:%d, total frames:%lld        ", streamHandle, buffIndex, totalFrames); //\rは同じ行の先頭に戻ることを意味する
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


//初期化，FGBの数，カメラ数の確認
kayacoaxpress::kayacoaxpress() : Camera(CAM_WIDTH, CAM_HEIGHT, CAM_FPS)
{
	//変数の初期化
	gain = "x1";
	offsetx = 0;
	offsety = 0;
	stream_handle = 0;
	
	//FGBの認識，接続
	status = KY_DeviceScan(&pcie_device_num);
	if (pcie_device_num == 0) std::runtime_error("No FGB device presrent.");
	for (size_t i = 0; i < pcie_device_num; i++)
	{
		std::cout << i << " " << KY_DeviceDisplayName(i) << std::endl;
	}
	fg_handle = KYFG_Open(0);
	cam_detect_cnt = 2;

	//カメラ数の確認
	status = KYFG_UpdateCameraList(fg_handle, &allcam_handle[0], &cam_detect_cnt);//ボード接続しているカメラすべて認識
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
	kayacoaxpressMessage("CycleBufferSize : " + std::to_string(cycle_buffer_size));
}

//個別カメラ接続
void kayacoaxpress::connect(int id)
{
	//カメラ接続
	cam_handle = allcam_handle[id];
	status = KYFG_CameraOpen2(cam_handle, NULL);
	if (status != FGSTATUS_OK) kayacoaxpressMessage("Camera cannot be opened.");

	//カメラパラメータの設定
	KYFG_SetCameraValueInt(cam_handle, "Width", CAM_WIDTH);
	KYFG_SetCameraValueInt(cam_handle, "Height", CAM_HEIGHT);
	KYFG_SetCameraValueFloat(cam_handle, "AcquisitionFrameRate", CAM_FPS);
}

void kayacoaxpress::disconnect()
{
	std::cout << std::endl;
	KYFG_CameraClose(cam_handle);//カメラを閉じる
	status = KYFG_Close(fg_handle);//ボードを閉じる
	kayacoaxpressMessage("Camera disconnected.");
}

//パラメタ設定後，Cycle_bufferを確立して撮像開始
void kayacoaxpress::start()
{
	//Bufferのセット
	cv::Mat in_img;
	if (format == "Mono8") { in_img = cv::Mat(height, width, CV_8UC1, cv::Scalar::all(255)); }
	else if (format == "BayerGR8") { in_img = cv::Mat(height, width, CV_8UC3, cv::Scalar::all(255)); }
	else { in_img = cv::Mat(height, width, CV_8UC1, cv::Scalar::all(255)); }
	for (size_t i = 0; i < kayacoaxpress::cycle_buffer_size; i++)
	{
		cycle_buffer_imgs.push_back(in_img.clone());
	}
	cvt_img = cv::Mat(height, width, CV_8UC3, cv::Scalar::all(255));

	//Callback関数のセット
	status = KYFG_CameraCallbackRegister(cam_handle, Stream_callback_func, 0);
	status = KYFG_StreamCreateAndAlloc(cam_handle, &stream_handle, cycle_buffer_size, 0);//Cyclic frame bufferのStreamの設定

	//カメラの動作開始，Framesを0にすると連続して画像を取り続ける
	KYFG_CameraStart(cam_handle, stream_handle, 0);
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
			kayacoaxpressMessage(" WIDTH : 64の倍数を満たしていないので入力値以下の最大値に設定します");
			width = param/64*64;
		}
		else
		{
			kayacoaxpressMessage(" WIDTH : Max以上の設定値を与えているのでMaxに設定します");
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
			kayacoaxpressMessage(" HEIGHT : 8の倍数を満たしていないので入力値以下の最大値に設定します");
			height = param / 8 * 8;
		}
		else
		{
			kayacoaxpressMessage(" HEIGHT : Max以上の設定値を与えているのでMaxに設定します");
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
			kayacoaxpressMessage(" FPS : Max以上の設定値を与えているのでMaxに設定します");
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
			kayacoaxpressMessage(" OFFSETX : 32の倍数を満たしていないので入力値以下の最大値に設定します");
			offsetx = param / 32 * 32;
		}
		else
		{
			kayacoaxpressMessage(" OFFSETX : Max以上の設定値を与えているのでMaxに設定します");
			offsetx = KYFG_GetCameraValueInt(cam_handle, "OffsetXMax");
		}
		KYFG_SetCameraValueInt(cam_handle, "OffsetX", offsetx);
		break;
	case paramTypeKAYACoaXpress::paramInt::OffsetY:
		if (KYFG_GetCameraValueInt(cam_handle, "OffsetYMax") >= param && param % 4 == 0) offsety = param;
		else if (param % 4 != 0)
		{
			kayacoaxpressMessage(" OFFSETY : 4の倍数を満たしていないので入力値以下の最大値に設定します");
			offsetx = param / 4 * 4;
		}
		else
		{
			kayacoaxpressMessage(" OFFSETY : Max以上の設定値を与えているのでMaxに設定します");
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
		if (KYFG_GetCameraValueInt(cam_handle, "pExposureTimeRegMax") >= param) exposuretime = param;
		else
		{
			kayacoaxpressMessage(" EXPOSURETIME : Max以上の設定値を与えているのでMaxに設定します");
			exposuretime = KYFG_GetCameraValueInt(cam_handle, "pExposureTimeRegMax");
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


void kayacoaxpress::kayacoaxpressMessage(std::string str)
{
	std::cout << "[KAYACoaXpress] " << str << std::endl;
}

void kayacoaxpress::debug_func()
{
	KYFG_SetGrabberValueEnum_ByValueName(fg_handle, "PixelFormat", "RGB8");
}

