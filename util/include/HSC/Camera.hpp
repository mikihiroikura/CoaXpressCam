#pragma once
///
/// @file		Camera.hpp
/// @brief		�����J�����p�̒��ۃN���X
/// @author		Satoshi Tabata
/// @date		2017/7/27 �쐬
/// @date		2017/8/22 �ŏI�X�V
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

namespace paramTypeKAYACoaXpress
{
	enum class AcquisitionMode
	{
		EnableAcquisitionFrameRate = 0,
		TriggerMode = 1
	};

	enum class Gain
	{
		x1 = 0,
		x2 = 1,
		x4 = 2,
		x8 = 3
	};

	enum class paramInt
	{
		OffsetX = 4,
		OffsetY = 5,
		CycleBufferSize = 6
	};

	enum class paramFloat
	{
		ExposureTime = 7
	};
	enum class CaptureType
	{
		ColorGrab = 0,
		MonocroGrab = 1,
		BayerGrab = 2,
		Monocro8Grab = 3,
		ColorBGRGrab = 4,

		BayerBGGrab = 5,
		BayerGBGrab = 6,
		BayerRGGrab = 7,
		BayerGRGrab = 8,
	};
};


class Camera
{
public:
protected:
	/// ������. �J�����䐔�m�F��
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

	/// �ʃJ�����ڑ�?
	virtual void connect(int id) = 0;
	/// �J�����I������
	virtual void disconnect() = 0;

	/// �p�����^�ݒ�� �B���J�n or �B���ҋ@
	virtual void start() = 0;
	/// �B���I��(�����)
	virtual void stop() = 0;

	/// �p�����^�̐ݒ�
	//virtual void setParam(const paramType &pT, void* param) = 0;
	virtual void setParam(const paramTypeCamera::paramInt &pT, const int param) = 0;
	virtual void setParam(const paramTypeCamera::paramFloat &pT, const float param) = 0;
	
	/// �p�����^�̎擾
	//virtual void getParam(const paramType &pT, void* param) = 0;
	virtual int getParam(const paramTypeCamera::paramInt &pT) = 0;
	virtual float getParam(const paramTypeCamera::paramFloat &pT) = 0;

	/// �摜�̎擾(�P��)
	virtual void captureFrame(void* data) = 0;
	/// �摜�̎擾(�X�e���I) : Photron �p
	virtual void captureFrameStereo(void* dataL, void* dataR) {};

	/// idpExpress�p
	virtual void setParam(const paramTypeIdpExpress::AcquisitionMode &pT) {};
	virtual void setParam(const paramTypeIdpExpress::CameraHead &pT) {};

	/// Basler�p
	virtual void setParam(const paramTypeBasler::Param &pT, const float param) {};
	virtual void setParam(const paramTypeBasler::AcquisitionMode &pT) {};
	virtual void setParam(const paramTypeBasler::FastMode &pT) {};
	virtual void setParam(const paramTypeBasler::CaptureType &pT) {};
	virtual void setParam(const paramTypeBasler::GrabStrategy &pT) {};
	virtual float getParam(const paramTypeBasler::Param &pT){ return 0; };
	virtual void parameter_all_print(){};

	/// Ximea�p
	virtual void setParam(const paramTypeXimea::AcquisitionMode &pT) {};
	virtual void setParam(const paramTypeXimea::CaptureType &pT) {};

	/// Eosens�p
	virtual void setParam(const paramTypeEosens::paramInt &pT, const  int param) {};
	virtual void setParam(const paramTypeEosens::paramFloat &pT, const  float param) {};
	virtual int getParam(const paramTypeEosens::paramInt &pT) { return 0; };
	virtual float getParam(const paramTypeEosens::paramFloat &pT) { return 0; };

};
