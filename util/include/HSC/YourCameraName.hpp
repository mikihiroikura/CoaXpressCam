#pragma once
///
/// @file		YourCameraName.hpp
/// @brief		YourCameraName �J����
/// @author		Satoshi Tabata
/// @date		2017/6/9 �쐬
/// @date		2017/8/22 �ŏI�X�V
/// @version	1.1.0
///
///

#include "Camera.hpp"

class YourCameraName : public Camera
{
private:
	static constexpr int CAM_WIDTH = 512;
	static constexpr int CAM_HEIGHT = 512;
	static constexpr float CAM_FPS = 1000;

	void originalFunc();

public:
	YourCameraName() : Camera(CAM_WIDTH, CAM_HEIGHT, CAM_FPS) {};
	YourCameraName(const YourCameraName &) = delete;
	YourCameraName &operator=(const YourCameraName &) = delete;

	virtual void connect();
	virtual void disconnect();

	virtual void start();
	virtual void stop();

	// virtual void setParam(const paramType &pT, void* param);

	virtual void setParam(const paramTypeCamera::paramInt &pT, const  int param);
	virtual void setParam(const paramTypeCamera::paramFloat &pT, const  float param);
	// virtual void getParam(const paramType &pT, void* param);

	virtual int getParam(const paramTypeCamera::paramInt &pT);
	virtual float getParam(const paramTypeCamera::paramFloat &pT);

	virtual void captureFrame(void* data);
	virtual void captureFrameStereo(void* dataL, void* dataR) ;

};

///
/// @brief	�J�������ƂɎg�p����֐�. �C�ӂɍ쐬����
///
void YourCameraName::originalFunc() {}

///
/// @brief	�J������ڑ�����
///
void YourCameraName::connect() {}

///
/// @brief	�J������ؒf����
///
void YourCameraName::disconnect(){}

///
/// @brief	�p�����^�̐ݒ���J�����ɓK�p��, �B�����J�n����
///
void YourCameraName::start() {}

///
/// @brief	�B�����I������
///
void YourCameraName::stop() {}

///
/// @brief	�p�����^��ݒ肷�� (int �^) 
///
/// @param[in]	pT �p�����^�̎��	
/// @param[in]	param		�ݒ肷��l
///
void YourCameraName::setParam(const paramTypeCamera::paramInt &pT, const int param){
	switch (pT) {
	case paramTypeCamera::paramInt::WIDTH:
		width = param;
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		height  = param;
		break;
	default:
		std::cerr << "There are no implementation" << std::endl;
		assert(false);
		break;
	}

}
///
/// @brief	�p�����^��ݒ肷�� (float �^) 
///
/// @param[in]	pT �p�����^�̎��	
/// @param[in]	param		�ݒ肷��l
///
void YourCameraName::setParam(const paramTypeCamera::paramFloat &pT, const float param){
	switch (pT) {
	case paramTypeCamera::paramFloat::FPS:
		fps = param;
		break;
	default:
		std::cerr << "There are no implementation" << std::endl;
		assert(false);
		break;
	}
}


///
/// @brief	�ݒ肳��Ă���p�����^���擾���� (int �^) 
///
/// @param[in]	pT �p�����^�̎��	
///
///
int YourCameraName::getParam(const paramTypeCamera::paramInt &pT) {
	switch (pT) {
	case paramTypeCamera::paramInt::WIDTH:
		return width;
		assert(width > 0);
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		return height;
		break;
	default:
		std::cerr << "There are no implementation" << std::endl;
		assert(false);
		break;
	}
}

///
/// @brief	�ݒ肳��Ă���p�����^���擾���� (float �^) 
///
/// @param[in]	pT �p�����^�̎��	
/// @param[in]	param	�󂯎��ꏊ
///
float YourCameraName::getParam(const paramTypeCamera::paramFloat &pT) {
	switch (pT) {
	case paramTypeCamera::paramFloat::FPS:
		return fps;
		break;
	default:
		std::cerr << "There are no implementation" << std::endl;
		assert(false);
		break;
	}
}



///
/// @brief	�摜�f�[�^���擾���� (�P��) 
///
/// @param[in]	data	�󂯎��ꏊ. ���O�Ɋm�ۂ��邱��. �T�C�Y��getParam���g���Ď擾���邱��.
///
void YourCameraName::captureFrame(void* data) {

}

///
/// @brief	�摜�f�[�^���擾���� (�X�e���I) 
///
/// @param[in]	dataL�@	�󂯎��ꏊ. ���O�Ɋm�ۂ��邱��. �T�C�Y��getParam���g���Ď擾���邱��.
/// @param[in]	dataR	�󂯎��ꏊ. ���O�Ɋm�ۂ��邱��. �T�C�Y��getParam���g���Ď擾���邱��.
///
void YourCameraName::captureFrameStereo(void* dataL, void* dataR) {

}
