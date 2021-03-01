///
/// @file		CameraUI.hpp
/// @brief		高速カメラの共通化クラス
/// @author		Satoshi Tabata
/// @date		2017/7/27 作成
/// @date		2017/9/13 最終更新
/// @version	1.1.3
///
///

#include <HSC/Camera.hpp>
#include <HSC/CameraUI.hpp>

CameraUI::CameraUI(const cameraType &ct) {
	if (cam) {
		assert(false);
	}
	else {
		switch (ct) {
		case cameraType::EOSENS:
#ifdef USE_EOSENS
			cam = new Eosens;
#endif
			break;
		case cameraType::BASLER:
#ifdef USE_BASLER
			cam = new basler;
#endif
			break;
		case cameraType::IDPEXPRESS:
#ifdef USE_IDPEXPRESS
			cam = new idpExpress;
#endif
			break;
		case cameraType::XIMEA:
#ifdef USE_XIMEA
			cam = new Ximea;
#endif
			break;

		default:
			assert(false);
			break;
		}
	}
}

CameraUI::~CameraUI() {
	if (cam) {
		delete cam;
	}
}

void CameraUI::connect(int id) {
	assert(cam);
	cam->connect(id);
}

void CameraUI::disconnect() {
	if (cam) {
		cam->disconnect();
	}
}

void CameraUI::start() {
	if (cam) {
		cam->start();
	}
}

void CameraUI::stop() {
	if (cam) {
		cam->stop();
	}
}


void CameraUI::setParam(const paramTypeCamera::paramInt &pT, const int param) {
	assert(cam);
	cam->setParam(pT, param);
}

void CameraUI::setParam(const paramTypeCamera::paramFloat &pT, const float param) {
	assert(cam);
	cam->setParam(pT, param);
}

int CameraUI::getParam(const paramTypeCamera::paramInt &pT) {
	assert(cam);
	return cam->getParam(pT);
}

float CameraUI::getParam(const paramTypeCamera::paramFloat &pT) {
	assert(cam);
	return cam->getParam(pT);
}


void CameraUI::captureFrame(void* data) {
	assert(cam);
	cam->captureFrame(data);
}

void CameraUI::captureFrameStereo(void* dataL, void* dataR) {
	assert(cam);
	cam->captureFrameStereo(dataL, dataR);

}


/// idpExpress用
void CameraUI::setParam(const paramTypeIdpExpress::AcquisitionMode &pT) {
	assert(cam);
	cam->setParam(pT);
};
void CameraUI::setParam(const paramTypeIdpExpress::CameraHead &pT) {
	assert(cam);
	cam->setParam(pT);
};

/// Basler用
void CameraUI::setParam(const paramTypeBasler::Param &pT, const float param) {
	assert(cam);
	cam->setParam(pT, param);
};
void CameraUI::setParam(const paramTypeBasler::AcquisitionMode &pT) {
	assert(cam);
	cam->setParam(pT);
};
void CameraUI::setParam(const paramTypeBasler::FastMode &pT) {
	assert(cam);
	cam->setParam(pT);
};
void CameraUI::setParam(const paramTypeBasler::CaptureType &pT) {
	assert(cam);
	cam->setParam(pT);
};
void CameraUI::setParam(const paramTypeBasler::GrabStrategy &pT) {
	assert(cam);
	cam->setParam(pT);
};
float CameraUI::getParam(const paramTypeBasler::Param &pT) {
	assert(cam);
	return cam->getParam(pT);
};

void CameraUI::parameter_all_print() {
	assert(cam);
	return cam->parameter_all_print();
}

/// Ximea用
void CameraUI::setParam(const paramTypeXimea::AcquisitionMode &pT) {
	assert(cam);
	cam->setParam(pT);
};
void CameraUI::setParam(const paramTypeXimea::CaptureType &pT) {
	assert(cam);
	cam->setParam(pT);
};

/// Eosens用
void CameraUI::setParam(const paramTypeEosens::paramInt &pT, const  int param) {
	assert(cam);
	cam->setParam(pT, param);
};
void CameraUI::setParam(const paramTypeEosens::paramFloat &pT, const  float param) {
	assert(cam);
	cam->setParam(pT, param);
};
int CameraUI::getParam(const paramTypeEosens::paramInt &pT) {
	assert(cam);
	return cam->getParam(pT);
};
float CameraUI::getParam(const paramTypeEosens::paramFloat &pT) {
	assert(cam);
	return cam->getParam(pT);
};

CameraUI::cameraType CameraUI::getCameraType() { return camtype; }