#pragma once
///
/// @file		CameraFunc.hpp
/// @brief		高速カメラの共通化関数
/// @author		Satoshi Tabata
/// @date		2017/8/22 作成
/// @date		2017/9/13 最終更新
/// @version	1.0.1
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


enum class cameraType
{
	IDPEXPRESS = 0,
	BASLER = 1,
	EOSENS = 2,
	XIMEA = 3,
};

Camera*  getCamera(const cameraType &ct);
