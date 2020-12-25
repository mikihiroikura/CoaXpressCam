#include <KYFGLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>
#include <cstring>
#include <time.h>
#include <Windows.h>

using namespace std;

#pragma warning(disable:4996)

cv::Mat in_img;
LARGE_INTEGER freq, start, end2;
double logtime;
vector<cv::Mat> cycle_buffer_imgs;
cv::Mat cvtimg = cv::Mat(1080, 1920, CV_8UC3, cv::Scalar::all(255));

//コールバック関数
void Stream_callback_func(void* userContext, STREAM_HANDLE streamHandle)
{
    static void* data = 0;
    static KYBOOL copyingDataFlag = KYFALSE;
    long long totalFrames = 0, buffSize = 0;
    int buffIndex;
    void* buffData;

    if (0 == streamHandle)		// callback with indicator for acquisition stop
    {
        copyingDataFlag = KYFALSE;
        return;
    }

    totalFrames = KYFG_GetGrabberValueInt(streamHandle, "RXFrameCounter");
    buffSize = KYFG_StreamGetSize(streamHandle);			// get buffer size
    buffIndex = KYFG_StreamGetFrameIndex(streamHandle);
    int callno = buffIndex - 2;
    if (callno < 0) callno += 200;
    buffData = KYFG_StreamGetPtr(streamHandle, callno);		// get pointer of buffer data

    if (KYFALSE == copyingDataFlag&&totalFrames>10)
    {
        copyingDataFlag = KYTRUE;
        data = (void*)realloc(data, buffSize); 		// allocate size for local buffer
        if (0 == data)
        {
            return;
        }
        printf("\rGood callback buffer handle:%X, current index:%d, total frames:%lld        ", streamHandle, buffIndex, totalFrames); //\rは同じ行の先頭に戻ることを意味する
        memcpy(data, buffData, buffSize);			// copy data to local buffer
        memcpy(in_img.data, buffData, buffSize);    // in_imgにデータを更新する
        //... Show Image with data ...
        //cv::imshow("img", in_img);
        copyingDataFlag = KYFALSE;
    }
}

//コールバック関数
void Stream_callback_func2(void* userContext, STREAM_HANDLE streamHandle)
{
    //時間計測開始
    QueryPerformanceCounter(&start);

    static KYBOOL copyingDataFlag = KYFALSE;
    long long totalFrames = 0, buffSize = 0;
    int buffIndex;
    void* buffData;

    if (0 == streamHandle)		// callback with indicator for acquisition stop
    {
        copyingDataFlag = KYFALSE;
        return;
    }

    totalFrames = KYFG_GetGrabberValueInt(streamHandle, "RXFrameCounter");
    buffSize = KYFG_StreamGetSize(streamHandle);			// get buffer size 1920x1080
    buffIndex = KYFG_StreamGetFrameIndex(streamHandle);
    buffData = KYFG_StreamGetPtr(streamHandle, buffIndex);		// get pointer of buffer data

    if (KYFALSE == copyingDataFlag)
    {
        copyingDataFlag = KYTRUE;
        printf("\rGood callback buffer handle:%X, current index:%d, total frames:%lld        ", streamHandle, buffIndex, totalFrames); //\rは同じ行の先頭に戻ることを意味する
        memcpy(cvtimg.data, (uchar*)buffData, buffSize);
        cv::cvtColor(cvtimg, cycle_buffer_imgs[buffIndex], CV_BGR2RGB);

        //... Show Image with data ...
        //cv::imshow("img", in_img);
        copyingDataFlag = KYFALSE;
        //時間計測終了
        QueryPerformanceCounter(&end2);
        logtime = (double)(end2.QuadPart - start.QuadPart) / freq.QuadPart;
        printf(" logtime: %.6f", logtime);
    }
}

int main() {
	KY_SOFTWARE_VERSION ver;
	KY_DEVICE_INFO device_info;
	FGHANDLE handle;
	CAMHANDLE camhandle[2];
    STREAM_HANDLE streamhandle = 0;

	int status;
	int device_num;
	int camdetectcnt;

	device_info.version = 3;
	ver.struct_version = 0;

    //in_imgの初期化
    int cyclebuffersize = 20;
    in_img = cv::Mat(1080, 1920, CV_8UC3, cv::Scalar::all(255));
    for (size_t i = 0; i < cyclebuffersize; i++)
    {
        cycle_buffer_imgs.push_back(in_img.clone());
    }

    //画像保存用のVector用意
    vector<cv::Mat> save_img;
    string save_dir = "E:\\Github_output\\CoaXpressCam\\GetImgs\\";
    time_t now = time(NULL);
    struct tm* pnow = localtime(&now);
    char buff[128] = "";
    sprintf(buff, "%04d%02d%02d%02d%02d",1900+pnow->tm_year, 1+pnow->tm_mon, pnow->tm_mday, pnow->tm_hour, pnow->tm_min);
    save_dir += string(buff);

    //時間変数の設定
    if (!QueryPerformanceFrequency(&freq)) return 0;

	
	//KYの動作確認
	status = KY_GetSoftwareVersion(&ver);

	status = KY_DeviceScan(&device_num); //PCIeのデバイスの数を表す
	for (size_t i = 0; i < device_num; i++)
	{
		cout << i << " " << KY_DeviceDisplayName(i) << endl;
	}
	handle = KYFG_Open(0);//0番目のデバイスを開く
	camdetectcnt = 2;
    int selcamnum = 1;
	status = KYFG_UpdateCameraList(handle, &camhandle[0], &camdetectcnt);//ボード接続しているカメラすべて認識
	status = KYFG_CameraOpen2(camhandle[selcamnum], NULL);//カメラを開く

	//カメラの動作設定
    status = KYFG_CameraCallbackRegister(camhandle[selcamnum], Stream_callback_func2, 0); //Callback関数をセット
   /* int param = 600;
    if (param % 64 != 0) param = param / 64 * 64;*/
    
    status = KYFG_SetCameraValueInt(camhandle[selcamnum], "Width", 896);
    int64_t c = KYFG_GetCameraValueInt(camhandle[selcamnum], "Width");
    KYFG_SetCameraValueInt(camhandle[selcamnum], "Height", 1080); //画像のWxHをセット
    //status = KYFG_SetCameraValueFloat(camhandle, "AcquisitionFrameRate", 1000.00);
    float fps = KYFG_GetCameraValueFloat(camhandle[selcamnum], "AcquisitionFrameRate");
    status = KYFG_SetCameraValueEnum_ByValueName(camhandle[selcamnum], "PixelFormat", "BayerGR8");
    status = KYFG_SetGrabberValueEnum_ByValueName(handle, "PixelFormat", "RGB8");

    double d = KYFG_GetCameraValueEnum(camhandle[selcamnum], "Gain");
    /*status = KYFG_SetCameraValueEnum_ByValueName(camhandle, "Gain", "x2");
    d = KYFG_GetCameraValueEnum(camhandle, "Gain");*/

    int64_t a = KYFG_GetCameraValueInt(camhandle[selcamnum], "WidthMax");

    int64_t w = KYFG_GetCameraValueInt(camhandle[selcamnum], "OffsetX");
    //status = KYFG_SetCameraValueInt(camhandle[selcamnum], "OffsetX", 0);
    a = KYFG_GetCameraValueInt(camhandle[selcamnum], "OffsetXMax");

    float x = KYFG_GetCameraValueFloat(camhandle[selcamnum], "ExposureTime");
    float g = KYFG_GetCameraValueFloat(camhandle[selcamnum], "pExposureTimeRegMax");
    status = KYFG_SetCameraValueFloat(camhandle[selcamnum], "ExposureTime", 912);

    status = KYFG_StreamCreateAndAlloc(camhandle[selcamnum], &streamhandle, cyclebuffersize, 0);//Cyclic frame bufferのStreamの設定

    status = KYFG_CameraStart(camhandle[selcamnum], streamhandle, 0);//カメラの動作開始，Framesを0にすると連続して画像を取り続ける
    //これ以降カメラのLEDが緑になる

    while (1)
    {
        //cv::cvtColor(cycle_buffer_imgs[0], cvtimg, CV_BGR2RGB);
        cv::imshow("img", cycle_buffer_imgs[0]);
        int key = cv::waitKey(1);
        if (key == 'q')break;
        else if (key == 'p')
        {
            save_img.push_back(cycle_buffer_imgs[0].clone());
        }
    }

    //画像の保存
    
    for (int i = 0; i < save_img.size(); i++)
    {
        sprintf(buff, "img%03d.png", i);
        string img_name = string(buff);
        cv::imwrite(save_dir + img_name, save_img[i]);
    }


    KYFG_CameraClose(camhandle[selcamnum]);//カメラを閉じる

	status = KYFG_Close(handle);//ボードを閉じる

	return 0;
}