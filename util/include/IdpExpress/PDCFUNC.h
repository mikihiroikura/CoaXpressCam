/*
 *	PDCFUNC.h
 *	Photron Device Control SDK
 *	Function Definition
 *
 *	Copyright (C) 2006-2012 PHOTRON LIMITED
 */

#ifndef	_PDC_FUNC_H_
#define	_PDC_FUNC_H_

#ifdef	__cplusplus
extern "C" {
#endif	/*	__cplusplus	*/

/*
 *	Initialize etc...
 */
unsigned long WINAPI
PDC_Init(
		unsigned long 			*pErrorCode
		);

unsigned long WINAPI
PDC_DetectDevice(
		unsigned long			nInterfaceCode,
		unsigned long			*pDetectNo,
		unsigned long			nDetectNum,
		unsigned long			nDetectParam,
		PPDC_DETECT_NUM_INFO	pDetectNumInfo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_OpenDevice(
		PPDC_DETECT_INFO		pDetectInfo,
		unsigned long			*pDeviceNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_OpenDevice2(
		PPDC_DETECT_INFO		pDetectInfo,
		long					nMaxRetryCount,
		long					nConnectMode,
		unsigned long			*pDeviceNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_CloseDevice(
		unsigned long			nDeviceNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetDeviceName(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		char					*pStrName,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSubInterface(
		unsigned long			nDeviceNo,
		unsigned long			*pIPAddress,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_OpenSubInterface(
		unsigned long			nDeviceNo,
		unsigned long			*pIPAddress,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_OpenSubInterface2(
		unsigned long			nDeviceNo,
		long					nMaxRetryCount,
		long					nConnectMode,
		unsigned long			*pIPAddress,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetNumberOfDevice(
		unsigned long			*pNum,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_IsFunction(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nFunction,
		char					*pFlag,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetInterfaceInfo(
		unsigned long			nInterfaceCode,
		unsigned long			nParam1,
		unsigned long			nParam2,
		unsigned long			nParam3,
		unsigned long			nParam4,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetInterfaceInfo(
		unsigned long			nInterfaceCode,
		unsigned long			*pParam1,
		unsigned long			*pParam2,
		unsigned long			*pParam3,
		unsigned long			*pParam4,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_DeviceStatusUpdate(
		unsigned long			nDeviceNo,
		unsigned long			*pErrorCode
		);

/*
 *	Get List
 */
unsigned long WINAPI
PDC_GetRecordRateList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetResolutionList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShutterSpeedFpsList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShutterSpeedUsecList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShutterSpeedNsecList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetTriggerModeList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSensorGainModeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSensorGammaModeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetColorTempModeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLUTModeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVariableRecordRateList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetDSShutterModeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVideoOutModeList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShadingModeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetAEShutterFpsList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetAEShutterUsecList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetAEShutterNsecList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetEdgeEnhanceModeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetExternalInModeList(
		unsigned long			nDeviceNo,
		unsigned long			nPortNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetExternalOutModeList(
		unsigned long			nDeviceNo,
		unsigned long			nPortNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSyncPriorityList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetExistChildDeviceList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetOpenedDeviceList(
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLUTValueList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nPlane,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLUTOrgValueList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nPlane,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetPartitionList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pCount,
		unsigned long			*pFrames,
		unsigned long			*pBlocks,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetProgSwitchModeList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetPixelGainModeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSyncOutTimesList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetExposeOutTimesList(
		unsigned long			nDeviceNo,
		unsigned long			nSignal,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetColorEnhanceModeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetHeadExchangeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_GetLiveResolutionList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetCameraCommandBaudRateList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetDownloadModeList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVideoOutSignalList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVideoOutHDSDIList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetHighSpeedModeList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetBlackClipLevelList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetRecordingTypeList(
		unsigned long			nDeviceNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

/*
 *	Set List
 */
unsigned long WINAPI
PDC_SetLUTValueList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nPlane,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetPartitionList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nCount,
		unsigned long			*pBlocks,
		unsigned long			*pErrorCode
		);

/*
 *	Get Device status
 */
unsigned long WINAPI
PDC_GetDeviceCode(
		unsigned long			nDeviceNo,
		unsigned long			*pCode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetDeviceID(
		unsigned long			nDeviceNo,
		unsigned long			*pID,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetProductID(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pID,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLotID(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pID,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetIndividualID(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pID,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVersion(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pVer,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetColorType(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		char					*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetStatus(
		unsigned long			nDeviceNo,
		unsigned long			*pStatus,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShutterLockMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetResolutionLockMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetPartitionIncMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetKeyPadLock(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVideoOutMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetChildDeviceCount(
		unsigned long			nDeviceNo,
		unsigned long			*pCount,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMaxChildDeviceCount(
		unsigned long			nDeviceNo,
		unsigned long			*pCount,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetExternalCount(
		unsigned long			nDeviceNo,
		unsigned long			*pIn,
		unsigned long			*pOut,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetExternalInMode(
		unsigned long			nDeviceNo,
		unsigned long			nPort,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetExternalOutMode(
		unsigned long			nDeviceNo,
		unsigned long			nPort,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetIRIG(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMCDL(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetIRIGOffset(
		unsigned long			nDeviceNo,
		long					*pValue,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetRecordRate(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pRate,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetTriggerMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pAFrames,
		unsigned long			*pRFrames,
		unsigned long			*pRCount,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetResolution(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetResolutionROI(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShutterSpeedFps(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pFps,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShutterSpeedUsec(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pUsec,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShutterSpeedNsec(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pNsec,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMaxPartition(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pCount,
		unsigned long			*pBlock,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetCurrentPartition(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSensorGainMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSensorGammaMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetColorTempMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pR,
		unsigned long			*pG,
		unsigned long			*pB,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSoftColorTempMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pR,
		unsigned long			*pG,
		unsigned long			*pB,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetAutoExposure(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLUTMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetDSShutterMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetDSShutterValue(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pValue,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetEdgeEnhanceMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShadingMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShadingTypeList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pSize,
		unsigned long			*pList,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetPixelGainMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMaxFrames(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pFrames,
		unsigned long			*pBlocks,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMaxResolution(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSegmentPosition(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pXPos,
		unsigned long			*pYPos,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVariableMaxResolution(
		unsigned long			nDeviceNo,
		unsigned long			nRate,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVariableMaxWidth(
		unsigned long			nDeviceNo,
		unsigned long			nRate,
		unsigned long			nHeight,
		unsigned long			*pWidth,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVariableMaxHeight(
		unsigned long			nDeviceNo,
		unsigned long			nRate,
		unsigned long			nWidth,
		unsigned long			*pHeight,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVariableMaxRate(
		unsigned long			nDeviceNo,
		unsigned long			nWidth,
		unsigned long			nHeight,
		unsigned long			*pRate,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVariableChannel(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pChannel,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVariableChannelInfo(
		unsigned long			nDeviceNo,
		unsigned long			nChannel,
		unsigned long			*pRate,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pXPos,
		unsigned long			*pYPos,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVariableChannelInfo2(
		unsigned long			nDeviceNo,
		unsigned long			nChannel,
		unsigned long			*pRate,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pXPos,
		unsigned long			*pYPos,
		unsigned long			*pIRIG,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVariableChannelInfo3(
		unsigned long			nDeviceNo,
		unsigned long			nChannel,
		unsigned long			*pRate,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pXPos,
		unsigned long			*pYPos,
		unsigned long			*pIRIG,
		unsigned long			*pHSMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetEnableVariableChannelSetting(
		unsigned long			nDeviceNo,
		unsigned long			pRate,
		unsigned long			pWidth,
		unsigned long			pHeight,
		unsigned long			*pEnable,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetAutoExposureInfo(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pXPos,
		unsigned long			*pYPos,
		unsigned long			*pValue,
		unsigned long			*pRange,
		unsigned long			*pLimit,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMaxBitDepth(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		char					*pDepth,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLUTParams(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nLUTMode,
		PPDC_LUT_PARAMS			pParams,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetDelay(
		unsigned long			nDeviceNo,
		unsigned long			nSignal,
		unsigned long			*pValue,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetDelayLimit(
		unsigned long			nDeviceNo,
		unsigned long			nSignal,
		unsigned long			*pValue,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVariableRestriction(
		unsigned long			nDeviceNo,
		unsigned long			*pWidthStep,
		unsigned long			*pHeightStep,
		unsigned long			*pXPosStep,
		unsigned long			*pYPosStep,
		unsigned long			*pWidthMin,
		unsigned long			*pHeightMin,
		unsigned long			*pFreePos,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetColorEnhanceValue(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nPlane,
		long					*pR,
		long					*pG,
		long					*pB,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetColorEnhanceMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetDownloadMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetCamMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMaxFullResoRate(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pRate,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLowLightMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetProgSwitchMode(
		unsigned long			nDeviceNo,
		unsigned long			nSwitchNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetColorTempBase(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pBase,
		unsigned long			*pMax,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetAutoExposureRestriction(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pWidthStep,
		unsigned long			*pHeightStep,
		unsigned long			*pXPosStep,
		unsigned long			*pYPosStep,
		unsigned long			*pWidthMin,
		unsigned long			*pHeightMin,
		unsigned long			*pMaxValue,
		unsigned long			*pMaxRange,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSyncOutTimes(
		unsigned long			nDeviceNo,
		unsigned long			*pRatio,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetStoreNum(
		unsigned long			nDeviceNo,
		unsigned long			*pNum,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetEthernetInfo(
		unsigned long			nDeviceNo,
		unsigned long			nTarget,
		unsigned long			*pValue,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetIRIGPhaseLock(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetIRIGPhaseLockStatus(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetHighSpeedMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetBurstTransfer(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetHeadExchange(
		unsigned long			nDeviceNo,
		unsigned long			*pNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetHeadTypeName(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nHeadNo,
		char					*pStrName,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetStepShutter(
		unsigned long			nDeviceNo,
		unsigned long			*pStep,
		unsigned long			*pMin,
		unsigned long			*pMax,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_GetLiveResolutionMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetCameraCommandBaudRate(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pRate,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetBitDepth(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		char					*pDepth,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetBitDepth2(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		char					*pDepth,
		unsigned long			*pBitSel,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSyncPriority(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVideoOutSignal(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetVideoOutHDSDI(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetRecordingType(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetPixelGainData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		void					*pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetAutoPlay(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetFactoryDefaults(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetStorePreset(
		unsigned long			nDeviceNo,
		unsigned long			*pNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_GetInstructionSet(
		unsigned long			nDeviceNo,
		unsigned long			*pInstSetCode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLEDMode(
		unsigned long			nDeviceNo,
		unsigned long			*pLEDMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetBatteryPowerMode(
		unsigned long			nDeviceNo,
		unsigned long			*pBattery,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_GetSyncInSignalStatus(
		unsigned long			nDeviceNo,
		unsigned long			*pStatus,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLiveIRIGData(
		unsigned long			nDeviceNo,
		PPDC_IRIG_INFO			pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetPolarizerConfig(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetShadingType(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetBlackClipLevel(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSubInterfaceMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetExposeExtension(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetImageTrigger(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetImageTriggerInfo(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pXPos,
		unsigned long			*pYPos,
		unsigned long			*pValue,
		unsigned long			*pRange,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetImageTriggerRestriction(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pWidthStep,
		unsigned long			*pHeightStep,
		unsigned long			*pXPosStep,
		unsigned long			*pYPosStep,
		unsigned long			*pWidthMin,
		unsigned long			*pHeightMin,
		unsigned long			*pMaxValue,
		unsigned long			*pMaxRange,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetImageTriggerConditions(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pConditions,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetRTC(
		unsigned long			nDeviceNo,
		unsigned long			*pYear,
		unsigned long			*pMonth,
		unsigned long			*pDay,
		unsigned long			*pHour,
		unsigned long			*pMinute,
		unsigned long			*pSecond,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemoryModePartition(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLensControl(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pType,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetGetherTransferMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMechaShutterMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

//Hieu ExposeTime
unsigned long WINAPI 
PDC_GetExposeTimeData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		UINT32					*pBuf,
		unsigned long			*pNum,
		unsigned long			*pErrorCode
		);

//hieu recon cmd 20131024
unsigned long WINAPI
PDC_GetCurrentFramesRecorded(
		unsigned long			nDevHandle,
		unsigned long			*pParam,
		unsigned long			*pErrorCode
		);

/*	FAN Control*/
unsigned long WINAPI
PDC_GetFanCtrl(
		unsigned long			nDeviceNo,
		unsigned long			*pCtrl,
		unsigned long			*pUsed,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetFanState(
		unsigned long			nDeviceNo,
		unsigned long			*pState,
		unsigned long			*pOnState,
		unsigned long			*pErrorCode
		);

/*	SD CARD Control*/
unsigned long WINAPI
PDC_GetSdCardCtrl(
		unsigned long 			nDeviceNo,
		unsigned long 			*pCtrl,
		unsigned long 			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSdCardState(
		unsigned long			nDeviceNo,
		unsigned long			nSlotNo,
		unsigned long			*pInOut,
		unsigned long			*pProtect,
		unsigned long			*pFormat,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSdCardInfo(
		unsigned long			nDeviceNo,
		unsigned long			nSlotNo,
		unsigned long			*pSpeedMode,
		unsigned long			*pSpeedClass,
		unsigned long			*pSize,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSdCardDataRecDate(
		unsigned long			nDeviceNo,
		unsigned long			nSlotNo,
		unsigned long			nDataNo,
		unsigned long			*pDate,
		unsigned long			*pTime,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSdCardDataRecFrames(
		unsigned long			nDeviceNo,
		unsigned long			nSlotNo,
		unsigned long			nDataNo,
		unsigned long			*pFrames,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSdCardMemoryMode(
		unsigned long			nDeviceNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetSdCardRunState(
		unsigned long			nDeviceNo,
		unsigned long			nSlotNo,
		unsigned long			*pState,
		unsigned long			*pOption,
		unsigned long			*pErrorCode
		);

/*
 *	Get Recorded status
 */
unsigned long WINAPI
PDC_GetMemIRIG(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemMCDL(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemRecordRate(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pRate,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemTriggerMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pAFrames,
		unsigned long			*pRFrames,
		unsigned long			*pRCount,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemResolution(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemResolutionROI(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pWidth,
		unsigned long			*pHeight,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemShutterSpeedFps(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pFps,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemSensorGainMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemSensorGammaMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemColorTempMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pR,
		unsigned long			*pG,
		unsigned long			*pB,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemAutoExposure(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemLUTMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemDSShutterMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemDSShutterValue(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pValue,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemEdgeEnhanceMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetTransferOption(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*p8BitSel,
		unsigned long			*pBayer,
		unsigned long			*pInterleave,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemFrameInfo(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		PPDC_FRAME_INFO			pFrame,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemColorEnhanceValue(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nPlane,
		long					*pR,
		long					*pG,
		long					*pB,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemColorEnhanceMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemIRIGPhaseLockStatus(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_GetMemLiveResolutionMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemBitDepth(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		char					*pDepth,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemBitDepth2(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		char					*pDepth,
		unsigned long			*pBitSel,
		unsigned long			*pErrorCode
		);

/*
 *	Set Device status
 */
unsigned long WINAPI
PDC_SetActiveChild(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetStatus(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetRecReady(
		unsigned long			nDeviceNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetEndless(
		unsigned long			nDeviceNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_TriggerIn(
		unsigned long			nDeviceNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetShutterLockMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetResolutionLockMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetPartitionIncMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetKeyPadLock(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetVideoOutMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetExternalInMode(
		unsigned long			nDeviceNo,
		unsigned long			nPort,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetExternalOutMode(
		unsigned long			nDeviceNo,
		unsigned long			nPort,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetIRIG(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetMCDL(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetIRIGOffset(
		unsigned long			nDeviceNo,
		long					nValue,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetRecordRate(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nRate,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetTriggerMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			nAFrames,
		unsigned long			nRFrames,
		unsigned long			nRCount,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetResolution(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nWidth,
		unsigned long			nHeight,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetResolutionROI(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nWidth,
		unsigned long			nHeight,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetShutterSpeedFps(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nFps,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetCurrentPartition(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetSensorGainMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetSensorGammaMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetColorTempMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			nR,
		unsigned long			nG,
		unsigned long			nB,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetSoftColorTempMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			nR,
		unsigned long			nG,
		unsigned long			nB,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetAutoExposure(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetLUTMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetDSShutterMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetDSShutterValue(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nValue,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetEdgeEnhanceMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetShadingMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetPixelGainMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetTransferOption(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			n8BitSel,
		unsigned long			nBayer,
		unsigned long			nInterleave,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_EraseVariableChannel(
		unsigned long			nDeviceNo,
		unsigned long			nChannel,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetVariableChannel(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nChannel,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetVariableChannelInfo(
		unsigned long			nDeviceNo,
		unsigned long			nChannel,
		unsigned long			nRate,
		unsigned long			nWidth,
		unsigned long			nHeight,
		unsigned long			nXPos,
		unsigned long			nYPos,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetAutoExposureInfo(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nWidth,
		unsigned long			nHeight,
		unsigned long			nXPos,
		unsigned long			nYPos,
		unsigned long			nValue,
		unsigned long			nRange,
		unsigned long			nLimit,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetLUTUserParams(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nLUTMode,
		PPDC_LUT_PARAMS			pParams,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_CreatePixelGainData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		void					*pSrc,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetPixelGainData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		void					*pSrc,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetLUTUser(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetDelay(
		unsigned long			nDeviceNo,
		unsigned long			nSignal,
		unsigned long			nValue,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetColorEnhanceMode(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetDownloadMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetLowLightMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetProgSwitchMode(
		unsigned long			nDeviceNo,
		unsigned long			nSwitchNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetSyncOutTimes(
		unsigned long			nDeviceNo,
		unsigned long			nRatio,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetStore(
		unsigned long			nDeviceNo,
		unsigned long			nNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetEthernetInfo(
		unsigned long			nDeviceNo,
		unsigned long			nTarget,
		unsigned long			nValue,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetIRIGPhaseLock(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetHighSpeedMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetBurstTransfer(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetHeadExchange(
		unsigned long			nDeviceNo,
		unsigned long			nNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_SetLiveResolutionMode( 
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetColorType(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		char					nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetBitDepth(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		char					nDepth,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetBitDepth2(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		char					nDepth,
		unsigned long			nBitSel,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetCameraCommandBaudRate(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nRate,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetSegmentPosition(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nXPos,
		unsigned long			nYPos,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetSyncPriority(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetDeviceID(
		unsigned long			nDeviceNo,
		unsigned long			nID,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetVideoOutSignal(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetVideoOutHDSDI(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetRecordingType(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetAutoPlay(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetFactoryDefaults(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetStorePreset(
		unsigned long			nDeviceNo,
		unsigned long			nNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetInstructionSet(
		unsigned long			nDeviceNo,
		unsigned long			nInstSetCode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_SetLEDMode(
		unsigned long			nDeviceNo,
		unsigned long			nLEDMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_SetBatteryPowerMode(
		unsigned long			nDeviceNo,
		unsigned long			nBattery,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetBatteryFunction(
		unsigned long nDeviceNo,
		unsigned long nValue,
		unsigned long *pErrorCode
		);

unsigned long WINAPI
PDC_SetKeyPadCommand(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetShadingType(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetBlackClipLevel(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetExposeExtension(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetImageTrigger(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetImageTriggerInfo(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nWidth,
		unsigned long			nHeight,
		unsigned long			nXPos,
		unsigned long			nYPos,
		unsigned long			nValue,
		unsigned long			nRange,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetRTC(
		unsigned long			nDeviceNo,
		unsigned long			nYear,
		unsigned long			nMonth,
		unsigned long			nDay,
		unsigned long			nHour,
		unsigned long			nMinute,
		unsigned long			nSecond,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetLensControlData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nSendDataSize,
		char					*pSendData,
		unsigned long			*pRecvDataSize,
		char					*pRecvData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetMemoryModePartition(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_SetGEtherTransferMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_SetMechaShutterMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetMechaShutterFunction(
		unsigned long			nDeviceNo,
		unsigned long			nValue,
		unsigned long			*pErrorCode
		);

/*	Fan control	*/
unsigned long WINAPI
PDC_SetFanCtrl(
		unsigned long			nDeviceNo,
		unsigned long			nValue,
		unsigned long			*pErrorCode
		);

/*	SD card control	*/
unsigned long WINAPI
PDC_SetSdCardMemoryMode(
		unsigned long			nDeviceNo,
		unsigned long			nMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetSdCardFormatRequest(
		unsigned long			nDeviceNo,
		unsigned long			nSlotNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetSdCardCancelRequest(
		unsigned long			nDeviceNo,
		unsigned long			nSlotNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetSdCardResetRequest(
		unsigned long			nDeviceNo,
		unsigned long			nSlotNo,
		unsigned long			*pErrorCode
		);
unsigned long WINAPI
PDC_SetSdCardSaveRequest(
		unsigned long			nDeviceNo,
		unsigned long			nSlotNo,
		unsigned long			*pErrorCode
		);

/*
 *	Get Image
 */
unsigned long WINAPI
PDC_GetLiveImageData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nBitDepth,
		void					*pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLiveImageAddress(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		void					**pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemImageData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		unsigned long			nBitDepth,
		void					*pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemImageDataStart(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		unsigned long			nBitDepth,
		void					*pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemImageDataEnd(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nBitDepth,
		void					*pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemImageAddress(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		void					**pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemIRIGData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		PPDC_IRIG_INFO			pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemMCDLData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		PPDC_MCDL_INFO			pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemIRIGandMCDLData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		unsigned long			nFrameNum,
		PPDC_IRIGMCDL_INFO		pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemMCDLByteData(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		unsigned long			nFrameNum,
		void					*pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_DrawLiveImage(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		HDC						hDC,
		unsigned long			nDisplayMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_DrawMemImage(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		HDC						hDC,
		unsigned long			nDisplayMode,
		long					nFrameNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLiveMemImageData(
						unsigned long			nDeviceNo,
						unsigned long			nChildNo,
						unsigned long			nBitDepth,
						void					*pData,
						unsigned long			*pErrorCode
						);

/*
 *	FPGA Setting
 */
unsigned long WINAPI
PDC_FPGAConfig(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		LPCTSTR					lpszFileName,
		unsigned long			nProgramMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetFPGAStatus(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pStatus,
		unsigned long 			*pBytesCompleted,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_LoadFPGAConfigFile(
		unsigned long	nDeviceNo,
		LPCTSTR			lpszPath,
		unsigned char	*pInfo,
		unsigned long	nSize,
		unsigned long	*pErrorCode
		);

unsigned long WINAPI
PDC_SetReset_FPGA(
		unsigned long nDeviceNo,
		unsigned long nEnable,
		unsigned long nFPGAType,
		unsigned long *pErrorCode
		);

/*
 *	File Save
 */
unsigned long WINAPI
PDC_BMPFileSave(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		LPCTSTR					lpszFileName,
		long					nFrameNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_AVIFileSaveOpen(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		LPCTSTR					lpszFileName,
		long					nRate,
		unsigned long			nShowCompressDlg,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_AVIFileSave(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		unsigned long			*pSize,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_AVIFileSaveClose(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_MRAWFileSaveOpen(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		LPCTSTR					lpszFileName,
		unsigned long			nMRawBitDepth,
		long					nMaxFrameNum,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_MRAWFileSave(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_MRAWFileSaveClose(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo, 
		unsigned long			*pErrorCode
		);

/*
 *	File Load
 */
unsigned long WINAPI 
PDC_MRAWFileLoadOpen(
		LPCTSTR					lpszFileName,
		PPDC_IMAGE_BUFF_INFO	pInfo,
		unsigned long			nMRawBitDepth,
		unsigned long			*pFileNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_MRAWFileLoadOpen2(
		LPCTSTR					lpszFileName,
		PPDC_IMAGE_BUFF_INFO	pInfo,
		unsigned long			nMRawBitDepth,
		unsigned long			*pSessionNo,
		PPDC_ORG_IMAGE_INFO		pOrgInfo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_MRAWFileLoad(
		unsigned long			nFileNo,
		long					nFrame,
		void					*pBuff,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_MRAWFileLoadClose(
		unsigned long			nFileNo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_RAWFileLoad(
		LPCTSTR					lpszFileName,
		PPDC_IMAGE_BUFF_INFO	pInfo,
		unsigned long			nRawBitDepth,
		void					*pBuff,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_RAWFileLoad2(
		LPCTSTR					lpszFileName,
		PPDC_IMAGE_BUFF_INFO	pInfo,
		unsigned long			nRawBitDepth,
		void					*pBuff,
		PPDC_ORG_IMAGE_INFO		pOrgInfo,
		unsigned long			*pErrorCode
		);

/*
 *	Camera Link Control
 */
unsigned long WINAPI
PDC_ReadCameraDefFile(
		unsigned long			nDeviceNo,
		LPCTSTR					lpszFileName,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_WriteCameraDefFile(
		unsigned long			nDeviceNo,
		LPCTSTR					lpszFileName,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetCameraCommand(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned char			*pBuf,
		unsigned long			nSize,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_GetCameraLinkStatus(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pMode,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_GetBayerAlignment(
	    unsigned long			nDeviceNo,
		unsigned long			*pBayerAlign,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PDC_SetBayerAlignment(
	    unsigned long			nDeviceNo,
		unsigned long			nBayerAlign,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemory(
		unsigned long			nDeviceNo,
		unsigned long			*pBlockAvailable,
		unsigned long			*pBlockReserved,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_SetMemory(
		unsigned long			nDeviceNo,
		unsigned long			nOption,
		unsigned long			nBlocks,
		unsigned long			nFramePerBlock,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetNumofDropFrame(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pDropFrame,
		unsigned long			*pErrorCode
		);

#if _MSC_VER < 1300
unsigned long WINAPI
PDC_GetMemoryInfo(
		unsigned long nDeviceNo,
		unsigned __int64 *pAvailableMemory,
		unsigned __int64 *pReservedMemory,
		unsigned long *pErrorCode
		);
#else
unsigned long WINAPI
PDC_GetMemoryInfo(
		unsigned long nDeviceNo,
		unsigned long long		*pAvailableMemory,
		unsigned long long		*pReservedMemory,
		unsigned long			*pErrorCode
		);
#endif

unsigned long WINAPI
PDC_GetDropFrameList(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		PPDC_DROP_FRAME_LIST	pList,
		unsigned long			*pSize,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetLiveImageAddress2(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			*pFrameNo,
		void					**pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemImageData2(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		unsigned long			nBitDepth,
		unsigned long			nPort,
		void					*pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemImageDataStart2(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		long					nFrameNo,
		unsigned long			nBitDepth,
		unsigned long			nPort,
		void					*pData,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PDC_GetMemImageDataEnd2(
		unsigned long			nDeviceNo,
		unsigned long			nChildNo,
		unsigned long			nBitDepth,
		unsigned long			nPort,
		void					*pData,
		unsigned long			*pErrorCode
		);

/*
 *	Image Processing
 */
unsigned long WINAPI
PDC_ConvertBayerImageData(
		PPDC_IMAGE_BUFF_INFO	pInfo, 
		PPDC_BAYER_CONVERT_INFO	pBayerInfo,
		void					*pSrc,
		void					*pDes,
		unsigned long			*pErrorCode
		);


#ifdef	__cplusplus
}
#endif	/*	__cplusplus	*/

#endif	/*	_PDC_FUNC_H_	*/

