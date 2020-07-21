///
/// @file		Ximea.cpp
/// @brief		Ximea カメラ
/// @author		
/// @date		2017/8/21 作成
/// @date		2017/8/22 最終更新
/// @version	1.1.2
///
///

#include <HSC/Ximea.hpp>

//#define XI_SET_TEST(input, message) {if (input != XI_OK) { XimeaMessage("error!! after xiSetParam: " message);}}

Ximea::Ximea() : Camera(CAM_WIDTH, CAM_HEIGHT, CAM_FPS), deviceName(""){
	memset(&xi_img, 0, sizeof(xi_img));
	xi_img.size = sizeof(XI_IMG);
	xiGetNumberDevices(&xi_camera_num);
	if (!xi_camera_num) { XimeaMessage("error!! after xiGetNumberDevices."); }
	XimeaMessage("Ximea Init. Cam: " + std::to_string(xi_camera_num));

};

void Ximea::connect(int id) {

	if (id < xi_camera_num)xi_status = xiOpenDevice(id, &xi_handle);
	if (xi_status != XI_OK) { XimeaMessage("error!! after xiOpenDevice."); }
	char deviceNameTmp[256] = "";
	xi_status = xiGetDeviceInfoString(id,XI_PRM_DEVICE_NAME, deviceNameTmp, 256);
	if (xi_status != XI_OK) { XimeaMessage("error!! after xiGetDeviceInfoString."); }
	deviceName = std::string(deviceNameTmp);
}


void Ximea::disconnect() {
	xiCloseDevice(xi_handle);
}

void Ximea::start() {
	//---- トリガ設定
	if (trigger_mode)
	{
		//---- トリガモードに設定
		xi_status = xiSetParamInt(xi_handle, XI_PRM_TRG_SOURCE, XI_TRG_EDGE_RISING);
		if (xi_status != XI_OK) { XimeaMessage("error!! after xiSetParamInt : " XI_PRM_TRG_SOURCE);}
	}
	else
	{
		//---- フリーランモードに設定
		xi_status = xiSetParamInt(xi_handle, XI_PRM_TRG_SOURCE, XI_TRG_OFF);
		if (xi_status != XI_OK) { XimeaMessage("error!! after xiSetParamInt : " XI_PRM_TRG_SOURCE); }
		//---- フレームレートを指定するとそれで撮像するように設定。これをしないとMAXの周波数で撮像する
		xi_status = xiSetParamInt(xi_handle, XI_PRM_ACQ_TIMING_MODE, XI_ACQ_TIMING_MODE_FRAME_RATE);
		if (xi_status != XI_OK) { XimeaMessage("error!! after xiSetParamInt : " XI_PRM_ACQ_TIMING_MODE); }
	}

	//---- 画像の種類を設定
	xi_status = xiSetParamInt(xi_handle, XI_PRM_IMAGE_DATA_FORMAT, img_type);
	if (xi_status != XI_OK) { XimeaMessage("error!! after xiSetParamInt : " XI_PRM_IMAGE_DATA_FORMAT); }

	//---- 解像度設定、フレームレート設定
	std::stringstream message;
	int width_max, height_max;
	float fps_max;
	xiGetParamInt(xi_handle, XI_PRM_WIDTH XI_PRM_INFO_MAX, &width_max);
	xiGetParamInt(xi_handle, XI_PRM_HEIGHT XI_PRM_INFO_MAX, &height_max);
	xiGetParamFloat(xi_handle, XI_PRM_FRAMERATE XI_PRM_INFO_MAX, &fps_max);

	int width_min, height_min;
	xiGetParamInt(xi_handle, XI_PRM_WIDTH XI_PRM_INFO_MIN, &width_min);
	xiGetParamInt(xi_handle, XI_PRM_HEIGHT XI_PRM_INFO_MIN, &height_min);
	//XimeaMessage("min: (  " + std::to_string(width_min) + ", " + std::to_string(height_min) + " ) ");
	//XimeaMessage("max: (  " + std::to_string(width_max) + ", " + std::to_string(height_max) + " ) ");
	message << deviceName << ". max : " << fps_max << " fps @ " << width_max << " x " << height_max;
	XimeaMessage(message.str());

	xi_status = xiSetParamInt(xi_handle, XI_PRM_WIDTH, std::min<int>(width_max, width));
	if (xi_status != XI_OK) { XimeaMessage("error!! after xiSetParamInt : " XI_PRM_WIDTH); }
	xi_status = xiSetParamInt(xi_handle, XI_PRM_HEIGHT, std::min<int>(height_max, height));
	if (xi_status != XI_OK) { XimeaMessage("error!! after xiSetParamInt : " XI_PRM_HEIGHT); }

	//---- 露光時間設定
	xi_status = xiSetParamInt(xi_handle, XI_PRM_EXPOSURE, (int)(1000000.0 / fps) - 100);
	if (xi_status != XI_OK) { XimeaMessage("error!! after xiSetParamInt : " XI_PRM_EXPOSURE); }


	if (!trigger_mode)
	{
		xi_status = xiSetParamFloat(xi_handle, XI_PRM_FRAMERATE, std::min<float>(fps_max, fps));
		if (xi_status != XI_OK) { XimeaMessage("error!! after xiSetParamFloat : " XI_PRM_FRAMERATE); }
	}

	//---- 各種パラメータを表示
	xiGetParamInt(xi_handle, XI_PRM_WIDTH, &width);
	xiGetParamInt(xi_handle, XI_PRM_HEIGHT, &height);
	xiGetParamFloat(xi_handle, XI_PRM_FRAMERATE, &fps);
	int depth;
	xiGetParamInt(xi_handle, XI_PRM_OUTPUT_DATA_BIT_DEPTH, &depth);
	XimeaMessage(" (  " + std::to_string(width) + ", " + std::to_string(height) + " ) @ " + std::to_string(fps) + " fps " + std::to_string(depth) + " bit ");

	//---- ゲイン設定
	float gain_tmp;
	xiSetParamFloat(xi_handle, XI_PRM_GAIN, gain);
	if (xi_status != XI_OK) { XimeaMessage("error!! after xiSetParamFloat : " XI_PRM_GAIN); }
	xiGetParamFloat(xi_handle, XI_PRM_GAIN, &gain_tmp);
	XimeaMessage("gain: " + std::to_string(gain_tmp));

	//---- 設定完了後、撮像開始
	xi_status = xiStartAcquisition(xi_handle);
	if (xi_status != XI_OK) { XimeaMessage("error!! after xiStartAcquisition."); }

	Sleep(1000);

}


//----------------------------------------------------------//
//-------------    パラメータ設定関数      -----------------//
//----------------------------------------------------------//
void Ximea::setParam(const paramTypeCamera::paramInt &pT, const int param) {
	switch (pT) {
	case paramTypeCamera::paramInt::WIDTH:
		width = param;
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		height = param;
		break;
	default:
		XimeaMessage("There are no implementation");

		assert(false);
		break;
	}

}
void Ximea::setParam(const paramTypeCamera::paramFloat &pT, const float param) {
	switch (pT) {
	case paramTypeCamera::paramFloat::FPS:
		fps = param;
		break;
	case paramTypeCamera::paramFloat::GAIN:
		gain = param;
		break;
	default:
		XimeaMessage("There are no implementation");
		assert(false);
		break;
	}
}
//---- トリガ設定
void Ximea::setParam(const paramTypeXimea::AcquisitionMode &pT)
{
	if (pT == paramTypeXimea::AcquisitionMode::TriggerMode)
	{
		trigger_mode = true;
	}
	else if (pT == paramTypeXimea::AcquisitionMode::EnableAcquisitionFrameRate)
	{
		trigger_mode = false;
	}
}
//---- 画像タイプ設定
void Ximea::setParam(const paramTypeXimea::CaptureType &pT)
{
	if (pT == paramTypeXimea::CaptureType::ColorGrab)
	{
		img_type = XI_RGB24;
	}
	else if (pT == paramTypeXimea::CaptureType::MonocroGrab)
	{
		img_type = XI_MONO8;
	}
	else if (pT == paramTypeXimea::CaptureType::BayerGrab)
	{
		img_type = XI_RAW8;
	}
}

//----------------------------------------------------------//
//-------------    パラメータ取得関数      -----------------//
//----------------------------------------------------------//
int Ximea::getParam(const paramTypeCamera::paramInt &pT) {
	switch (pT) {
	case paramTypeCamera::paramInt::WIDTH:
		return width;
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		return  height;
		break;
	default:
		XimeaMessage("There are no implementation");
		assert(false);
		break;
	}
	return 0;
}

float Ximea::getParam(const paramTypeCamera::paramFloat &pT) {
	switch (pT) {
	case paramTypeCamera::paramFloat::FPS:
		return fps;
		break;
	case paramTypeCamera::paramFloat::GAIN:
		return gain;
		break;
	default:
		XimeaMessage("There are no implementation");
		assert(false);
		break;
	}
	return 0;
}

//----------------------------------------------------------//
//------------------    画像取得関数     -------------------//
//----------------------------------------------------------//
void Ximea::captureFrame(void* data) {
	xiGetImage(xi_handle, 50000, &xi_img);
	if (img_type == XI_RGB24) {
		memcpy((unsigned char *)data, xi_img.bp, 3 * xi_img.width*xi_img.height);
	}
	else {
		memcpy((unsigned char *)data, xi_img.bp, xi_img.width*xi_img.height);
	}
}


void  Ximea::XimeaMessage(std::string str) {
	HANDLE hStdout;
	WORD wAttributes;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	wAttributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hStdout, wAttributes);
	std::cout << "[Ximea] " << str << std::endl;
	SetConsoleTextAttribute(hStdout, csbi.wAttributes);
	return;
}