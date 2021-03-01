///
/// @file		CameraFunc.cpp
/// @brief		�����J�����̋��ʉ��֐�
/// @author		Satoshi Tabata
/// @date		2017/9/13 �쐬
/// @date		2017/9/13 �ŏI�X�V
/// @version	1.0.1
///
///

#include <HSC/Camera.hpp>
#include <HSC/CameraFunc.hpp>


Camera*  getCamera(const cameraType &ct) {
	switch (ct) {
	case cameraType::EOSENS:
#ifdef USE_EOSENS
		return new Eosens;
#endif
		break;
	case cameraType::BASLER:
#ifdef USE_BASLER
		return new basler;
#endif
		break;
	case cameraType::IDPEXPRESS:
#ifdef USE_IDPEXPRESS
		return new idpExpress;
#endif
		break;
	case cameraType::XIMEA:
#ifdef USE_XIMEA
		return new Ximea;
#endif
		break;
	default:
		assert(false);
		break;
	}
	return nullptr;
}
