#include <HSC/KAYACoaXpressClass.hpp>
#include <vector>
#include <time.h>

#ifdef _DEBUG
#define LIB_EXT "d.lib"
#else
#define LIB_EXT ".lib"
#endif


#pragma comment(lib,"KAYACoaXpressLib" LIB_EXT)
#pragma warning(disable:4996)
using namespace std;

int main() {
	//カメラパラメータ
	int width = 896;
	int height = 896;
	float fps = 1000.0;
	float exposuretime = 912.0;
	int offsetx = 480;
	int offsety = 0;

	kayacoaxpress cam;
	cam.connect(0);
	//パラメータの設定
	cam.setParam(paramTypeCamera::paramInt::WIDTH, width);
	cam.setParam(paramTypeCamera::paramInt::HEIGHT, height);
	cam.setParam(paramTypeCamera::paramFloat::FPS, fps);
	cam.setParam(paramTypeKAYACoaXpress::paramFloat::ExposureTime, exposuretime);
	cam.setParam(paramTypeKAYACoaXpress::Gain::x1);
	cam.setParam(paramTypeKAYACoaXpress::CaptureType::BayerGRGrab);
	//cam.setParam(paramTypeKAYACoaXpress::paramInt::CycleBufferSize, 256);
	cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetX, offsetx);
	//cam.setParam(paramTypeKAYACoaXpress::paramInt::OffsetY, offsety);
	cam.parameter_all_print();
	//cam.parameter_all_print_debug();

	//デバッグ用
	/*const char* a = cam.getParam(paramTypeKAYACoaXpress::Gain::x1);
	int b = cam.getParam(paramTypeCamera::paramInt::HEIGHT);
	int c = cam.getParam(paramTypeCamera::paramInt::WIDTH);
	int d = cam.getParam(paramTypeKAYACoaXpress::paramInt::CycleBufferSize);
	int e = cam.getParam(paramTypeKAYACoaXpress::paramInt::OffsetX); 
	int f = cam.getParam(paramTypeKAYACoaXpress::paramInt::OffsetY);
	float g = cam.getParam(paramTypeCamera::paramFloat::FPS);
	float h = cam.getParam(paramTypeKAYACoaXpress::paramFloat::ExposureTime);*/

	const char* outformat = cam.getParam(paramTypeKAYACoaXpress::CaptureType::BayerGRGrab);
	cv::Mat in_img;
	if (outformat == "Mono8")
	{
		in_img = cv::Mat(cam.getParam(paramTypeCamera::paramInt::HEIGHT), cam.getParam(paramTypeCamera::paramInt::WIDTH), CV_8UC1, cv::Scalar::all(255));
	}
	else if(outformat=="BayerGR8")
	{
		in_img = cv::Mat(cam.getParam(paramTypeCamera::paramInt::HEIGHT), cam.getParam(paramTypeCamera::paramInt::WIDTH), CV_8UC3, cv::Scalar::all(255));
	}
	else
	{
		in_img = cv::Mat(cam.getParam(paramTypeCamera::paramInt::HEIGHT), cam.getParam(paramTypeCamera::paramInt::WIDTH), CV_8UC1, cv::Scalar::all(255));
	}
	cv::Mat out_img;

	//画像保存用のVector用意
	vector<cv::Mat> save_img;
	string save_dir = "D:\\Github_output\\CoaXpressCam\\GetImgs_WithLib\\";
	time_t now = time(NULL);
	struct tm* pnow = localtime(&now);
	char buff[128] = "";
	sprintf(buff, "%04d%02d%02d%02d%02d", 1900 + pnow->tm_year, 1 + pnow->tm_mon, pnow->tm_mday, pnow->tm_hour, pnow->tm_min);
	save_dir += string(buff);

	cam.start();

	while (true)
	{
		cam.captureFrame(in_img.data);
		//cv::cvtColor(in_img, out_img, CV_RGB2GRAY);
		cv::imshow("imgs", in_img);
		int key = cv::waitKey(1);
		if (key == 'q') break;
		else if (key == 'p')
		{
			save_img.push_back(in_img.clone());
		}
	}

	cam.stop();
	cam.disconnect();

	//画像の保存
	for (int i = 0; i < save_img.size(); i++)
	{
		sprintf(buff, "img%03d.png", i);
		string img_name = string(buff);
		cv::imwrite(save_dir + img_name, save_img[i]);
	}
	return 0;
}