#pragma once
///
/// @file		Ximea.hpp
/// @brief		Ximea カメラ
/// @author		
/// @date		2017/8/21 作成
/// @date		2017/8/22 最終更新
/// @version	1.1.2
///
///
#include <HSC/Camera.hpp>

#include <windows.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <Ximea/xiApi.h>
#pragma comment(lib, "m3apiX64.lib")



/// Camera.hpp へ移動
/*
namespace paramTypeXimea
{
	enum class CaptureType
	{
		ColorGrab = 0,
		MonocroGrab = 1,
		BayerGrab = 2
	};
	enum class AcquisitionMode
	{
		EnableAcquisitionFrameRate = 0,
		TriggerMode = 1
	};
}
*/

class Ximea : public Camera
{
private:
	static constexpr int CAM_WIDTH = 648;
	static constexpr int CAM_HEIGHT = 488;
	static constexpr float CAM_FPS = 500;


	XI_IMG xi_img;
	HANDLE xi_handle;
	DWORD xi_camera_num;
	XI_RETURN xi_status;

	//---- トリガモード
	bool trigger_mode = false;

	//---- 画像の種類
	XI_IMG_FORMAT  img_type = XI_RAW8;
	//static constexpr XI_IMG_FORMAT  USE_IMG = XI_RAW8;

	std::string deviceName;

public:
	Ximea();

	Ximea(const Ximea &) = delete;
	Ximea &operator=(const Ximea &) = delete;
	virtual void connect(int id);
	virtual void disconnect();
	virtual void start();
	virtual void stop() {};

	//virtual void setParam(const paramType &pT, void* param);
	virtual void setParam(const paramTypeCamera::paramInt &pT, const  int param);
	virtual void setParam(const paramTypeCamera::paramFloat &pT, const  float param);

	//virtual void getParam(const paramType &pT, void* param);
	virtual int getParam(const paramTypeCamera::paramInt &pT);
	virtual float getParam(const paramTypeCamera::paramFloat &pT);

	virtual void setParam(const paramTypeXimea::AcquisitionMode &pT);
	virtual void setParam(const paramTypeXimea::CaptureType &pT);

	virtual void captureFrame(void* data);

	void  XimeaMessage(std::string str);
};

