#include <KYFGLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>

using namespace std;

cv::Mat in_img;

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
    buffData = KYFG_StreamGetPtr(streamHandle, buffIndex);		// get pointer of buffer data

    if (KYFALSE == copyingDataFlag)
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

int main() {
	KY_SOFTWARE_VERSION ver;
	KY_DEVICE_INFO device_info;
	FGHANDLE handle;
	CAMHANDLE camhandle;
    STREAM_HANDLE streamhandle = 0;

	int status;
	int device_num;
	int camdetectcnt;

	device_info.version = 3;
	ver.struct_version = 0;

    //in_imgの初期化
    in_img = cv::Mat(1080, 1920, CV_8UC1, cv::Scalar::all(255));

    cv::Mat a = cv::Mat(1080, 1920, CV_8UC1, cv::Scalar::all(255));
	
	//KYの動作確認
	status = KY_GetSoftwareVersion(&ver);

	status = KY_DeviceScan(&device_num); //PCIeのデバイスの数を表す
	for (size_t i = 0; i < device_num; i++)
	{
		cout << i << " " << KY_DeviceDisplayName(i) << endl;
	}
	handle = KYFG_Open(0);//0番目のデバイスを開く
	camdetectcnt = 1;
	status = KYFG_UpdateCameraList(handle, &camhandle, &camdetectcnt);//ボード接続しているカメラすべて認識
	status = KYFG_CameraOpen2(camhandle, NULL);//カメラを開く

	//カメラの動作設定
    status = KYFG_CameraCallbackRegister(camhandle, Stream_callback_func, 0); //Callback関数をセット
    KYFG_SetCameraValueInt(camhandle, "Width", 1920);
    KYFG_SetCameraValueInt(camhandle, "Height", 1080); //画像のWxHをセット
    status = KYFG_SetCameraValueEnum_ByValueName(camhandle, "PixelFormat", "Mono8");

    status = KYFG_StreamCreateAndAlloc(camhandle, &streamhandle, 16, 0);//Cyclic frame bufferのStreamの設定

    status = KYFG_CameraStart(camhandle, streamhandle, 0);//カメラの動作開始，Framesを0にすると連続して画像を取り続ける
    //これ以降カメラのLEDが緑になる

    while (1)
    {
        cv::imshow("img", in_img);
        int key = cv::waitKey(1);
        if (key == 'q')break;
    }


    KYFG_CameraClose(camhandle);//カメラを閉じる

	status = KYFG_Close(handle);//ボードを閉じる

	return 0;
}