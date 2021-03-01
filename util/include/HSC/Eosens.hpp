#pragma once
///
/// @file		Eosens.hpp
/// @brief		Eosens カメラ
/// @author		Yunpu Hu
/// @date		2017/08/09 作成
/// @date		2017/08/22 最終更新
/// @version	1.0.0
///
///
//Eosens is a 1280*768, 8 bit mono or raw format, 500fps camera.
//The parameters can be adjusted by a outside tool MCcontroltools, 
//which should be opened to set the tap type before use
#include <HSC/Camera.hpp>
#include <string>
#include <sstream>
#include <./Eosens/fgrab_struct.h>
#include <./Eosens/fgrab_prototyp.h>
#include <./Eosens/fgrab_define.h>
#pragma comment(lib, "./fglib5.lib")


/// Camera.hpp へ移動
/*
namespace paramTypeEosens
{
	enum class paramInt
	{
//		WIDTH = 0,
//		HEIGHT = 1,
		EXPOSURE = 2,
		MODE = 3,
//		GAIN = 4,
//		GAMMA = 5

	};

	enum class paramFloat
	{
		GAMMA = 5
	};
};
*/

class Eosens : public Camera
{
private:
	//const variables
	static constexpr int NMEMORY = 12; //---- キャプチャ用メモリ
	static constexpr int BOARD_NUM = 0; //---- ボード番号 0 or 1
	static constexpr paramTypeEosens::AcquisitionMode TRIGGER_MODE = paramTypeEosens::AcquisitionMode::EnableAcquisitionFrameRate;//---- キャプチャモード  freerun=0 trigger=1
	static constexpr int EXPOSURE_TIME = 1950; //---- 露光時間[us]

	static constexpr int CAMERA_WIDTH = 1280;
	static constexpr int CAMERA_HEIGHT = 1024;
	static constexpr int CAMERA_FPS = 500;
	static constexpr int MAXF = 200 * 10;
	static constexpr int NBUF = 12;	//number of buffer

	//private variables
	Fg_Struct *fg;
	int nCamPort;	//port numberof the camera
	int nBoard;		//board number
	int expo;		//exposure time (cannot be changed during free run)
	dma_mem *pMem0;	//dma pointer
	int lastPicNr;	//last acquired picture
	//int mode;		//freerun-0,trigger-1,
	paramTypeEosens::AcquisitionMode triggerMode;
	int nr_of_buffer ;			// Number of memory buffer
	float gamma = 1;
	int ErrorMessage(Fg_Struct *fg);
	int ErrorMessageWait(Fg_Struct *fg);

	std::string dllName;
	
public:
//	enum class paramType
//	{
//		WIDTH = 0,
//		HEIGHT = 1,
//		EXPOSURE = 2,
//		MODE = 3,
//		GAIN = 4,
//		GAMMA = 5
//
//	};

	Eosens() : Camera(CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_FPS) {
	//initilize the parameters
		//char *dllName = "./FullAreaGray8.dll";//or "DualAreaGray16.dll;
		 dllName = "./FullAreaGray8.dll";//or "DualAreaGray16.dll;
		 nCamPort=0;	//port A
		 nBoard=BOARD_NUM;		//board 0
		 expo=1950;		//exposure time 
		 triggerMode = TRIGGER_MODE;		//freerun-0,trigger-1
		 nr_of_buffer = NBUF;	//number of buffer
		 lastPicNr = 0;
		 int status;
		 if ((fg = Fg_Init(dllName.c_str(), nBoard)) == NULL) {
			 status = ErrorMessageWait(fg);

		 }
		 EosensMessage("Eosens Init.");
	};

	Eosens(const Eosens &) = delete;
	Eosens &operator=(const Eosens &) = delete;

	virtual void connect(int id);
	virtual void disconnect();

	virtual void start();

	virtual void stop() { Fg_stopAcquireEx(fg, nCamPort, pMem0, 0); };

	// virtual void setParam(const paramType &pT, void* param);

	virtual void setParam(const paramTypeCamera::paramInt &pT, const  int param);
	virtual void setParam(const paramTypeCamera::paramFloat &pT, const  float param);
	// virtual void getParam(const paramType &pT, void* param);

	// virtual void getParam(const paramType &pT, void* param);

	/// パラメタの取得
	//virtual void getParam(const paramType &pT, void* param) = 0;
	virtual int getParam(const paramTypeCamera::paramInt &pT);
	virtual float getParam(const paramTypeCamera::paramFloat &pT);// { int pai; getParam(pT, pai); param = pai; }


	virtual void setParam(const paramTypeEosens::paramInt &pT, const  int param);
	virtual void setParam(const paramTypeEosens::paramFloat &pT, const  float param);
	virtual void setParam(const paramTypeEosens::AcquisitionMode &pT);
	virtual int getParam(const paramTypeEosens::paramInt &pT);
	virtual float getParam(const paramTypeEosens::paramFloat &pT);

	
	//capture
	virtual void captureFrame(void* data);

	void  EosensMessage(std::string str);
};
