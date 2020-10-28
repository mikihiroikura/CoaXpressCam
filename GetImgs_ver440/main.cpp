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

//�R�[���o�b�N�֐�
void Stream_callback_func2(void* userContext, STREAM_HANDLE streamHandle)
{


    static KYBOOL copyingDataFlag = KYFALSE;
    long long totalFrames = 0, buffSize = 0;
    int buffIndex;
    void* buffData;
    cv::Mat conv_img = cv::Mat(1080, 1920, CV_8UC1, cv::Scalar::all(255));

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
        printf("\rGood callback buffer handle:%X, current index:%d, total frames:%lld        ", streamHandle, buffIndex, totalFrames); //\r�͓����s�̐擪�ɖ߂邱�Ƃ��Ӗ�����
        memcpy(conv_img.data, buffData, buffSize);
        QueryPerformanceCounter(&start);
        cv::cvtColor(conv_img, cycle_buffer_imgs[buffIndex], CV_BayerGR2GRAY);
        QueryPerformanceCounter(&end2);
        //memcpy(cycle_buffer_imgs[buffIndex].data, buffData, buffSize);			// copy data to local buffer
        //... Show Image with data ...
        //cv::imshow("img", in_img);
        copyingDataFlag = KYFALSE;
        logtime = (double)(end2.QuadPart - start.QuadPart) / freq.QuadPart;
        printf(" logtime: %.6f", logtime);
    }
}

int main() {
    KY_DEVICE_INFO device_info;
    FGHANDLE handle;
    CAMHANDLE camhandle;
    STREAM_HANDLE streamhandle = 0;

    int status;
    int device_num;
    int camdetectcnt;

    //in_img�̏�����
    int cyclebuffersize = 20;
    in_img = cv::Mat(1080, 1920, CV_8UC1, cv::Scalar::all(255));
    for (size_t i = 0; i < cyclebuffersize; i++)
    {
        cycle_buffer_imgs.push_back(in_img.clone());
    }

    ////�摜�ۑ��p��Vector�p��
    //vector<cv::Mat> save_img;
    //string save_dir = "E:\\Github_output\\CoaXpressCam\\GetImgs\\";
    //time_t now = time(NULL);
    //struct tm* pnow = localtime(&now);
    //char buff[128] = "";
    //sprintf(buff, "%04d%02d%02d%02d%02d", 1900 + pnow->tm_year, 1 + pnow->tm_mon, pnow->tm_mday, pnow->tm_hour, pnow->tm_min);
    //save_dir += string(buff);

    //���ԕϐ��̐ݒ�
    if (!QueryPerformanceFrequency(&freq)) return 0;


    //KY�̓���m�F
    /*status = KY_GetSoftwareVersion(&ver);*/

    device_num = KYFG_Scan(0,0); //PCIe�̃f�o�C�X�̐���\��
    for (size_t i = 0; i < device_num; i++)
    {
        cout << i << " " << KY_DeviceDisplayName(i) << endl;
    }
    handle = KYFG_Open(0);//0�Ԗڂ̃f�o�C�X���J��
    camdetectcnt = 1;
    status = KYFG_CameraScan(handle, &camhandle, &camdetectcnt);
    //status = KYFG_UpdateCameraList(handle, &camhandle, &camdetectcnt);//�{�[�h�ڑ����Ă���J�������ׂĔF��
    status = KYFG_CameraOpen2(camhandle, NULL);//�J�������J��

    //�J�����̓���ݒ�
    status = KYFG_CameraCallbackRegister(camhandle, Stream_callback_func2, 0); //Callback�֐����Z�b�g
    status = KYFG_StreamCreateAndAlloc(camhandle, &streamhandle, 20, 0);//Cyclic frame buffer��Stream�̐ݒ�
    /*int param = 600;
    if (param % 64 != 0) param = param / 64 * 64;
    status = KYFG_SetCameraValueInt(camhandle, "Width", param);*/
    status = KYFG_SetCameraValueInt(camhandle, "Width", 1920);
    int64_t c = KYFG_GetCameraValueInt(camhandle, "Width");
    KYFG_SetCameraValueInt(camhandle, "Height", 1080); //�摜��WxH���Z�b�g
    //status = KYFG_SetCameraValueFloat(camhandle, "AcquisitionFrameRate", 1000.00);
    float fps = KYFG_GetCameraValueFloat(camhandle, "AcquisitionFrameRate");
    status = KYFG_SetCameraValueEnum_ByValueName(camhandle, "PixelFormat", "BayerGR8");

    double d = KYFG_GetCameraValueEnum(camhandle, "Gain");
    status = KYFG_SetCameraValueEnum_ByValueName(camhandle, "Gain", "x2");
    d = KYFG_GetCameraValueEnum(camhandle, "Gain");

    int64_t a = KYFG_GetCameraValueInt(camhandle, "WidthMax");

    int64_t w = KYFG_GetCameraValueInt(camhandle, "OffsetX");
    status = KYFG_SetCameraValueInt(camhandle, "OffsetX", 0);
    a = KYFG_GetCameraValueInt(camhandle, "OffsetXMax");

    float x = KYFG_GetCameraValueFloat(camhandle, "ExposureTime");
    float g = KYFG_GetCameraValueFloat(camhandle, "pExposureTimeRegMax");
    status = KYFG_SetCameraValueFloat(camhandle, "ExposureTime", 912);

    status = KYFG_StreamCreateAndAlloc(camhandle, &streamhandle, cyclebuffersize, 0);//Cyclic frame buffer��Stream�̐ݒ�

    status = KYFG_CameraStart(camhandle, streamhandle, 0);//�J�����̓���J�n�CFrames��0�ɂ���ƘA�����ĉ摜����葱����
    //����ȍ~�J������LED���΂ɂȂ�

    //���Ԍv���J�n
    //QueryPerformanceCounter(&start);
    while (1)
    {
        cv::imshow("img", cycle_buffer_imgs[0]);
        int key = cv::waitKey(1);
        if (key == 'q')break;
        /*else if (key == 'p')
        {
            save_img.push_back(cycle_buffer_imgs[0].clone());
        }*/
        //���Ԍv���I��
        //QueryPerformanceCounter(&end2);
        
    }

    //�摜�̕ۑ�

    //for (int i = 0; i < save_img.size(); i++)
    //{
    //    sprintf(buff, "img%03d.png", i);
    //    string img_name = string(buff);
    //    cv::imwrite(save_dir + img_name, save_img[i]);
    //}


    KYFG_CameraClose(camhandle);//�J���������

    status = KYFG_Close(handle);//�{�[�h�����

    return 0;
}