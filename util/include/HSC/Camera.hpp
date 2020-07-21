#pragma once
///
/// @file		Camera.hpp
/// @brief		高速カメラ用の抽象クラス
/// @author		Satoshi Tabata
/// @date		2017/7/27 作成
/// @date		2017/8/22 最終更新
/// @version	1.1.3
///
///

#include <iostream>
#include <cassert>
#include <type_traits>
#include <iomanip>

namespace paramTypeCamera
{
	enum class paramInt
	{
		WIDTH = 0,
		HEIGHT = 1,
		//FPS = 2,
		//GAIN = 3,
	};

	enum class paramFloat
	{
		//WIDTH = 0,
		//HEIGHT = 1,
		FPS = 2,
		GAIN = 3,
	};
}

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


namespace paramTypeBasler
{
	enum class Param
	{
		ExposureTime = 0,
		TriggerDelay = 1,
	};
	enum class FastMode
	{
		SensorReadoutModeFast = 0,
		SensorReadoutModeNormal = 1
	};
	enum class AcquisitionMode
	{
		EnableAcquisitionFrameRate = 0,
		TriggerMode = 1
	};
	enum class CaptureType
	{
		ColorGrab = 0,
		MonocroGrab = 1,
		BayerGrab = 2,
		Monocro10Grab = 3,
		ColorBGRGrab = 4,

		BayerBGGrab = 5,
		BayerGBGrab = 6,
		BayerRGGrab = 7,
		BayerGRGrab = 8,
	};
	enum class GrabStrategy
	{
		OneByOne = 0,
		LatestOnlyFrame = 1
	};
}

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

namespace paramTypeEosens
{
	enum class AcquisitionMode
	{
		EnableAcquisitionFrameRate = 0,
		TriggerMode = 1
	};

	enum class paramInt
	{
		EXPOSURE = 2,
		MODE = 3,

	};

	enum class paramFloat
	{
		GAMMA = 5
	};
};


class Camera
{
public:
protected:
	/// 初期化. カメラ台数確認等
	Camera() = delete;
	Camera(int width, int height, int fps) : width(width), height(height), fps(fps) {}
	int width;
	int height;
	float fps;
	float gain;

public:
	virtual ~Camera() {}

	Camera(const Camera &) = delete;
	Camera &operator=(const Camera &) = delete;

	/// 個別カメラ接続?
	virtual void connect(int id) = 0;
	/// カメラ終了処理
	virtual void disconnect() = 0;

	/// パラメタ設定後 撮像開始 or 撮像待機
	virtual void start() = 0;
	/// 撮像終了(あれば)
	virtual void stop() = 0;

	/// パラメタの設定
	//virtual void setParam(const paramType &pT, void* param) = 0;
	virtual void setParam(const paramTypeCamera::paramInt &pT, const int param) = 0;
	virtual void setParam(const paramTypeCamera::paramFloat &pT, const float param) = 0;
	
	/// パラメタの取得
	//virtual void getParam(const paramType &pT, void* param) = 0;
	virtual int getParam(const paramTypeCamera::paramInt &pT) = 0;
	virtual float getParam(const paramTypeCamera::paramFloat &pT) = 0;

	/// 画像の取得(単眼)
	virtual void captureFrame(void* data) = 0;
	/// 画像の取得(ステレオ) : Photron 用
	virtual void captureFrameStereo(void* dataL, void* dataR) {};

	/// idpExpress用
	virtual void setParam(const paramTypeIdpExpress::AcquisitionMode &pT) {};
	virtual void setParam(const paramTypeIdpExpress::CameraHead &pT) {};

	/// Basler用
	virtual void setParam(const paramTypeBasler::Param &pT, const float param) {};
	virtual void setParam(const paramTypeBasler::AcquisitionMode &pT) {};
	virtual void setParam(const paramTypeBasler::FastMode &pT) {};
	virtual void setParam(const paramTypeBasler::CaptureType &pT) {};
	virtual void setParam(const paramTypeBasler::GrabStrategy &pT) {};
	virtual float getParam(const paramTypeBasler::Param &pT){ return 0; };
	virtual void parameter_all_print(){};

	/// Ximea用
	virtual void setParam(const paramTypeXimea::AcquisitionMode &pT) {};
	virtual void setParam(const paramTypeXimea::CaptureType &pT) {};

	/// Eosens用
	virtual void setParam(const paramTypeEosens::paramInt &pT, const  int param) {};
	virtual void setParam(const paramTypeEosens::paramFloat &pT, const  float param) {};
	virtual int getParam(const paramTypeEosens::paramInt &pT) { return 0; };
	virtual float getParam(const paramTypeEosens::paramFloat &pT) { return 0; };

};
