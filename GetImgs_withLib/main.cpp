#include <HSC/KAYACoaXpressClass.hpp>

#ifdef _DEBUG
#define LIB_EXT "d.lib"
#else
#define LIB_EXT ".lib"
#endif


#pragma comment(lib,"KAYACoaXpressLib" LIB_EXT)

int main() {
	//�J�����p�����[�^
	int width = 1920;
	int height = 1080;
	float fps = 1000.0;
	float exposuretime = 912.0;
	int offsetx = 0;
	int offsety = 0;

	kayacoaxpress cam;
	cam.connect(0);
	//�p�����[�^�̐ݒ�
	cam.setParam(paramTypeCamera::paramInt::WIDTH, width);
	cam.setParam(paramTypeCamera::paramInt::HEIGHT, height);
	cam.setParam(paramTypeCamera::paramFloat::FPS, fps);
	cam.setParam(paramTypeKAYACoaXpress::paramFloat::ExposureTime, exposuretime);
	cam.setParam(paramTypeKAYACoaXpress::Gain::x1);
	//cam.setParam(paramTypeKAYACoaXpress::paramInt::CycleBufferSize, 256);
	//cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetX, offsetx);
	//cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetY, offsety);
	cam.parameter_all_print();
	//cam.parameter_all_print_debug();

	//�f�o�b�O�p

	cv::Mat in_img = cv::Mat(cam.getParam(paramTypeCamera::paramInt::HEIGHT), cam.getParam(paramTypeCamera::paramInt::WIDTH), CV_8UC1, cv::Scalar::all(255));

	cam.start();

	while (true)
	{
		cam.captureFrame(in_img.data);
		cv::imshow("imgs", in_img);
		int key = cv::waitKey(1);
		if (key == 'q') break;
	}

	cam.stop();
	cam.disconnect();
	return 0;
}