///
/// @file		KAYACoaXpressClass.cpp
/// @brief		KAYACoaXpress Optronics�J����
/// @author		Mikihiro Ikura
/// @date		2020/7/21 �쐬
/// @date		2020/7/21 �ŏI�X�V
/// @version	1.0.0
///

#include <HSC/KAYACoaXpressClass.hpp>
#include <iostream>

int kayacoaxpress::status = 0;
int kayacoaxpress::pcie_device_num = 0;
int kayacoaxpress::cam_detect_cnt = 0;


//�������CFGB�̐��C�J�������̊m�F
kayacoaxpress::kayacoaxpress() : Camera(CAM_WIDTH, CAM_HEIGHT, CAM_FPS)
{
	//�ϐ��̏�����
	stream_handle = 0;

	//FGB�̔F���C�ڑ�
	status = KY_DeviceScan(&pcie_device_num);
	if (pcie_device_num == 0) std::runtime_error("No FGB device presrent.");
	for (size_t i = 0; i < pcie_device_num; i++)
	{
		std::cout << i << " " << KY_DeviceDisplayName(i) << std::endl;
	}
	fg_handle = KYFG_Open(0);
	cam_detect_cnt = 1;

	//�J�������̊m�F
	status = KYFG_UpdateCameraList(fg_handle, &cam_handle, &cam_detect_cnt);//�{�[�h�ڑ����Ă���J�������ׂĔF��
}

kayacoaxpress::~kayacoaxpress()
{
	status = KYFG_Close(fg_handle);
	kayacoaxpressMessage("Frame Grabber Board Closed.");
}

void kayacoaxpress::connect(int id)
{
	printf("a");
}

void kayacoaxpress::disconnect()
{
	printf("a");
}

void kayacoaxpress::start()
{

}

void kayacoaxpress::stop()
{

}

void kayacoaxpress::captureFrame(void* data)
{

}

void kayacoaxpress::setParam(const paramTypeCamera::paramInt& pT, const int param)
{

}

void kayacoaxpress::setParam(const paramTypeCamera::paramFloat& pT, const float param)
{

}

int kayacoaxpress::getParam(const paramTypeCamera::paramInt& pT)
{
	return 10;
}

float kayacoaxpress::getParam(const paramTypeCamera::paramFloat& pT)
{
	return 10;
}

void kayacoaxpress::kayacoaxpressMessage(std::string str)
{
	std::cout << "[KAYACoaXpress] " << str << std::endl;
}
