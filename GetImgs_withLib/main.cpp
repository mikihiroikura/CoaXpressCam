#include <HSC/KAYACoaXpressClass.hpp>

#ifdef _DEBUG
#define LIB_EXT "d.lib"
#else
#define LIB_EXT ".lib"
#endif


#pragma comment(lib,"KAYACoaXpressLib" LIB_EXT)

int main() {
	cv::Mat in_img = cv::Mat(1080, 1920, CV_8UC1, cv::Scalar::all(255));

	kayacoaxpress cam;
	cam.connect(0);
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