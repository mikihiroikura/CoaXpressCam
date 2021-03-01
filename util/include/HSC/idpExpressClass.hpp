#pragma once
///
/// @file		idpExpressClass.hpp
/// @brief		idpExpress カメラ
/// @author		Satoshi Tabata
/// @date		2017/6/9 作成
/// @date		2017/8/22 最終更新
/// @version	1.1.2
///
///
#include <HSC/Camera.hpp>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <IdpExpress/PDCLIB.h>
#pragma comment(lib, "PDCLIB.lib")
#define USE_SSE 1

/// Camera.hpp へ移動
/*
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
*/

class idpExpress : public Camera
{
private:
	static constexpr int CAM_WIDTH = 512;
	static constexpr int CAM_HEIGHT = 512;
	static constexpr float CAM_FPS = 1000;
	/** フレームレート[fps](50, 60, 125, 250, 500, 1000から選択可能) */
	//static constexpr int FRAME_PER_SEC = 1000;
	/** カメラヘッドID(2台接続時に1台だけ使用する場合, 0or1) */
	//static constexpr int USC_CAM_HEAD_ID = 0;
	/** ステレオ接続時にSSEを用いた高速化をするかどうか(1は使う, 0は使わない) */
	//static constexpr int USE_SSE = 1;

	/** 接続されたデバイスの検索結果 */
	static PDC_DETECT_NUM_INFO DetectNumInfo;
	static int instanceCount;

	/** デバイス番号 */
	unsigned long nDeviceNo;
	/** デバイスの状態 */
	unsigned long nStatus;
	/** 子デバイスの数 */
	unsigned long nCount;
	/** 子デバイス番号 */
	unsigned long ChildNo[PDC_MAX_DEVICE];
	/** 取得した画像の番号 */
	unsigned long nFrameNo;
	/** 一つ前の画像の番号 */
	unsigned long nOldFrameNo;
	/** 関数成否判定変数 */
	unsigned long nRet;
	/** エラー番号 */
	unsigned long nErrorCode;


	unsigned long maxWidth;
	unsigned long maxHeight;
	unsigned long maxFPS;

	/// トリガモードのON/OFF
	paramTypeIdpExpress::AcquisitionMode triggerMode;
	paramTypeIdpExpress::CameraHead useCameraHeadId;

	int showError(std::string str);
	int idpExpressMessage(std::string str);
	void parameter_all_print();

	void devideSSE(unsigned char *data, unsigned char *dataDst);
	int getFrame(unsigned char *data);
	void devideSSEStereo(unsigned char *data, unsigned char *dataDst1, unsigned char *dataDst2);
	int getFrameStereo(unsigned char *data1, unsigned char *data2);

	//int getFrameROI(cv::Mat image, int x_start, int x_end, int y_start, int y_end);
	//int getFrameStereoROI(cv::Mat image1, cv::Mat image2, int x_start, int x_end, int y_start, int y_end);
	//int getFrameStereoNumber(cv::Mat image1, cv::Mat image2);

public:


	idpExpress();

	virtual void connect(int id);
	virtual void disconnect();

	virtual void start();
	virtual void stop();

	// virtual void setParam(const paramType &pT, void* param);

	virtual void setParam(const paramTypeCamera::paramInt &pT, const int param);
	virtual void setParam(const paramTypeCamera::paramFloat &pT, const float param);

	virtual void setParam(const paramTypeIdpExpress::AcquisitionMode &pT);
	virtual void setParam(const paramTypeIdpExpress::CameraHead &pT);

	// virtual void getParam(const paramType &pT, void* param);

	virtual int getParam(const paramTypeCamera::paramInt &pT);
	virtual float getParam(const paramTypeCamera::paramFloat &pT);

	virtual void captureFrame(void* data);
	virtual void captureFrameStereo(void* dataL, void* dataR);

};

