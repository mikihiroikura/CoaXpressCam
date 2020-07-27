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

static std::vector<cv::Mat> cycle_buffer_imgs;


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
		printf("\rGood callback buffer handle:%X, current index:%d, total frames:%lld        ", streamHandle, buffIndex, totalFrames); //\rは同じ行の先頭に戻ることを意味する
		memcpy(cycle_buffer_imgs[buffIndex].data, buffData, buffSize);			// copy data to local buffer
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
	stream_handle = 0;
	cv::Mat in_img = cv::Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC1, cv::Scalar::all(255));
	for (size_t i = 0; i < kayacoaxpress::cycle_buffer_size; i++)
	{
		cycle_buffer_imgs.push_back(in_img.clone());
	}

	//FGBの認識，接続
	status = KY_DeviceScan(&pcie_device_num);
	if (pcie_device_num == 0) std::runtime_error("No FGB device presrent.");
	for (size_t i = 0; i < pcie_device_num; i++)
	{
		std::cout << i << " " << KY_DeviceDisplayName(i) << std::endl;
	}
	fg_handle = KYFG_Open(0);
	cam_detect_cnt = 1;

	//カメラ数の確認
	status = KYFG_UpdateCameraList(fg_handle, &cam_handle, &cam_detect_cnt);//ボード接続しているカメラすべて認識
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
}

//個別カメラ接続
void kayacoaxpress::connect(int id)
{
	//カメラ接続
	status = KYFG_CameraOpen2(cam_handle, NULL);
	if (status != FGSTATUS_OK) std::runtime_error("Camera cannot be opened.");

	//カメラパラメータの設定
	KYFG_SetCameraValueInt(cam_handle, "Width", CAM_WIDTH);
	KYFG_SetCameraValueInt(cam_handle, "Height", CAM_HEIGHT);
	KYFG_SetCameraValueFloat(cam_handle, "AcquisitionFrameRate", CAM_FPS);
	KYFG_SetCameraValueEnum_ByValueName(cam_handle, "PixelFormat", "Mono8");

	//Callback関数のセット
	status = KYFG_CameraCallbackRegister(cam_handle, Stream_callback_func, 0);
	status = KYFG_StreamCreateAndAlloc(cam_handle, &stream_handle, cycle_buffer_size, 0);//Cyclic frame bufferのStreamの設定
}

void kayacoaxpress::disconnect()
{
	std::cout << std::endl;
	KYFG_CameraClose(cam_handle);//カメラを閉じる
	status = KYFG_Close(fg_handle);//ボードを閉じる
	kayacoaxpressMessage("Camera disconnected.");
}

//パラメタ設定後，撮像開始
void kayacoaxpress::start()
{
	KYFG_CameraStart(cam_handle, stream_handle, 0);//カメラの動作開始，Framesを0にすると連続して画像を取り続ける
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
		KYFG_SetCameraValueInt(cam_handle, "AcquisitionFrameRate", fps);
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

void kayacoaxpress::kayacoaxpressMessage(std::string str)
{
	std::cout << "[KAYACoaXpress] " << str << std::endl;
}

