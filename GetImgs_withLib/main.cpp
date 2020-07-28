#include <HSC/KAYACoaXpressClass.hpp>

#ifdef _DEBUG
#define LIB_EXT "d.lib"
#else
#define LIB_EXT ".lib"
#endif


#pragma comment(lib,"KAYACoaXpressLib" LIB_EXT)

int main() {
	//�J�����p�����[�^
	int width = 600;
	int height = 400;
	float fps = 1000.0;
	int offsetx = 0;
	int offsety = 0;

	cv::Mat in_img = cv::Mat(height, width, CV_8UC1, cv::Scalar::all(255));

	kayacoaxpress cam;
	cam.connect(0);
	//�p�����[�^�̐ݒ�
	cam.setParam(paramTypeCamera::paramInt::WIDTH, width);
	cam.setParam(paramTypeCamera::paramInt::HEIGHT, height);
	cam.setParam(paramTypeCamera::paramFloat::FPS, fps);
	cam.setParam(paramTypeKAYACoaXpress::Gain::x1);
	//cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetX, offsetx);
	//cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetY, offsety);
	cam.parameter_all_print();
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