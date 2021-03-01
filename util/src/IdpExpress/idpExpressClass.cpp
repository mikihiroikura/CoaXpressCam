///
/// @file		idpExpressClass.cpp
/// @brief		idpExpress カメラ
/// @author		Satoshi Tabata
/// @date		2017/8/22 作成
/// @date		2017/8/22 最終更新
/// @version	1.1.2
///
///
#include <HSC/idpExpressClass.hpp>


PDC_DETECT_NUM_INFO idpExpress::DetectNumInfo;
int idpExpress::instanceCount = 0;

idpExpress::idpExpress() : Camera(CAM_WIDTH, CAM_HEIGHT, CAM_FPS),
nOldFrameNo(-1), maxWidth(0), maxHeight(0), maxFPS(0),
triggerMode(paramTypeIdpExpress::AcquisitionMode::EnableAcquisitionFrameRate),
useCameraHeadId(paramTypeIdpExpress::CameraHead::CameraHead1)
{
	if (instanceCount == 0) {
		/** 初期化 */
		nRet = PDC_Init(&nErrorCode);
		showError("PDC_Init");

		/** 接続されたデバイスの検索 */
		nRet = PDC_DetectDevice(PDC_INTTYPE_PCI, NULL, 1, PDC_DETECT_NORMAL, &DetectNumInfo, &nErrorCode);
		showError("PDC_DetectDevice");

		//std::cout << "idpExpress Init. Cam: " << DetectNumInfo.m_nDeviceNum << std::endl;
		idpExpressMessage("idpExpress Init. Cam: " + std::to_string(DetectNumInfo.m_nDeviceNum));
	}
	instanceCount++;

};

///
/// @brief	カメラを接続する
///
void idpExpress::connect(int id) {

	/** 検索結果が0台の場合は終了 */
	if (DetectNumInfo.m_nDeviceNum <  id) {
		showError("Detect Number < " + std::to_string(id));
		return;
	}

	/** 検索結果がIDPExpressでない場合は終了 */
	if (DetectNumInfo.m_DetectInfo[id].m_nDeviceCode != PDC_DEVTYPE_IDPEXPRESS) {
		showError("Detect Device is not PDC_DEVTYPE_IDPEXPRESS");
		return;
	}

	/** デバイスのオープン */
	nRet = PDC_OpenDevice(&(DetectNumInfo.m_DetectInfo[id]), &nDeviceNo, &nErrorCode);
	showError("PDC_OpenDevice");

	/** 子デバイスの取得 */
	nRet = PDC_GetExistChildDeviceList(nDeviceNo, &nCount, ChildNo, &nErrorCode);
	showError("PDC_GetExistChildDeviceList");

	/** 現在の動作モードを取得 */
	nRet = PDC_GetStatus(nDeviceNo, &nStatus, &nErrorCode);
	showError("PDC_GetStatus");

	/** メモリ再生モードの場合はライブモードに切り替える */
	if (nStatus == PDC_STATUS_PLAYBACK) {
		nRet = PDC_SetStatus(nDeviceNo, PDC_STATUS_LIVE, &nErrorCode);
		showError("PDC_SetStatus");
	}

	nRet = PDC_GetMaxResolution(nDeviceNo, ChildNo[0], &maxWidth, &maxHeight, &nErrorCode);
	showError("PDC_GetMaxResolution");
	nRet = PDC_GetMaxFullResoRate(nDeviceNo, ChildNo[0], &maxFPS, &nErrorCode);
	showError("PDC_GetMaxFullResoRate");


	std::stringstream message;
	message << "PDC_DEVTYPE_IDPEXPRESS. max : " << maxFPS << " fps @ " << maxWidth << " x " << maxHeight;
	idpExpressMessage(message.str());

	for (int i = 0; i < (int)nCount; i++) {
		/** 撮影速度の設定 */
		nRet = PDC_SetRecordRate(nDeviceNo, ChildNo[i], static_cast<int>(fps), &nErrorCode);
		showError("PDC_SetRecordRate");

		/** 撮影解像度の設定 */
		nRet = PDC_SetResolution(nDeviceNo, ChildNo[i], width, height, &nErrorCode);
		showError("PDC_SetResolution");

		/** センサーゲイン増幅の設定 */
		if (gain >= 1.5) nRet = PDC_SetSensorGainMode(nDeviceNo, ChildNo[i], PDC_SENSOR_GAIN_X3, &nErrorCode);
		else nRet = PDC_SetSensorGainMode(nDeviceNo, ChildNo[i], PDC_SENSOR_GAIN_X1, &nErrorCode);
		showError("PDC_SetSensorGainMode");

		/** ベイヤー転送を有効化 */
		nRet = PDC_SetTransferOption(nDeviceNo, ChildNo[i], PDC_8BITSEL_8NORMAL, PDC_FUNCTION_ON, PDC_COLORDATA_INTERLEAVE_BGR, &nErrorCode);
		showError("PDC_SetTransferOption");

		/** ライブ画像取得可能か確認 */
		unsigned char *pBuf = (unsigned char*)malloc(width* height);
		nRet = PDC_GetLiveImageData(nDeviceNo, ChildNo[i], 8, pBuf, &nErrorCode);
		showError("PDC_GetLiveImageData");
		free(pBuf);
	}

}

///
/// @brief	カメラを切断する
///
void idpExpress::disconnect() {
	/** デバイスクローズ */
	instanceCount--;
	if (instanceCount <= 0) {
		nRet = PDC_CloseDevice(nDeviceNo, &nErrorCode);
		showError("PDC_CloseDevice");
	}
}

///
/// @brief	パラメタの設定をカメラに適用し, 撮像を開始する
///
void idpExpress::start() {

	for (int i = 0; i < (int)nCount; i++) {
		/** 撮影速度の設定 */
		nRet = PDC_SetRecordRate(nDeviceNo, ChildNo[i], static_cast<int>(fps), &nErrorCode);
		showError("PDC_SetRecordRate");

		/** 撮影解像度の設定 */
		nRet = PDC_SetResolution(nDeviceNo, ChildNo[i], width, height, &nErrorCode);
		showError("PDC_SetResolution");

		/** センサーゲイン増幅の設定 */
		if (gain >= 1.5) nRet = PDC_SetSensorGainMode(nDeviceNo, ChildNo[i], PDC_SENSOR_GAIN_X3, &nErrorCode);
		else nRet = PDC_SetSensorGainMode(nDeviceNo, ChildNo[i], PDC_SENSOR_GAIN_X1, &nErrorCode);
		showError("PDC_SetSensorGainMode");

	}

	/// トリガ同期に設定
	if (triggerMode == paramTypeIdpExpress::AcquisitionMode::TriggerMode) {
		nRet = PDC_SetExternalInMode(nDeviceNo, 1, PDC_EXT_IN_OTHERSSYNC_POSI, &nErrorCode);
		showError("PDC_SetExternalInMode");
	}
	/**
	* センタートリガに設定
	* @attention PDC_GetLiveImageAddress2を呼んだときににnFrameNoが更新されるようになる
	*/
	nRet = PDC_SetTriggerMode(nDeviceNo, PDC_TRIGGER_CENTER, 0, 0, 0, &nErrorCode);
	//nRet = PDC_SetTriggerMode(nDeviceNo, PDC_TRIGGER_RANDOM, 0, 1, 0, &nErrorCode);
	showError("PDC_SetTriggerMode");

	/** 録画準備状態に設定 */
	nRet = PDC_SetRecReady(nDeviceNo, &nErrorCode);
	showError("PDC_SetRecReady");


	/** エンドレス録画状態に */
	nRet = PDC_SetEndless(nDeviceNo, &nErrorCode);
	showError("PDC_SetEndless");

	parameter_all_print();

	/** カメラ起動まで待つ */
	//Sleep(1000);

}


///
/// @brief	撮像を終了する
///
void idpExpress::stop() {

}

///
/// @brief	パラメタを設定する (int 型) 
///
/// @param[in]	pT パラメタの種類	
/// @param[in]	param		設定する値
///
void idpExpress::setParam(const paramTypeCamera::paramInt &pT, const int param) {
	switch (pT) {
	case paramTypeCamera::paramInt::WIDTH:
		if (param <= maxWidth && 0 < param) {
			width = param;
		}
		else {
			//std::cout << " WIDTH :: Max以上の設定値を与えているのでMaxに設定します" << std::endl;
			idpExpressMessage(" WIDTH :: Max以上の設定値を与えているのでMaxに設定します");
			width = maxWidth;
		}
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		if (param <= maxHeight && 0 < param) {
			height = param;
		}
		else {
			//std::cout << " HEIGHT :: Max以上の設定値を与えているのでMaxに設定します" << std::endl;
			idpExpressMessage(" HEIGHT :: Max以上の設定値を与えているのでMaxに設定します");
			height = maxHeight;
		}
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
void idpExpress::setParam(const paramTypeCamera::paramFloat &pT, const float param) {
	switch (pT) {
	case paramTypeCamera::paramFloat::FPS:
		fps = param;
		break;
	case paramTypeCamera::paramFloat::GAIN:
		gain = param;
		break;
	default:
		std::cerr << "There are no implementation" << std::endl;
		assert(false);
		break;
	}
}



void idpExpress::setParam(const paramTypeIdpExpress::AcquisitionMode &pT) {
	triggerMode = pT;
}


void idpExpress::setParam(const paramTypeIdpExpress::CameraHead &pT) {
	useCameraHeadId = pT;
}

///
/// @brief	設定されているパラメタを取得する (int 型) 
///
/// @param[in]	pT パラメタの種類	
/// @param[in]	param	受け取る場所
///
/// map を使って定義することも検討
///
int idpExpress::getParam(const paramTypeCamera::paramInt &pT) {
	switch (pT) {
	case paramTypeCamera::paramInt::WIDTH:
		return width;
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		return height;
		break;
	default:
		std::cerr << "There are no implementation" << std::endl;
		assert(false);
		break;
	}
	return 0;
}

///
/// @brief	設定されているパラメタを取得する (float 型) 
///
/// @param[in]	pT パラメタの種類	
/// @param[in]	param	受け取る場所
///
float idpExpress::getParam(const paramTypeCamera::paramFloat &pT) {
	switch (pT) {
	case paramTypeCamera::paramFloat::FPS:
		return fps;
		break;
	case paramTypeCamera::paramFloat::GAIN:
		return gain;
		break;
	default:
		std::cerr << "There are no implementation" << std::endl;
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
void idpExpress::captureFrame(void* data) {
	while (1) {
		if (getFrame((unsigned char *)data) >= 0)break;
	}
}

///
/// @brief	画像データを取得する (ステレオ) 
///
/// @param[in]	dataL　	受け取る場所. 事前に確保すること. サイズはgetParamを使って取得すること.
/// @param[in]	dataR	受け取る場所. 事前に確保すること. サイズはgetParamを使って取得すること.
///
void idpExpress::captureFrameStereo(void* dataL, void* dataR) {
	while (1) {
		if (getFrameStereo((unsigned char *)dataL, (unsigned char *)dataR) >= 0)break;
	}
}


/**
* @brief エラー確認
*
* @param[in] str 関数名
* @return エラーがなければ0, あれば1
*/
int idpExpress::showError(std::string str) {
	std::stringstream message;
	message << str << " Error " << nErrorCode;
	if (nRet == PDC_FAILED) {
		//std::cout << message.str() << std::endl;
		idpExpressMessage(message.str());
		return -1;
	}
	return 0;
}

int  idpExpress::idpExpressMessage(std::string str) {
	HANDLE hStdout;
	WORD wAttributes;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	wAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hStdout, wAttributes);
	std::cout << "[idpExpress] " << str << std::endl;
	SetConsoleTextAttribute(hStdout, csbi.wAttributes);
	return 0;
}

void idpExpress::parameter_all_print()
{
	idpExpressMessage("Width : " + std::to_string(width));
	idpExpressMessage("Height : " + std::to_string(height));
	idpExpressMessage("FPS     : " + std::to_string(fps));
	idpExpressMessage("GAIN   : " + std::to_string(gain));
}


/**
* @brief SSEを利用した画像分割(単眼取得用)
*
* @param[in] data			画像情報を格納しているメモリアドレス
* @param[in, out] dataDst	取り込む画像(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)のデータ部である必要アリ)
*/
void idpExpress::devideSSE(unsigned char *data, unsigned char *dataDst) {
	__m128i a;
	int j = 0;
	int size = 2 * height * width;

	for (int i = 0; i<size; i += 32) {
		__m128i in1 = _mm_loadu_si128((__m128i*)(data + i));
		__m128i in2 = _mm_loadu_si128((__m128i*)(data + i + 16));

		a = _mm_unpacklo_epi64(in1, in2);

		j = i / 2;
		_mm_storeu_si128((__m128i*)(dataDst + j), a);
	}
}
/**
* @brief 単眼画像取得
*
* @param[in, out] image 取り込む画像(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)である必要アリ)
* @return 画像取得成功時は0，失敗時は-1
*/
int idpExpress::getFrame(unsigned char *data) {
	void  *baseAdress;
	unsigned char *pSrc, *pDst;
	if (PDC_GetLiveImageAddress2(nDeviceNo, ChildNo[0], &nFrameNo, &baseAdress, &nErrorCode) != PDC_SUCCEEDED)
		return -1;
	if (nOldFrameNo != -1 && nFrameNo == nOldFrameNo) return -1;
	nOldFrameNo = nFrameNo;

	if (nCount == 1) {
		pSrc = (unsigned char *)baseAdress + 8; // 8 for header, 8 for second camera
		pDst = data;
		memcpy(pDst, pSrc, width * height * sizeof(unsigned char));
	}
	else if (nCount == 2) {
		if (useCameraHeadId == paramTypeIdpExpress::CameraHead::CameraHead1) {
			pSrc = (unsigned char *)baseAdress + 8 + 8; // 8 for header, 8 for second camera
		}
		else {
			pSrc = (unsigned char *)baseAdress + 8; // 8 for header, 8 for second camera
		}
#if USE_SSE
		devideSSE(pSrc, data);
#else
		for (int y = 0; y < height; y++) {
			pDst = (unsigned char *)&(data[width * sizeof(unsigned char)*y]);
			for (int x = 0; x < width; x = x + 8) {
				memcpy(pDst, pSrc, 8 * sizeof(unsigned char));
				pDst += 8; pSrc += 16;
			}
		}
#endif
	}
	else {
		return -1;
	}

	return nFrameNo;
}


/**
* @brief SSEを利用した画像分割(ステレオ取得用)
*
* @param[in] data			画像情報を格納しているメモリアドレス
* @param[in, out] image1	取り込む画像(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)である必要アリ)
* @param[in, out] image2	取り込む画像(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)である必要アリ)
*/
void idpExpress::devideSSEStereo(unsigned char *data, unsigned char *dataDst1, unsigned char *dataDst2) {
	__m128i a, b;
	int j = 0;
	int size = 2 * height * width;

	for (int i = 0; i<size; i += 32) {
		__m128i in1 = _mm_loadu_si128((__m128i*)(data + i));
		__m128i in2 = _mm_loadu_si128((__m128i*)(data + i + 16));

		b = _mm_unpackhi_epi64(in1, in2);
		a = _mm_unpacklo_epi64(in1, in2);

		j = i / 2;
		_mm_storeu_si128((__m128i*)(dataDst1 + j), a);
		_mm_storeu_si128((__m128i*)(dataDst2 + j), b);
	}
}

/**
* @brief ROIを指定した単眼画像取得
*
* @param[in, out] image	取り込む画像(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)である必要アリ)
* @param[in] x_start		X方向開始位置
* @param[in] x_end		X方向終了位置
* @param[in] y_start	Y方向開始位置
* @param[in] y_end		Y方向終了位置
* @return 画像取得成功時は0，失敗時は-1
*/
/*
int idpExpress::getFrameROI(cv::Mat image, int x_start, int x_end, int y_start, int y_end) {
void  *baseAdress;
unsigned char *pSrc, *pDst;
if (PDC_GetLiveImageAddress2(nDeviceNo, ChildNo[0], &nFrameNo, &baseAdress, &nErrorCode) != PDC_SUCCEEDED)
return -1;
if (nOldFrameNo != 0 && nFrameNo == nOldFrameNo) return -1;
nOldFrameNo = nFrameNo;

// 接続カメラヘッド台数確認
if (nCount == 1) {
pSrc = (unsigned char *)baseAdress + 8; // 8 for header, 8 for second camera
for (int y = 0; y < height; y++) {
pDst = (unsigned char *)&(image.data[image.step*y]);
for (int x = 0; x < width; x = x + 8) {
if (x > x_start && x < x_end && y > y_start && y < y_end)
memcpy(pDst, pSrc, 8 * sizeof(unsigned char));
pDst += 8; pSrc += 8;
}
}
}
else if (nCount == 2) {
if (USC_CAM_HEAD_ID == 0)
pSrc = (unsigned char *)baseAdress + 8 + 8; // 8 for header, 8 for second camera
else
pSrc = (unsigned char *)baseAdress + 8; // 8 for header, 8 for second camera
for (int y = 0; y < height; y++) {
pDst = (unsigned char *)&(image.data[image.step*y]);
for (int x = 0; x < width; x = x + 8) {
if (x > x_start && x < x_end && y > y_start && y < y_end)
memcpy(pDst, pSrc, 8 * sizeof(unsigned char));
pDst += 8; pSrc += 16;
}
}
}
else {
return -1;
}

return 0;
}
*/

/**
* @brief ステレオ画像取得
*
* @param[in, out] image1 取り込む画像1(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)である必要アリ)
* @param[in, out] image2 取り込む画像2(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)である必要アリ)
* @return 画像取得成功時は0，失敗時は-1
*/
int idpExpress::getFrameStereo(unsigned char *data1, unsigned char *data2) {
	// 接続カメラヘッド数が2台か確認
	if (nCount != 2) return -1;

	void  *baseAdress;
	unsigned char *pSrc, *pDst1, *pDst2;
	if (PDC_GetLiveImageAddress2(nDeviceNo, ChildNo[0], &nFrameNo, &baseAdress, &nErrorCode) != PDC_SUCCEEDED)
		return -1;
	if (nOldFrameNo != -1 && nFrameNo == nOldFrameNo) return -1;
	nOldFrameNo = nFrameNo;

	pSrc = (unsigned char *)baseAdress + 8; // 8 for header
#if USE_SSE
	devideSSEStereo(pSrc, data2, data1);
#else
	for (int y = 0; y < height; y++) {
		pDst1 = (unsigned char *)&(data1[width * sizeof(unsigned char)*y]);
		pDst2 = (unsigned char *)&(data2[width * sizeof(unsigned char)
			*y]);
		for (int x = 0; x < width; x = x + 8) {
			memcpy(pDst2, pSrc, 8 * sizeof(unsigned char));
			pDst2 += 8; pSrc += 8;
			memcpy(pDst1, pSrc, 8 * sizeof(unsigned char));
			pDst1 += 8; pSrc += 8;
		}
	}
#endif
	return nFrameNo;
}

/**
* @brief ROIを指定したステレオ画像取得
*
* @param[in, out] image1 取り込む画像1(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)である必要アリ)
* @param[in, out] image2 取り込む画像2(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)である必要アリ)
* @param[in] x_start		X方向開始位置
* @param[in] x_end		X方向終了位置
* @param[in] y_start	Y方向開始位置
* @param[in] y_end		Y方向終了位置
* @return 画像取得成功時は0，失敗時は-1
*/
/*
int idpExpress::getFrameStereoROI(cv::Mat image1, cv::Mat image2, int x_start, int x_end, int y_start, int y_end) {
// 接続カメラヘッド数が2台か確認
if (nCount != 2) return -1;

void  *baseAdress;
unsigned char *pSrc, *pDst1, *pDst2;
if (PDC_GetLiveImageAddress2(nDeviceNo, ChildNo[0], &nFrameNo, &baseAdress, &nErrorCode) != PDC_SUCCEEDED)
return -1;
if (nOldFrameNo != 0 && nFrameNo == nOldFrameNo) return -1;
nOldFrameNo = nFrameNo;

pSrc = (unsigned char *)baseAdress + 8; // 8 for header
for (int y = 0; y < height; y++) {
pDst1 = (unsigned char *)&(image1.data[image1.step*y]);
pDst2 = (unsigned char *)&(image2.data[image2.step*y]);
for (int x = 0; x < width; x = x + 8) {
if (x > x_start && x < x_end && y > y_start && y < y_end)
memcpy(pDst2, pSrc, 8 * sizeof(unsigned char));
pDst2 += 8; pSrc += 8;
if (x > x_start && x < x_end && y > y_start && y < y_end)
memcpy(pDst1, pSrc, 8 * sizeof(unsigned char));
pDst1 += 8; pSrc += 8;
}
}

return 0;
}
*/

/**
* @brief ステレオ画像取得
*
* @param[in, out] image1 取り込む画像1(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)である必要アリ)
* @param[in, out] image2 取り込む画像2(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)である必要アリ)
* @return 画像取得成功時は0，失敗時は-1 -> フレームの差を示す
*/
/*
int idpExpress::getFrameStereoNumber(cv::Mat image1, cv::Mat image2) {
// 接続カメラヘッド数が2台か確認
if (nCount != 2) return -1;

void  *baseAdress;
unsigned char *pSrc, *pDst1, *pDst2;
if (PDC_GetLiveImageAddress2(nDeviceNo, ChildNo[0], &nFrameNo, &baseAdress, &nErrorCode) != PDC_SUCCEEDED)
return -1;
if (nOldFrameNo != 0 && nFrameNo == nOldFrameNo) return -1;
int diffFrameNo = nFrameNo - nOldFrameNo;
nOldFrameNo = nFrameNo;

pSrc = (unsigned char *)baseAdress + 8; // 8 for header
#if USE_SSE
devideSSEStereo(pSrc, image2, image1);
#else
for (int y = 0; y < height; y++) {
pDst1 = (unsigned char *)&(image1.data[image1.step*y]);
pDst2 = (unsigned char *)&(image2.data[image2.step*y]);
for (int x = 0; x < width; x = x + 8) {
memcpy(pDst2, pSrc, 8 * sizeof(unsigned char));
pDst2 += 8; pSrc += 8;
memcpy(pDst1, pSrc, 8 * sizeof(unsigned char));
pDst1 += 8; pSrc += 8;
}
}
#endif
return diffFrameNo;
return 0;
}
*/
