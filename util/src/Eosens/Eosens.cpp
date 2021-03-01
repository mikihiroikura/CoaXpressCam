///
/// @file		Eosens.cpp
/// @brief		Eosens カメラ
/// @author		Yunpu Hu
/// @date		2017/08/22 作成
/// @date		2017/08/22 最終更新
/// @version	1.0.0
///
///

#include <HSC/Eosens.hpp>

#define FG_TRIGGER_LEGACY_MODE 0


///
/// @brief	カメラを接続する
///
void Eosens::connect(int id) {

	//camera
	//char *dllName = "./FullAreaGray8.dll";//or "DualAreaGray16.dll;
	int status = 1;
	double		timebuf[MAXF];
	int			imgnum[MAXF];
	//	BYTE		*imgb,*imgb_t;
	int			frmptr = 0;
	int i;
	lastPicNr = 1;

	int boardType = Fg_getBoardType(nBoard);

	/***********************************
	*        Camera Initialization
	************************************/
	// Initialization of the microEnable frame grabber

#ifdef OFFLINE
	EosensMessage("Init Grabber ok");
#endif
	// Setting the image size
	if (Fg_setParameter(fg, FG_WIDTH, &width, nCamPort) < 0) {
		status = ErrorMessageWait(fg);

	}
	if (Fg_setParameter(fg, FG_HEIGHT, &height, nCamPort) < 0) {
		status = ErrorMessageWait(fg);

	}

	int bitAlignment = FG_LEFT_ALIGNED;
	if (Fg_setParameter(fg, FG_BITALIGNMENT, &bitAlignment, nCamPort) < 0) {
		status = ErrorMessageWait(fg);

	}
	assert(status >= 0);
#ifdef OFFLINE
	EosensMessage("Set Image Size on port " + std::to_string(nCamPort) + " (w: " + std::to_string(width) + ",h: " + std::to_string(height) + ") ok");
#endif
	// Setting the format and cl interface
	int nTapFormat = FG_CL_8BIT_FULL_8;
	int nUseDval = FG_YES;
	int nPixelformat = FG_GRAY;
	double gain = 1.1;
	if (Fg_setParameter(fg, FG_CAMERA_LINK_CAMTYP, &nTapFormat, nCamPort)<0) { status = ErrorMessageWait(fg); /* */ }
	if (Fg_setParameter(fg, FG_USEDVAL, &nUseDval, nCamPort)<0) { status = ErrorMessageWait(fg); /* */ }
	if (Fg_setParameter(fg, FG_FORMAT, &nPixelformat, nCamPort)<0) { status = ErrorMessageWait(fg); /* */ }
	if (Fg_setParameter(fg, FG_KNEE_LUT_SCALE, &gain, nCamPort)<0) { status = ErrorMessageWait(fg); /* */ }

	//setParam(Eosens::paramType::EXPOSURE, 5000);
	//if (Fg_setParameter(fg, FG_FRAMERATE, &fps, nCamPort)<0) { status = ErrorMessageWait(fg); /* */ }
	assert(status >= 0);

	// Memory allocation
	int format;
	Fg_getParameter(fg, FG_FORMAT, &format, nCamPort);
	int bytesPerPixel = 1;
	switch (format) {
	case FG_GRAY:	bytesPerPixel = 1; break;
	case FG_GRAY16:	bytesPerPixel = 2; break;
	case FG_COL24:	bytesPerPixel = 3; break;
	case FG_COL32:	bytesPerPixel = 4; break;
	case FG_COL30:	bytesPerPixel = 5; break;
	case FG_COL48:	bytesPerPixel = 6; break;
	}
	size_t totalBufSize = width*height*nr_of_buffer*bytesPerPixel;
	//int totalBufSize = width*height*nr_of_buffer*bytesPerPixel;
	if ((pMem0 = Fg_AllocMemEx(fg, totalBufSize, nr_of_buffer)) == NULL) {
		status = ErrorMessageWait(fg);
		// 
	}
	else {
#ifdef OFFLINE
		EosensMessage(std::to_string(nr_of_buffer) + " framebuffer allocated for port " + std::to_string(nCamPort) + " ok");
#endif
	}

	// check for advanced applets, which have a newly designed trigger, anyway set it to camptibility mode
	bool isAdvancedApplets = false; // 
	if (strstr(dllName.c_str(), "Acq_")) {
		isAdvancedApplets = true;
	}
	if (isAdvancedApplets) {
		// set advanced applet into compatibility mode to use current AcquisitionApplets in order to use the same
		// trigger parameters. Otherwise check applet documentation in order to use the advanced trigger features
		uint32_t on = FG_ON;
		status = Fg_setParameterWithType(fg, FG_TRIGGER_LEGACY_MODE, &on, nCamPort, FG_PARAM_TYPE_UINT32_T);
		if (status != FG_OK) {
			ErrorMessageWait(fg);
			// 
		}
	}
	assert(status >= 0);
	// setting trigger or freerun
	if (triggerMode == paramTypeEosens::AcquisitionMode::EnableAcquisitionFrameRate) {
		//free run mode
		int		nTriggerMode = FREE_RUN;//ASYNC_TRIGGER;	
		if (Fg_setParameter(fg, FG_TRIGGERMODE, &nTriggerMode, nCamPort)<0) { status = ErrorMessageWait(fg); }
		//	if (Fg_setParameter(fg, FG_EXPOSURE, &expo, nCamPort) < 0) { status = ErrorMessageWait(fg); }

		assert(status >= 0);
	}
	else if (triggerMode == paramTypeEosens::AcquisitionMode::TriggerMode) {
		//trigger mode
		int		nTriggerMode = ASYNC_TRIGGER;
		int	    nExposureInMicroSec = expo;
		int		nTrgSource = 0;
		int		ExsyncEnable = 1;
		if (Fg_setParameter(fg, FG_TRIGGERMODE, &nTriggerMode, nCamPort)<0) { status = ErrorMessageWait(fg); }
		if (Fg_setParameter(fg, FG_EXSYNCON, &ExsyncEnable, nCamPort)<0) { status = ErrorMessageWait(fg);/*  */ }
		if (Fg_setParameter(fg, FG_EXPOSURE, &nExposureInMicroSec, nCamPort)<0) { status = ErrorMessageWait(fg);/*  */ }
		if (Fg_setParameter(fg, FG_TRIGGERINSRC, &nTrgSource, nCamPort)<0) { status = ErrorMessageWait(fg);/*  */ }
	}
	else {
		EosensMessage("mode must be 0 or 1");
	}


	for (i = 0; i<MAXF; i++) {
		timebuf[i] = 0;		imgnum[i] = 0;
	}


	assert(status >= 0);
}

///
/// @brief	カメラを切断する
///
void Eosens::disconnect() {

	Fg_FreeMemEx(fg, pMem0);
	Fg_FreeGrabber(fg);
	return;
}

///
/// @brief	パラメタの設定をカメラに適用し, 撮像を開始する
///
void Eosens::start() {
	int status = 1;
	if ((Fg_AcquireEx(fg, nCamPort, GRAB_INFINITE, ACQ_STANDARD, pMem0)) < 0) {
		status = ErrorMessageWait(fg);
		// 
	}
	assert(status >= 0);
}

///
/// @brief	パラメタを設定する (int 型) 
///
/// @param[in]	pT パラメタの種類	
/// @param[in]	param		設定する値
void Eosens::setParam(const paramTypeCamera::paramInt &pT, const int param)
{
	assert(param >= 0);
	int status;

	switch (pT) {
	case paramTypeCamera::paramInt::WIDTH:
		width = param;
		if (Fg_setParameter(fg, FG_WIDTH, &width, nCamPort) < 0) {
			status = ErrorMessageWait(fg);
			assert(status >= 0);
		}
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		height = param;
		if (Fg_setParameter(fg, FG_HEIGHT, &height, nCamPort) < 0) {
			status = ErrorMessageWait(fg);
			assert(status >= 0);
		}
		break;
	default:
		EosensMessage("Not defined");
		break;
	}
}
void Eosens::setParam(const paramTypeEosens::paramInt &pT, const int param) {
	assert(param >= 0);
	int status;
	switch (pT) {
	case paramTypeEosens::paramInt::EXPOSURE:
		expo = param;
		if (Fg_setParameter(fg, FG_EXPOSURE, &expo, nCamPort)<0) { status = ErrorMessageWait(fg);/* return status;*/ }
		assert(status >= 0);
		break;

	default:
		EosensMessage("There is no implementation");
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
void Eosens::setParam(const paramTypeEosens::paramFloat &pT, const float param) {
	int status = 1;
	double paramd = (double)param;
	switch (pT) {
	case paramTypeEosens::paramFloat::GAMMA:
		gamma = param;
		if (Fg_setParameter(fg, FG_KNEE_LUT_GAMMA, &paramd, nCamPort)<0) { status = ErrorMessageWait(fg);/* return status;*/ }
		break;
	default:
		EosensMessage("Type Error");
		assert(false);
		break;
	}
}
void Eosens::setParam(const paramTypeCamera::paramFloat &pT, const float param) {
	int status = 1;
	double paramd = (double)param;
	switch (pT) {
	case paramTypeCamera::paramFloat::GAIN:
		gain = param;
		if (Fg_setParameter(fg, FG_KNEE_LUT_SCALE, &paramd, nCamPort)<0) { status = ErrorMessageWait(fg);/* return status;*/ }
		break;
	case paramTypeCamera::paramFloat::FPS:
		break;
	default:
		EosensMessage("Type Error");
		assert(false);
		break;
	}
}

void Eosens::setParam(const paramTypeEosens::AcquisitionMode &pT){
	int status = 1;
	triggerMode = pT;
	int		nTriggerMode = FREE_RUN;//ASYNC_TRIGGER;
	switch (pT) {
	case  paramTypeEosens::AcquisitionMode::EnableAcquisitionFrameRate:
		//free run mode	
		nTriggerMode = FREE_RUN;
		if (Fg_setParameter(fg, FG_TRIGGERMODE, &nTriggerMode, nCamPort) < 0) { status = ErrorMessageWait(fg); }
		if (Fg_setParameter(fg, FG_EXPOSURE, &expo, nCamPort) < 0) { status = ErrorMessageWait(fg); }
		break;
	case  paramTypeEosens::AcquisitionMode::TriggerMode:
		//trigger mode
		nTriggerMode = ASYNC_TRIGGER;
		int	    nExposureInMicroSec = expo;
		int		nTrgSource = 0;
		int		ExsyncEnable = 1;

		if (Fg_setParameter(fg, FG_TRIGGERMODE, &nTriggerMode, nCamPort) < 0) { status = ErrorMessageWait(fg); }
		if (Fg_setParameter(fg, FG_EXSYNCON, &ExsyncEnable, nCamPort) < 0) { status = ErrorMessageWait(fg);/* return status;*/ }
		if (Fg_setParameter(fg, FG_EXPOSURE, &nExposureInMicroSec, nCamPort) < 0) { status = ErrorMessageWait(fg);/* return status;*/ }
		if (Fg_setParameter(fg, FG_TRIGGERINSRC, &nTrgSource, nCamPort) < 0) { status = ErrorMessageWait(fg);/* return status;*/ }

		break;
	}
}

///
/// @brief	設定されているパラメタを取得する (int 型) 
///
/// @param[in]	pT パラメタの種類	
/// @param[in]	param	受け取る場所
///
/// map を使って定義することも検討
///
int Eosens::getParam(const paramTypeCamera::paramInt &pT)
{
	switch (pT) {
	case paramTypeCamera::paramInt::WIDTH:
		getParam(paramTypeCamera::paramInt::WIDTH);
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		getParam(paramTypeCamera::paramInt::HEIGHT);
		break;
	default:
		EosensMessage("Not supported");
		break;
	}
	return 0;
}

int Eosens::getParam(const paramTypeEosens::paramInt &pT) {
	switch (pT) {
	case paramTypeEosens::paramInt::EXPOSURE:
		return expo;
		break;
	default:
		EosensMessage("Type Error");
		assert(false);
		break;
	}
	return 0;
}


float Eosens::getParam(const paramTypeCamera::paramFloat &pT) {
	switch (pT) {
	case paramTypeCamera::paramFloat::GAIN:
		return gain;
		break;
	default:
		EosensMessage("Type Error");
		assert(false);
		break;
	}
	return 0;
}

float Eosens::getParam(const paramTypeEosens::paramFloat &pT) {
	switch (pT) {
	case paramTypeEosens::paramFloat::GAMMA:
		return gamma;
		break;
	default:
		EosensMessage("Type Error");
		assert(false);
		break;
	}
	return 0;
}

///
/// @brief	画像データを取得する (単眼) 
///
/// @param[in]	data	受け取る場所. 事前に確保すること. サイズはgetParamを使って取得すること.
///
void Eosens::captureFrame(void* data) {
	//BYTE* imgb;
	int status = 1;
	lastPicNr = Fg_getLastPicNumberBlockingEx(fg, lastPicNr + 1, nCamPort, 10, pMem0);
	if (lastPicNr <0) {
		status = ErrorMessageWait(fg);
		Fg_stopAcquireEx(fg, nCamPort, pMem0, 0);
		Fg_FreeMem(fg, nCamPort);
		Fg_FreeGrabber(fg);
	}
	memcpy(data, Fg_getImagePtrEx(fg, lastPicNr, 0, pMem0), width*height * sizeof(BYTE));
	//data = (void*)Fg_getImagePtrEx(fg, lastPicNr, 0, pMem0);
	return;// imgb = (BYTE *)Fg_getImagePtrEx(fg, lastPicNr, 0, pMem0);

}


int Eosens::ErrorMessage(Fg_Struct *fg)
{
	int			error = Fg_getLastErrorNumber(fg);
	const char*	err_str = Fg_getLastErrorDescription(fg);

	EosensMessage("Error: " + std::to_string(error) + " : " + std::string(err_str));

	return error;
}

int Eosens::ErrorMessageWait(Fg_Struct *fg)
{
	int			error = Fg_getLastErrorNumber(fg);
	const char*	err_str = Fg_getLastErrorDescription(fg);

	if (err_str == "") {
		EosensMessage("Error: " + std::to_string(error));
	}
	else {
		EosensMessage("Error: " + std::to_string(error) + " : " + std::string(err_str));
	}
	EosensMessage(" ... press any key to continue");
	//_getche();
	return error;
}


void  Eosens::EosensMessage(std::string str) {
	HANDLE hStdout;
	WORD wAttributes;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hStdout, wAttributes);
	std::cout << "[Eosens] " << str << std::endl;
	SetConsoleTextAttribute(hStdout, csbi.wAttributes);
	return;
}