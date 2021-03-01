#pragma once
///
/// @file		CameraUI.hpp
/// @brief		高速カメラの共通化クラス
/// @author		Satoshi Tabata
/// @date		2017/7/27 作成
/// @date		2017/9/13 最終更新
/// @version	1.1.3
///
///

//#define YOUR_CAMERA
//#define USE_EOSENS
//#define USE_XIMEA
//#define USE_IDPEXPRESS
//#define USE_BASLER


#ifdef _DEBUG
#define CAMERA_EXT "d.lib"
#else
#define CAMERA_EXT ".lib"
#endif

#include <HSC/Camera.hpp>

#ifdef USE_EOSENS
#include "Eosens.hpp"
#pragma comment(lib, "EosensLib" CAMERA_EXT)
#endif
#ifdef YOUR_CAMERA
#include "YourCameraName.hpp"
#endif
#ifdef USE_XIMEA
#include "Ximea.hpp"
#pragma comment(lib, "XimeaLib" CAMERA_EXT)
#endif
#ifdef USE_IDPEXPRESS
#include "idpExpressClass.hpp"
#pragma comment(lib, "idpExpressLib" CAMERA_EXT)
#endif
#ifdef USE_BASLER
#include "baslerClass.hpp"
#pragma comment(lib, "BaslerLib" CAMERA_EXT)
#endif

class CameraUI {
public:
	enum class cameraType
	{
		IDPEXPRESS = 0,
		BASLER = 1,
		EOSENS = 2,
		XIMEA = 3,
	};

private:
	Camera* cam;

	cameraType camtype;

public:

	CameraUI(const cameraType &ct);
	~CameraUI();

	void connect(int id);
	void start();
	void stop();
	void disconnect();
	void captureFrame(void* data);
	//void captureFrame(cv::Mat img);
	void captureFrameStereo(void* dataL, void* dataR);
	//void captureFrameStereo(cv::Mat imgL, cv::Mat imgR);

	void setParam(const paramTypeCamera::paramInt &pT, const int param);
	void setParam(const paramTypeCamera::paramFloat &pT, const float param);

	int getParam(const paramTypeCamera::paramInt &pT);
	float getParam(const paramTypeCamera::paramFloat &pT);

	/// idpExpress用
	void setParam(const paramTypeIdpExpress::AcquisitionMode &pT) ;
	void setParam(const paramTypeIdpExpress::CameraHead &pT) ;

	/// Basler用
	void setParam(const paramTypeBasler::Param &pT, const float param) ;
	void setParam(const paramTypeBasler::AcquisitionMode &pT) ;
	void setParam(const paramTypeBasler::FastMode &pT) ;
	void setParam(const paramTypeBasler::CaptureType &pT) ;
	void setParam(const paramTypeBasler::GrabStrategy &pT) ;
	float getParam(const paramTypeBasler::Param &pT) ;
	void parameter_all_print();

	/// Ximea用
	void setParam(const paramTypeXimea::AcquisitionMode &pT) ;
	void setParam(const paramTypeXimea::CaptureType &pT) ;

	/// Eosens用
	void setParam(const paramTypeEosens::paramInt &pT, const  int param) ;
	void setParam(const paramTypeEosens::paramFloat &pT, const  float param) ;
	int getParam(const paramTypeEosens::paramInt &pT) ;
	float getParam(const paramTypeEosens::paramFloat &pT) ;


	cameraType getCameraType();
};
