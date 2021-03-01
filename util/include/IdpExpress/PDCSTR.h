/*
 *	PDCSTR.h
 *	Photron Device Control SDK
 *	Structure Definition
 *
 *	Copyright (C) 2006-2012 PHOTRON LIMITED
 */

#include	"PDCVALUE.h"
#include	<vfw.h>

#ifndef	_PDC_STR_H_
#define	_PDC_STR_H_

#pragma	pack(4)
/*
 *	Device detect structure
 */
typedef struct {
	/*	Device code	*/
	unsigned long	m_nDeviceCode;
	/*	Virtual device number(IP-address)	*/
	unsigned long	m_nTmpDeviceNo;
	/*	Interface code	*/
	unsigned long	m_nInterfaceCode;
} PDC_DETECT_INFO, *PPDC_DETECT_INFO;

/*
 *	Device detect integration structure
 */
typedef struct {
	/*	Detected device num	*/
	unsigned long	m_nDeviceNum;
	/*	Device detect structure	*/
	PDC_DETECT_INFO	m_DetectInfo[PDC_MAX_DEVICE];
} PDC_DETECT_NUM_INFO, *PPDC_DETECT_NUM_INFO;

/*
 *	Frame information
 */
typedef struct {
	long			m_nStart;	/*	Recording start frame number	*/
	long			m_nEnd;		/*	Recording end frame number	*/
	long			m_nTrigger;	/*	Trigger frame number	*/
	long			m_nTwoStageLowToHigh;	/*	Twostage LOW->HIGH frame number	*/
	long			m_nTwoStageHighToLow;	/*	Twostage HIGH->LOW frame number	*/
	unsigned long	m_nTwoStageTiming;		/*	Twostage LOW->HIGH timming	*/
	long			m_nEvent[10];		/*	Event frame number	*/
	unsigned long	m_nEventCount;		/*	Event frame count	*/
	unsigned long	m_nRecordedFrames;		/*	Recording frame quantity	*/
} PDC_FRAME_INFO, *PPDC_FRAME_INFO;

/*
 *	Image data information
 */
typedef struct {
	long			m_nWidth;
	long			m_nHeight;
	long			m_nBytePerLine;
	unsigned short	m_nRealBitPerPixel;
	long			m_nInterleave;	/* Buffer Interleave	*/
} PDC_IMAGE_BUFF_INFO, *PPDC_IMAGE_BUFF_INFO;

/*	Bayer Convert Information	*/
typedef struct {
	unsigned long	m_nMode;
	unsigned long	m_nDeviceCode;
	void*			m_pLUTR;
	void*			m_pLUTG;
	void*			m_pLUTB;
	unsigned long	m_nOutputInterleave;
	unsigned long	m_nEdgeEnhance;
	unsigned long	m_nColorTempR;
	unsigned long	m_nColorTempG;
	unsigned long	m_nColorTempB;
	long			m_nColorMatrixRR;
	long			m_nColorMatrixRG;
	long			m_nColorMatrixRB;
	long			m_nColorMatrixGR;
	long			m_nColorMatrixGG;
	long			m_nColorMatrixGB;
	long			m_nColorMatrixBR;
	long			m_nColorMatrixBG;
	long			m_nColorMatrixBB;
} PDC_BAYER_CONVERT_INFO, *PPDC_BAYER_CONVERT_INFO;

/*
 *	IRIG information
 */
typedef struct {
	unsigned long	m_nDayOfYear;	//	Day
	unsigned char	m_nHour;		//	Hour
	unsigned char	m_nMinute;		//	Minute
	unsigned char	m_nSecond;		//	Second
	unsigned long	m_nMicroSecond;	//	Micro second
	unsigned char	m_ExistSignal;	//	Presence of signal
	unsigned long	m_Reserve;		//	Reserved
} PDC_IRIG_INFO, *PPDC_IRIG_INFO;

/*
 *	MCDL information
 */
typedef struct {
	unsigned char	m_nDigital[10];	/*	Digital	*/
	double			m_nAnalogA[10];	/*	AnalogA	*/
	double			m_nAnalogB[10];	/*	AnalogB	*/
	double			m_nAnalogC[10];	/*	AnalogC	*/
	double			m_nAnalogD[10];	/*	AnalogD	*/
} PDC_MCDL_INFO, *PPDC_MCDL_INFO;

/*
 *	IRIG / MCDL information
 */
typedef struct {
	PDC_IRIG_INFO	m_IRIGInfo;	/*	IRIG	*/
	PDC_MCDL_INFO	m_MCDLInfo;	/*	MCDL	*/
} PDC_IRIGMCDL_INFO, *PPDC_IRIGMCDL_INFO;

/*
 *	Pre-LUT information
 */
typedef struct {
	long			m_nBrightnessR;
	long			m_nBrightnessG;
	long			m_nBrightnessB;
	long			m_nContrastR;
	long			m_nContrastG;
	long			m_nContrastB;
	double			m_nGainR;
	double			m_nGainG;
	double			m_nGainB;
	double			m_nGammaR;
	double			m_nGammaG;
	double			m_nGammaB;
	unsigned char	m_nPosiNegaR;
	unsigned char	m_nPosiNegaG;
	unsigned char	m_nPosiNegaB;
	unsigned char	m_nFreeMode;
} PDC_LUT_PARAMS, *PPDC_LUT_PARAMS;

/*
 *	AVI Frame Parameter
 */
typedef struct {
	long			m_nStart;
	long			m_nEnd;
	long			m_nTotal;
	long			m_nArea_start;
	long			m_nArea_end;
	long			m_nArea_total;
	long			m_nCurrent;
	long			m_nTrigger;
	long			m_nKey;
} PDC_AVI_FRAME_PARAMS, *PPDC_AVI_FRAME_PARAMS;

/*
 *	drop Frame List
 */
typedef struct pdc_drop_frame_list
{
  long			m_nFrame;		/* drop start frame number */
  unsigned long	m_nDropFrame;	/* number of dropped frame */
} PDC_DROP_FRAME_LIST, *PPDC_DROP_FRAME_LIST;

/* 
 * Original Image data Information
 */
typedef struct {
	unsigned long	m_nDeviceCode;
	unsigned long	m_nColorType;
	unsigned short	m_nBitDepth;
	unsigned long	m_nEffectiveBits;
	unsigned long	m_n8BitSel;
} PDC_ORG_IMAGE_INFO, *PPDC_ORG_IMAGE_INFO;

#pragma pack()
#endif	/*	_PDC_STR_H_	*/

