///
/// @file		idpExpressClass.cpp
/// @brief		idpExpress �J����
/// @author		Satoshi Tabata
/// @date		2017/8/22 �쐬
/// @date		2017/8/22 �ŏI�X�V
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
		/** ������ */
		nRet = PDC_Init(&nErrorCode);
		showError("PDC_Init");

		/** �ڑ����ꂽ�f�o�C�X�̌��� */
		nRet = PDC_DetectDevice(PDC_INTTYPE_PCI, NULL, 1, PDC_DETECT_NORMAL, &DetectNumInfo, &nErrorCode);
		showError("PDC_DetectDevice");

		//std::cout << "idpExpress Init. Cam: " << DetectNumInfo.m_nDeviceNum << std::endl;
		idpExpressMessage("idpExpress Init. Cam: " + std::to_string(DetectNumInfo.m_nDeviceNum));
	}
	instanceCount++;

};

///
/// @brief	�J������ڑ�����
///
void idpExpress::connect(int id) {

	/** �������ʂ�0��̏ꍇ�͏I�� */
	if (DetectNumInfo.m_nDeviceNum <  id) {
		showError("Detect Number < " + std::to_string(id));
		return;
	}

	/** �������ʂ�IDPExpress�łȂ��ꍇ�͏I�� */
	if (DetectNumInfo.m_DetectInfo[id].m_nDeviceCode != PDC_DEVTYPE_IDPEXPRESS) {
		showError("Detect Device is not PDC_DEVTYPE_IDPEXPRESS");
		return;
	}

	/** �f�o�C�X�̃I�[�v�� */
	nRet = PDC_OpenDevice(&(DetectNumInfo.m_DetectInfo[id]), &nDeviceNo, &nErrorCode);
	showError("PDC_OpenDevice");

	/** �q�f�o�C�X�̎擾 */
	nRet = PDC_GetExistChildDeviceList(nDeviceNo, &nCount, ChildNo, &nErrorCode);
	showError("PDC_GetExistChildDeviceList");

	/** ���݂̓��샂�[�h���擾 */
	nRet = PDC_GetStatus(nDeviceNo, &nStatus, &nErrorCode);
	showError("PDC_GetStatus");

	/** �������Đ����[�h�̏ꍇ�̓��C�u���[�h�ɐ؂�ւ��� */
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
		/** �B�e���x�̐ݒ� */
		nRet = PDC_SetRecordRate(nDeviceNo, ChildNo[i], static_cast<int>(fps), &nErrorCode);
		showError("PDC_SetRecordRate");

		/** �B�e�𑜓x�̐ݒ� */
		nRet = PDC_SetResolution(nDeviceNo, ChildNo[i], width, height, &nErrorCode);
		showError("PDC_SetResolution");

		/** �Z���T�[�Q�C�������̐ݒ� */
		if (gain >= 1.5) nRet = PDC_SetSensorGainMode(nDeviceNo, ChildNo[i], PDC_SENSOR_GAIN_X3, &nErrorCode);
		else nRet = PDC_SetSensorGainMode(nDeviceNo, ChildNo[i], PDC_SENSOR_GAIN_X1, &nErrorCode);
		showError("PDC_SetSensorGainMode");

		/** �x�C���[�]����L���� */
		nRet = PDC_SetTransferOption(nDeviceNo, ChildNo[i], PDC_8BITSEL_8NORMAL, PDC_FUNCTION_ON, PDC_COLORDATA_INTERLEAVE_BGR, &nErrorCode);
		showError("PDC_SetTransferOption");

		/** ���C�u�摜�擾�\���m�F */
		unsigned char *pBuf = (unsigned char*)malloc(width* height);
		nRet = PDC_GetLiveImageData(nDeviceNo, ChildNo[i], 8, pBuf, &nErrorCode);
		showError("PDC_GetLiveImageData");
		free(pBuf);
	}

}

///
/// @brief	�J������ؒf����
///
void idpExpress::disconnect() {
	/** �f�o�C�X�N���[�Y */
	instanceCount--;
	if (instanceCount <= 0) {
		nRet = PDC_CloseDevice(nDeviceNo, &nErrorCode);
		showError("PDC_CloseDevice");
	}
}

///
/// @brief	�p�����^�̐ݒ���J�����ɓK�p��, �B�����J�n����
///
void idpExpress::start() {

	for (int i = 0; i < (int)nCount; i++) {
		/** �B�e���x�̐ݒ� */
		nRet = PDC_SetRecordRate(nDeviceNo, ChildNo[i], static_cast<int>(fps), &nErrorCode);
		showError("PDC_SetRecordRate");

		/** �B�e�𑜓x�̐ݒ� */
		nRet = PDC_SetResolution(nDeviceNo, ChildNo[i], width, height, &nErrorCode);
		showError("PDC_SetResolution");

		/** �Z���T�[�Q�C�������̐ݒ� */
		if (gain >= 1.5) nRet = PDC_SetSensorGainMode(nDeviceNo, ChildNo[i], PDC_SENSOR_GAIN_X3, &nErrorCode);
		else nRet = PDC_SetSensorGainMode(nDeviceNo, ChildNo[i], PDC_SENSOR_GAIN_X1, &nErrorCode);
		showError("PDC_SetSensorGainMode");

	}

	/// �g���K�����ɐݒ�
	if (triggerMode == paramTypeIdpExpress::AcquisitionMode::TriggerMode) {
		nRet = PDC_SetExternalInMode(nDeviceNo, 1, PDC_EXT_IN_OTHERSSYNC_POSI, &nErrorCode);
		showError("PDC_SetExternalInMode");
	}
	/**
	* �Z���^�[�g���K�ɐݒ�
	* @attention PDC_GetLiveImageAddress2���Ă񂾂Ƃ��ɂ�nFrameNo���X�V�����悤�ɂȂ�
	*/
	nRet = PDC_SetTriggerMode(nDeviceNo, PDC_TRIGGER_CENTER, 0, 0, 0, &nErrorCode);
	//nRet = PDC_SetTriggerMode(nDeviceNo, PDC_TRIGGER_RANDOM, 0, 1, 0, &nErrorCode);
	showError("PDC_SetTriggerMode");

	/** �^�揀����Ԃɐݒ� */
	nRet = PDC_SetRecReady(nDeviceNo, &nErrorCode);
	showError("PDC_SetRecReady");


	/** �G���h���X�^���Ԃ� */
	nRet = PDC_SetEndless(nDeviceNo, &nErrorCode);
	showError("PDC_SetEndless");

	parameter_all_print();

	/** �J�����N���܂ő҂� */
	//Sleep(1000);

}


///
/// @brief	�B�����I������
///
void idpExpress::stop() {

}

///
/// @brief	�p�����^��ݒ肷�� (int �^) 
///
/// @param[in]	pT �p�����^�̎��	
/// @param[in]	param		�ݒ肷��l
///
void idpExpress::setParam(const paramTypeCamera::paramInt &pT, const int param) {
	switch (pT) {
	case paramTypeCamera::paramInt::WIDTH:
		if (param <= maxWidth && 0 < param) {
			width = param;
		}
		else {
			//std::cout << " WIDTH :: Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�" << std::endl;
			idpExpressMessage(" WIDTH :: Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
			width = maxWidth;
		}
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		if (param <= maxHeight && 0 < param) {
			height = param;
		}
		else {
			//std::cout << " HEIGHT :: Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�" << std::endl;
			idpExpressMessage(" HEIGHT :: Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
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
/// @brief	�p�����^��ݒ肷�� (float �^) 
///
/// @param[in]	pT �p�����^�̎��	
/// @param[in]	param		�ݒ肷��l
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
/// @brief	�ݒ肳��Ă���p�����^���擾���� (int �^) 
///
/// @param[in]	pT �p�����^�̎��	
/// @param[in]	param	�󂯎��ꏊ
///
/// map ���g���Ē�`���邱�Ƃ�����
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
/// @brief	�ݒ肳��Ă���p�����^���擾���� (float �^) 
///
/// @param[in]	pT �p�����^�̎��	
/// @param[in]	param	�󂯎��ꏊ
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
/// @brief	�摜�f�[�^���擾���� (�P��) 
///
/// @param[in]	data	�󂯎��ꏊ. ���O�Ɋm�ۂ��邱��. �T�C�Y��getParam���g���Ď擾���邱��.
///
void idpExpress::captureFrame(void* data) {
	while (1) {
		if (getFrame((unsigned char *)data) >= 0)break;
	}
}

///
/// @brief	�摜�f�[�^���擾���� (�X�e���I) 
///
/// @param[in]	dataL�@	�󂯎��ꏊ. ���O�Ɋm�ۂ��邱��. �T�C�Y��getParam���g���Ď擾���邱��.
/// @param[in]	dataR	�󂯎��ꏊ. ���O�Ɋm�ۂ��邱��. �T�C�Y��getParam���g���Ď擾���邱��.
///
void idpExpress::captureFrameStereo(void* dataL, void* dataR) {
	while (1) {
		if (getFrameStereo((unsigned char *)dataL, (unsigned char *)dataR) >= 0)break;
	}
}


/**
* @brief �G���[�m�F
*
* @param[in] str �֐���
* @return �G���[���Ȃ����0, �����1
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
* @brief SSE�𗘗p�����摜����(�P��擾�p)
*
* @param[in] data			�摜�����i�[���Ă��郁�����A�h���X
* @param[in, out] dataDst	��荞�މ摜(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�̃f�[�^���ł���K�v�A��)
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
* @brief �P��摜�擾
*
* @param[in, out] image ��荞�މ摜(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�ł���K�v�A��)
* @return �摜�擾��������0�C���s����-1
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
* @brief SSE�𗘗p�����摜����(�X�e���I�擾�p)
*
* @param[in] data			�摜�����i�[���Ă��郁�����A�h���X
* @param[in, out] image1	��荞�މ摜(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�ł���K�v�A��)
* @param[in, out] image2	��荞�މ摜(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�ł���K�v�A��)
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
* @brief ROI���w�肵���P��摜�擾
*
* @param[in, out] image	��荞�މ摜(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�ł���K�v�A��)
* @param[in] x_start		X�����J�n�ʒu
* @param[in] x_end		X�����I���ʒu
* @param[in] y_start	Y�����J�n�ʒu
* @param[in] y_end		Y�����I���ʒu
* @return �摜�擾��������0�C���s����-1
*/
/*
int idpExpress::getFrameROI(cv::Mat image, int x_start, int x_end, int y_start, int y_end) {
void  *baseAdress;
unsigned char *pSrc, *pDst;
if (PDC_GetLiveImageAddress2(nDeviceNo, ChildNo[0], &nFrameNo, &baseAdress, &nErrorCode) != PDC_SUCCEEDED)
return -1;
if (nOldFrameNo != 0 && nFrameNo == nOldFrameNo) return -1;
nOldFrameNo = nFrameNo;

// �ڑ��J�����w�b�h�䐔�m�F
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
* @brief �X�e���I�摜�擾
*
* @param[in, out] image1 ��荞�މ摜1(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�ł���K�v�A��)
* @param[in, out] image2 ��荞�މ摜2(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�ł���K�v�A��)
* @return �摜�擾��������0�C���s����-1
*/
int idpExpress::getFrameStereo(unsigned char *data1, unsigned char *data2) {
	// �ڑ��J�����w�b�h����2�䂩�m�F
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
* @brief ROI���w�肵���X�e���I�摜�擾
*
* @param[in, out] image1 ��荞�މ摜1(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�ł���K�v�A��)
* @param[in, out] image2 ��荞�މ摜2(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�ł���K�v�A��)
* @param[in] x_start		X�����J�n�ʒu
* @param[in] x_end		X�����I���ʒu
* @param[in] y_start	Y�����J�n�ʒu
* @param[in] y_end		Y�����I���ʒu
* @return �摜�擾��������0�C���s����-1
*/
/*
int idpExpress::getFrameStereoROI(cv::Mat image1, cv::Mat image2, int x_start, int x_end, int y_start, int y_end) {
// �ڑ��J�����w�b�h����2�䂩�m�F
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
* @brief �X�e���I�摜�擾
*
* @param[in, out] image1 ��荞�މ摜1(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�ł���K�v�A��)
* @param[in, out] image2 ��荞�މ摜2(cv::Mat(CAMERA_HEIGHT, CAMERA_WIDTH, CV_8UC1)�ł���K�v�A��)
* @return �摜�擾��������0�C���s����-1 -> �t���[���̍�������
*/
/*
int idpExpress::getFrameStereoNumber(cv::Mat image1, cv::Mat image2) {
// �ڑ��J�����w�b�h����2�䂩�m�F
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
