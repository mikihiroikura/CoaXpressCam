#pragma once

///
/// @file		KAYACoaXpressClass.cpp
/// @brief		KAYACoaXpress Optronicsカメラ
/// @author		Mikihiro Ikura
/// @date		2020/7/21 作成
/// @date		2020/7/21 最終更新
/// @version	1.0.0
///

#include <HSC/Camera.hpp>
#include <KYFGLib.h>
#include <KYFGcppLib.h>
#include <clserkyi.h>
#include <vector>
#include <opencv2/opencv.hpp>

#pragma comment(lib,"clserkyi_vc141.lib")
#pragma comment(lib,"KYFGLib.lib")
#pragma comment(lib,"KYFGLib_vc141.lib")

class kayacoaxpress : public Camera
{
private:
	static constexpr int CAM_WIDTH = 1920;
	static constexpr int CAM_HEIGHT = 1080;
	static constexpr int CAM_FPS = 1000;

	static int status;
	static int pcie_device_num;
	static int cam_detect_cnt;
	static int cycle_buffer_size;
	FGHANDLE fg_handle;
	CAMHANDLE cam_handle;
	CAMHANDLE allcam_handle[2];
	STREAM_HANDLE stream_handle;

	const char* gain;
	const char* format;
	static int offsetx;
	static int offsety;
	static float exposuretime;
	const char* acquisition_mode;
	

public:
	kayacoaxpress();
	~kayacoaxpress();
	void parameter_all_print();
	void parameter_all_print_debug();

	void connect(int id);
	void disconnect();

	void start();
	void stop();

	void captureFrame(void* data);
	void captureFrame(uint8_t* data, int num);
	void captureFrame2(void* data);
	void captureFrame2(uint8_t* data, int num);

	void setParam(const paramTypeCamera::paramInt& pT, const int param);
	void setParam(const paramTypeCamera::paramFloat& pT, const float param);
	void setParam(const paramTypeKAYACoaXpress::Gain& pT);
	void setParam(const paramTypeKAYACoaXpress::paramInt& pT, const int param);
	void setParam(const paramTypeKAYACoaXpress::paramFloat& pT, const float param);
	void setParam(const paramTypeKAYACoaXpress::CaptureType& pt);
	void setParam(const paramTypeKAYACoaXpress::AcquisitionMode& pt, int id);

	int getParam(const paramTypeCamera::paramInt& pT);
	float getParam(const paramTypeCamera::paramFloat& pT);
	const char* getParam(const paramTypeKAYACoaXpress::Gain& pT);
	int getParam(const paramTypeKAYACoaXpress::paramInt& pT);
	float getParam(const paramTypeKAYACoaXpress::paramFloat& pT);
	const char* getParam(const paramTypeKAYACoaXpress::CaptureType& pT);
	const char* getParam(const paramTypeKAYACoaXpress::AcquisitionMode& pT);

	void kayacoaxpressMessage(std::string str);

	void debug_func();
	
};

