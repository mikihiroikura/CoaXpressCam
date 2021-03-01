#pragma once
///
/// @file		YourCameraName.hpp
/// @brief		YourCameraName カメラ
/// @author		Satoshi Tabata
/// @date		2017/6/9 作成
/// @date		2017/8/22 最終更新
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
/// @brief	カメラごとに使用する関数. 任意に作成する
///
void YourCameraName::originalFunc() {}

///
/// @brief	カメラを接続する
///
void YourCameraName::connect() {}

///
/// @brief	カメラを切断する
///
void YourCameraName::disconnect(){}

///
/// @brief	パラメタの設定をカメラに適用し, 撮像を開始する
///
void YourCameraName::start() {}

///
/// @brief	撮像を終了する
///
void YourCameraName::stop() {}

///
/// @brief	パラメタを設定する (int 型) 
///
/// @param[in]	pT パラメタの種類	
/// @param[in]	param		設定する値
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
/// @brief	パラメタを設定する (float 型) 
///
/// @param[in]	pT パラメタの種類	
/// @param[in]	param		設定する値
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
/// @brief	設定されているパラメタを取得する (int 型) 
///
/// @param[in]	pT パラメタの種類	
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
/// @brief	設定されているパラメタを取得する (float 型) 
///
/// @param[in]	pT パラメタの種類	
/// @param[in]	param	受け取る場所
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
/// @brief	画像データを取得する (単眼) 
///
/// @param[in]	data	受け取る場所. 事前に確保すること. サイズはgetParamを使って取得すること.
///
void YourCameraName::captureFrame(void* data) {

}

///
/// @brief	画像データを取得する (ステレオ) 
///
/// @param[in]	dataL　	受け取る場所. 事前に確保すること. サイズはgetParamを使って取得すること.
/// @param[in]	dataR	受け取る場所. 事前に確保すること. サイズはgetParamを使って取得すること.
///
void YourCameraName::captureFrameStereo(void* dataL, void* dataR) {

}
