///
/// @file		baslerClass.cpp
/// @brief		baslerClass basler�J����
/// @author		Hikaru Amano
/// @date		2017/8/10 �쐬
/// @date		2017/8/22 �ŏI�X�V
/// @version	1.0.1
///
#include <HSC/baslerClass.hpp>

unsigned int basler::camera_count = 0;
unsigned int basler::camera_number = 0;
DeviceInfoList_t basler::devices;

/// ������. �J�����䐔�m�F��
basler::basler() : Camera(CAM_WIDTH, CAM_HEIGHT, CAM_FPS),
grab_strategy(GrabStrategy_OneByOne),
frameNumber(0u),
deviceName("")
{
	PylonInitialize();
	CTlFactory& tlFactoryTmp = CTlFactory::GetInstance();
	tlFactory = &tlFactoryTmp;
	if (camera_number == 0)
	{
		camera_number = tlFactory->EnumerateDevices(devices);
		if (camera_number == 0) std::runtime_error("No camera present.");
		baslerMessage("Basler Init Cam : " + std::to_string(camera_number));
	}
	camera_count++;
	formatConverter.OutputPixelFormat = PixelType_RGB8packed;

	// �f�o�b�O�p
	debug_flag.trigger_delay = 0.0f;
	debug_flag.acquisition_mode = static_cast< int >(paramTypeBasler::AcquisitionMode::EnableAcquisitionFrameRate);
	debug_flag.fast_mode = static_cast< int >(paramTypeBasler::FastMode::SensorReadoutModeNormal);
	debug_flag.grab_strategy = static_cast< int >(paramTypeBasler::GrabStrategy::OneByOne);
	debug_flag.cap_type = static_cast< int >(paramTypeBasler::CaptureType::ColorGrab);
}

basler::~basler()
{
	PylonTerminate();
}

void basler::parameter_all_print()
{
	baslerMessage(" Width : " + std::to_string(width));
	baslerMessage(" Height : " + std::to_string(height));
	baslerMessage(" fps : " + std::to_string(fps));
	baslerMessage(" gain : " + std::to_string(gain));
	baslerMessage(" Exposure time : " + std::to_string(debug_flag.exposure_time));
	baslerMessage(" Trigger delay : " + std::to_string(debug_flag.trigger_delay));
	if (debug_flag.acquisition_mode == static_cast< int >(paramTypeBasler::AcquisitionMode::EnableAcquisitionFrameRate))
	{
		baslerMessage(" Acquisition mode : FrameRate");
	}
	else
	{
		baslerMessage(" Acquisition mode : Trigger");
	}
	if (debug_flag.fast_mode == static_cast< int >(paramTypeBasler::FastMode::SensorReadoutModeNormal))
	{
		baslerMessage(" Fast mode : Normal");
	}
	else
	{
		baslerMessage(" Fast mode : Fast");
	}
	if (debug_flag.grab_strategy == static_cast< int >(paramTypeBasler::GrabStrategy::OneByOne))
	{
		baslerMessage(" Grab strategy : OneByOne");
	}
	else
	{
		baslerMessage(" Grab strategy : LatestOnly");
	}
	if (debug_flag.cap_type == static_cast< int >(paramTypeBasler::CaptureType::ColorGrab))
	{
		baslerMessage(" Capture type :  Color");
	}
	else if (debug_flag.cap_type == static_cast< int >(paramTypeBasler::CaptureType::MonocroGrab))
	{
		baslerMessage(" Capture type :  Mono");
	}
	else if (debug_flag.cap_type == static_cast< int >(paramTypeBasler::CaptureType::BayerGrab))
	{
		baslerMessage(" Capture type :  Bayer");
	}
	else if (debug_flag.cap_type == static_cast< int >(paramTypeBasler::CaptureType::Monocro10Grab))
	{
		baslerMessage(" Capture type :  Mono 10");
	}

	else if (debug_flag.cap_type == static_cast< int >(paramTypeBasler::CaptureType::BayerBGGrab))
	{
		baslerMessage(" Capture type :  BayerBG");
	}
	else if (debug_flag.cap_type == static_cast< int >(paramTypeBasler::CaptureType::BayerGBGrab))
	{
		baslerMessage(" Capture type :  BayerGB");
	}
	else if (debug_flag.cap_type == static_cast< int >(paramTypeBasler::CaptureType::BayerRGGrab))
	{
		baslerMessage(" Capture type :  BayerRG");
	}
	else if (debug_flag.cap_type == static_cast< int >(paramTypeBasler::CaptureType::BayerGRGrab))
	{
		baslerMessage(" Capture type :  BayerGR");
	}
}

/// �ʃJ�����ڑ�?
void basler::connect(int id)
{
	if (id < camera_number)
	{
		Cameras = new Camera_t(tlFactory->CreateDevice(devices[id]));
	}
	else
	{
		std::runtime_error("number of camera is over flow.");
	}
	deviceName = Cameras->GetDeviceInfo().GetModelName();
	baslerMessage("Using device " + deviceName);
	Cameras->Open();
	Cameras->AcquisitionFrameRateEnable.SetValue(true);
	if (deviceName == "acA640-750uc" || deviceName == "acA640-750um") {
		Cameras->SensorReadoutMode.SetValue(Basler_UsbCameraParams::SensorReadoutMode_Normal);
	}
	Cameras->AcquisitionFrameRate.SetValue(CAM_FPS);
	Cameras->Width.SetValue(CAM_WIDTH);
	Cameras->Height.SetValue(CAM_HEIGHT);
	// �f�o�b�O�p
	debug_flag.exposure_time = Cameras->ExposureTime.GetValue();
}

/// �J�����I������
void basler::disconnect()
{
	Cameras->Close();
}
/// �p�����^�ݒ�� �B���J�n or �B���ҋ@
void basler::start()
{
	Cameras->StartGrabbing(grab_strategy);
}
/// �B���I��(�����)
void basler::stop()
{
	Cameras->StopGrabbing();
}

/// �p�����^�̐ݒ�
void basler::setParam(const paramTypeCamera::paramInt &pT, const int param)
{
	switch (pT)
	{
	case paramTypeCamera::paramInt::WIDTH:
		if (Cameras->Width.GetMax() >= param) width = param;
		else
		{
			baslerMessage(" WIDTH :: Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
			width = Cameras->Width.GetMax();
		}
		Cameras->Width.SetValue(width);
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		if (Cameras->Height.GetMax() >= param) height = param;
		else
		{
			baslerMessage(" HEIGHT :: Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
			height = Cameras->Height.GetMax();
		}
		Cameras->Height.SetValue(param);
		break;
		//case paramType::FPS  :
		//case paramType::GAIN :
		//	std::runtime_error( " param�̈����𖾎��I��float�^�Ƃ��ė^���Ă������� " );
		//	break;
	}
}
void basler::setParam(const paramTypeCamera::paramFloat &pT, const float param)
{
	switch (pT)
	{
	case paramTypeCamera::paramFloat::FPS:
		if (Cameras->AcquisitionFrameRateEnable.GetValue() == true)
		{
			Cameras->AcquisitionFrameRate.SetValue(param);
			if (Cameras->ResultingFrameRate.GetValue() >= param) {
				fps = param;
			}
			else{
				baslerMessage(" FPS :: Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
				fps = Cameras->ResultingFrameRate.GetValue();
			}
		}
		else
		{
			baslerMessage("trigger mode�Ƃ̕��p�͂ł��܂���");
		}
		break;
	case paramTypeCamera::paramFloat::GAIN:
		if (Cameras->Gain.GetMax() < param)
		{
			baslerMessage(" GAIN :: Max�ȏ�̐ݒ�l��^���Ă���̂�Max�ɐݒ肵�܂�");
			gain = Cameras->Gain.GetMax();
		}
		else
		{
			gain = param;
		}
		Cameras->Gain.SetValue(param);
		break;
		//case paramType::WIDTH  :
		//case paramType::HEIGHT :
		//	std::runtime_error( " param�̈����𖾎��I��int�^�Ƃ��ė^���Ă������� " );
		//	break;
	}
}
void basler::setParam(const paramTypeBasler::Param &pT, const float param)
{
	switch (pT)
	{
	case paramTypeBasler::Param::ExposureTime:
		if (param > (1000000.0f / fps - 50.0f) && fps < 1000.0f) std::runtime_error("�B�����[�g�ɑ΂��ĘI�����Ԃ��������܂�");
		Cameras->ExposureTime.SetValue(param);
		debug_flag.exposure_time = param;
		break;
	case paramTypeBasler::Param::TriggerDelay:
		if (Cameras->TriggerMode.GetIntValue() != Basler_UsbCameraParams::TriggerMode_On) std::runtime_error("trigger mode����ON�ɂ��Ă�������");
		Cameras->TriggerDelay.SetValue(param);
		debug_flag.trigger_delay = param;
		break;
	}
}
void basler::setParam(const paramTypeBasler::AcquisitionMode &pT)
{
	if (pT == paramTypeBasler::AcquisitionMode::TriggerMode)
	{
		Cameras->AcquisitionFrameRateEnable.SetValue(false);
		Cameras->TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Line3);
		Cameras->TriggerSelector.SetValue(Basler_UsbCameraParams::TriggerSelector_FrameStart);
		Cameras->TriggerMode.SetIntValue(Basler_UsbCameraParams::TriggerMode_On);
		debug_flag.acquisition_mode = static_cast< int >(paramTypeBasler::AcquisitionMode::TriggerMode);
	}
	else if (pT == paramTypeBasler::AcquisitionMode::EnableAcquisitionFrameRate)
	{
		Cameras->TriggerMode.SetIntValue(Basler_UsbCameraParams::TriggerMode_Off);
		Cameras->AcquisitionFrameRateEnable.SetValue(true);
		debug_flag.acquisition_mode = static_cast< int >(paramTypeBasler::AcquisitionMode::EnableAcquisitionFrameRate);
	}
}
void basler::setParam(const paramTypeBasler::FastMode &pT)
{
	if (deviceName == "acA640-750uc" || deviceName == "acA640-750um") {
		if (pT == paramTypeBasler::FastMode::SensorReadoutModeFast)
		{
			Cameras->SensorReadoutMode.SetValue(Basler_UsbCameraParams::SensorReadoutMode_Fast);
			debug_flag.fast_mode = static_cast<int>(paramTypeBasler::FastMode::SensorReadoutModeFast);
		}
		else if (pT == paramTypeBasler::FastMode::SensorReadoutModeNormal)
		{
			Cameras->SensorReadoutMode.SetValue(Basler_UsbCameraParams::SensorReadoutMode_Normal);
			debug_flag.fast_mode = static_cast<int>(paramTypeBasler::FastMode::SensorReadoutModeNormal);
		}
	}
}
void basler::setParam(const paramTypeBasler::CaptureType &pT)
{
	switch (pT)
	{
	case paramTypeBasler::CaptureType::ColorGrab:
		formatConverter.OutputPixelFormat = PixelType_RGB8packed;
		capture_type_flag = paramTypeBasler::CaptureType::ColorGrab;
		debug_flag.cap_type = static_cast< int >(paramTypeBasler::CaptureType::ColorGrab);
		break;
	case paramTypeBasler::CaptureType::ColorBGRGrab:
		formatConverter.OutputPixelFormat = PixelType_BGR8packed;
		capture_type_flag = paramTypeBasler::CaptureType::ColorBGRGrab;
		debug_flag.cap_type = static_cast< int >(paramTypeBasler::CaptureType::ColorBGRGrab);
		break;
	case paramTypeBasler::CaptureType::MonocroGrab:
		Cameras->PixelFormat.SetValue(Basler_UsbCameraParams::PixelFormat_Mono8);
		formatConverter.OutputPixelFormat = PixelType_Mono8;
		capture_type_flag = paramTypeBasler::CaptureType::MonocroGrab;
		debug_flag.cap_type = static_cast< int >(paramTypeBasler::CaptureType::MonocroGrab);
		break;
	case paramTypeBasler::CaptureType::Monocro10Grab:
		Cameras->PixelFormat.SetValue(Basler_UsbCameraParams::PixelFormat_Mono10);
		capture_type_flag = paramTypeBasler::CaptureType::Monocro10Grab;
		debug_flag.cap_type = static_cast< int >(paramTypeBasler::CaptureType::Monocro10Grab);
		break;
	case paramTypeBasler::CaptureType::BayerGrab:
	case paramTypeBasler::CaptureType::BayerBGGrab:
		//formatConverter.OutputPixelFormat = PixelType_BayerBG8;
		Cameras->PixelFormat.SetValue(Basler_UsbCameraParams::PixelFormat_BayerBG8);
		capture_type_flag = paramTypeBasler::CaptureType::BayerBGGrab;
		debug_flag.cap_type = static_cast< int >(paramTypeBasler::CaptureType::BayerBGGrab);
		break;

	case paramTypeBasler::CaptureType::BayerGBGrab:
		Cameras->PixelFormat.SetValue(Basler_UsbCameraParams::PixelFormat_BayerGB8);
		capture_type_flag = paramTypeBasler::CaptureType::BayerGBGrab;
		debug_flag.cap_type = static_cast< int >(paramTypeBasler::CaptureType::BayerGBGrab);
		break;
	case paramTypeBasler::CaptureType::BayerRGGrab:
		Cameras->PixelFormat.SetValue(Basler_UsbCameraParams::PixelFormat_BayerRG8);
		capture_type_flag = paramTypeBasler::CaptureType::BayerRGGrab;
		debug_flag.cap_type = static_cast< int >(paramTypeBasler::CaptureType::BayerRGGrab);
		break;
	case paramTypeBasler::CaptureType::BayerGRGrab:
		Cameras->PixelFormat.SetValue(Basler_UsbCameraParams::PixelFormat_BayerBG8);
		capture_type_flag = paramTypeBasler::CaptureType::BayerGRGrab;
		debug_flag.cap_type = static_cast< int >(paramTypeBasler::CaptureType::BayerGRGrab);
		break;
	}
}
void basler::setParam(const paramTypeBasler::GrabStrategy &pT)
{
	switch (pT)
	{
	case paramTypeBasler::GrabStrategy::OneByOne:
		grab_strategy = GrabStrategy_OneByOne;
		debug_flag.grab_strategy = static_cast< int >(paramTypeBasler::GrabStrategy::OneByOne);
		break;
	case paramTypeBasler::GrabStrategy::LatestOnlyFrame:
		grab_strategy = GrabStrategy_LatestImageOnly;
		debug_flag.grab_strategy = static_cast< int >(paramTypeBasler::GrabStrategy::LatestOnlyFrame);
		break;
	}
}

/// �p�����^�̎擾
//virtual void getParam(const paramType &pT, void* param) = 0;
int basler::getParam(const paramTypeCamera::paramInt &pT)
{
	int tmp;
	switch (pT)
	{
	case paramTypeCamera::paramInt::WIDTH:
		tmp = width;
		break;
	case paramTypeCamera::paramInt::HEIGHT:
		tmp = height;
		break;
	}
	return tmp;
}

float basler::getParam(const paramTypeCamera::paramFloat &pT)
{
	float tmp;
	switch (pT)
	{
	case paramTypeCamera::paramFloat::FPS:
		tmp = fps;
		break;
	case paramTypeCamera::paramFloat::GAIN:
		tmp = gain;
		break;
	}
	return tmp;
}
float basler::getParam(const paramTypeBasler::Param &pT)
{
	float tmp;
	switch (pT)
	{
	case paramTypeBasler::Param::ExposureTime:
		tmp = Cameras->ExposureTime.GetValue();
		break;
	case paramTypeBasler::Param::TriggerDelay:
		tmp = Cameras->TriggerDelay.GetValue();
		break;
	}
	return tmp;
}

/// �摜�̎擾(�P��)
void basler::captureFrame(void* data)
{
	if (Cameras->IsGrabbing())
	{
		Cameras->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
		if (ptrGrabResult->GrabSucceeded())
		{
			//formatConverter.Convert(pylonImage, ptrGrabResult);
			switch (capture_type_flag)
			{
			case paramTypeBasler::CaptureType::ColorGrab:
			case paramTypeBasler::CaptureType::ColorBGRGrab:
				formatConverter.Convert(pylonImage, ptrGrabResult);
				memcpy(data, pylonImage.GetBuffer(), width*height * 3);
				break;
			case paramTypeBasler::CaptureType::MonocroGrab:
			case paramTypeBasler::CaptureType::BayerGrab:
			case paramTypeBasler::CaptureType::BayerBGGrab:
			case paramTypeBasler::CaptureType::BayerGBGrab:
			case paramTypeBasler::CaptureType::BayerRGGrab:
			case paramTypeBasler::CaptureType::BayerGRGrab:
				//formatConverter.Convert(pylonImage, ptrGrabResult);
				//memcpy(data, pylonImage.GetBuffer(), width*height);
				memcpy(data, ptrGrabResult->GetBuffer(), width*height);
				break;
			case paramTypeBasler::CaptureType::Monocro10Grab:
				//formatConverter.Convert(pylonImage, ptrGrabResult);
				//memcpy(data, pylonImage.GetBuffer(), width*height);
				memcpy(data, ptrGrabResult->GetBuffer(), width*height * 2);
				break;
			}
			frameNumber = ptrGrabResult->GetBlockID();
		}
	}
}


void  basler::baslerMessage(std::string str) {
	HANDLE hStdout;
	WORD wAttributes;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	wAttributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hStdout, wAttributes);
	std::cout << "[Basler] " << str << std::endl;
	SetConsoleTextAttribute(hStdout, csbi.wAttributes);
	return;
}
