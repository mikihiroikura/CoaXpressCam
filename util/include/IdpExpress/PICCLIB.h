/*
 *	PICCLIB.h
 *	PHOTRON Image Control SDK (Compact version)
 *
 *	Copyright (C) 2012 PHOTRON LIMITED
 */

#ifndef	_PICLIB_H_
#define	_PICLIB_H_

//#include	<vfw.h>

#ifdef	__cplusplus
extern "C" {
#endif	/*	__cplusplus	*/

/*	Return Value	*/
#define	PIC_SUCCEEDED				1	/*	����	*/
#define	PIC_FAILED					0	/*	���s	*/

/*	Error code	*/
#define	PIC_ERROR_NOERROR			1	/*	����	*/
#define	PIC_ERROR_UNINITIALIZE		2	/*	��������	*/
#define	PIC_ERROR_ILLEGAL_VALUE		3	/*	�p�����[�^���s��	*/
#define	PIC_ERROR_LIBRARY_ERROR		4	/*	���C�u�����ُ�	*/
#define	PIC_ERROR_INITIALIZED		6	/*	���ɏ����ς�	*/
#define	PIC_ERROR_ILLEGAL_STRUCTURE	7	/*	���\���̂��s��	*/
#define	PIC_ERROR_AVI_NOT_OPENED	8	/*	AVI�쐬���I�[�v�����Ă��Ȃ�	*/
#define	PIC_ERROR_TIMEOUT			9	/*	�^�C���A�E�g	*/
#define	PIC_ERROR_FILEWRITE_FAILED	12	/*	�������݃G���[	*/
#define PIC_ERROR_FILEREAD_FAILED	13	/*  �ǂݍ��݃G���[  */
#define	PIC_ERROR_FILEOPENED		16	/*	�I�[�v���ς�	*/
#define	PIC_ERROR_ILLEGAL_DATA		17	/*	�ݒ�f�[�^���s��	*/
#define	PIC_ERROR_FILE_EXIST		18	/*	���ɂ��̃t�@�C�������݂��Ă���	*/
#define	PIC_ERROR_NOT_SUPPORTED		19	/*	���T�|�[�g	*/
#define	PIC_ERROR_NOT_REGISTERED	30	/*	MPEG�I�v�V�����̃��C�Z���X���Ȃ�	*/
#define	PIC_ERROR_MPEG_NOT_OPENED	31	/*	MPEG�쐬���I�[�v�����Ă��Ȃ�	*/
#define PIC_ERROR_FILEREAD_NOT_SUPPORTED	40	/*	�Ή����Ă��Ȃ��t�@�C���\�� (�p���b�g�`����)	*/
#define PIC_ERROR_FILEREAD_BUFFSIZE	41	/*	���\���̂ɋL�ڂ���Ă���摜�o�b�t�@�T�C�Y���t�@�C���w�b�_�ɋL�ڂ���Ă���T�C�Y�ƈ�v���Ȃ�	*/

/*	Image Buffer Information	*/
typedef struct {
	long			m_nWidth;
	long			m_nHeight;
	long			m_nBytePerLine;
	unsigned short	m_nRealBitPerPixel;
	long			m_nInterleave;	/* Buffer Interleave	*/
} PIC_IMAGE_BUFF_INFO, *PPIC_IMAGE_BUFF_INFO;

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
	unsigned long	m_nBlackClip;
} PIC_BAYER_CONVERT_INFO, *PPIC_BAYER_CONVERT_INFO;

/* 
 * Original Image data Information
 */
typedef struct {
	unsigned long	m_nDeviceCode;
	unsigned long	m_nColorType;
	unsigned short	m_nBitDepth;
	unsigned long	m_nEffectiveBits;
	unsigned long	m_n8BitSel;
} PIC_ORG_IMAGE_INFO, *PPIC_ORG_IMAGE_INFO;

/*	Data Interleave	*/
#define	PIC_COLORDATA_NOCOLOR				0
#define	PIC_COLORDATA_INTERLEAVE_BGR		1
#define	PIC_COLORDATA_INTERLEAVE_RGB		2

/*	Effective Bit	*/
#define	PIC_EFFECTIVE_BITS_HIGH				0
#define	PIC_EFFECTIVE_BITS_LOW				1

/*	BayerConvert	*/
#define	PIC_BAYERCONVERT_MODE1				0
#define	PIC_BAYERCONVERT_MODE2				1
#define	PIC_BAYERCONVERT_MODE3				2

/*	EdgeEnhance	*/
#define	PIC_EDGE_ENHANCE_OFF				1
#define	PIC_EDGE_ENHANCE_MODE1				2
#define	PIC_EDGE_ENHANCE_MODE2				3
#define	PIC_EDGE_ENHANCE_MODE3				4

/*	BlackClip	*/
#define	PIC_BLACK_CLIP_LEVEL_OFF			0
#define	PIC_BLACK_CLIP_LEVEL_1				1
#define	PIC_BLACK_CLIP_LEVEL_2				2
#define	PIC_BLACK_CLIP_LEVEL_3				3
#define	PIC_BLACK_CLIP_LEVEL_4				4
#define	PIC_BLACK_CLIP_LEVEL_5				5
#define	PIC_BLACK_CLIP_LEVEL_6				6
#define	PIC_BLACK_CLIP_LEVEL_7				7
#define	PIC_BLACK_CLIP_LEVEL_8				8


/* CPU���߃Z�b�g���󂯂邽�߂̌^ */
typedef UINT32	PIC_INSTRUCTIONSET;

/* CPU���߃Z�b�g�̎��ʎq */
// �ŏ���2bit�͎w�肵�����߃Z�b�g����Ή��������Ƃ��̃f�t�H���g�̎w��t���O
// �c���bit�͖��߃Z�b�g�̎w��t���O
#define PIC_INSTSET_NONE	0x80000000
#define PIC_INSTSET_AUTO	0x40000000
#define PIC_INSTSET_MMX		0x00000001
#define PIC_INSTSET_SSE		0x00000002
#define PIC_INSTSET_SSE2	0x00000004
#define PIC_INSTSET_3DNOW	0x00000010
#define PIC_INSTSET_E3DNOW	0x00000020


typedef struct
{
	void				*raw_info;
	PIC_IMAGE_BUFF_INFO	image_info;
	int					num_page;
} PIC_MRAW;

typedef enum tag_PIC_TIFF_BITDEPTH
{
	//PIC_TIFF_BITDEPTH_8,
	PIC_TIFF_BITDEPTH_10,
	PIC_TIFF_BITDEPTH_12,
	PIC_TIFF_BITDEPTH_16,
} PIC_TIFF_BITDEPTH;

typedef enum tag_PIC_RAW_BITDEPTH
{
	PIC_RAW_BITDEPTH_8,
	PIC_RAW_BITDEPTH_10,
	PIC_RAW_BITDEPTH_12,
	PIC_RAW_BITDEPTH_16,
} PIC_RAW_BITDEPTH;

/*
 *	File Save
 */
unsigned long WINAPI
PIC_Init(
		unsigned long			*pErrorCode
		);


unsigned long WINAPI
PIC_RAWFileSave(
		LPCTSTR					lpszFileName,
		PPIC_IMAGE_BUFF_INFO	pInfo,
		unsigned char			*pBuff,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PIC_RAW16FileSave(
		LPCTSTR					lpszFileName,
		PPIC_IMAGE_BUFF_INFO	pInfo,
		unsigned short			*pBuff,
		unsigned long			nEffectiveBit,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PIC_MRAWFileSaveOpen(
		LPCTSTR					lpszFileName,
		PPIC_IMAGE_BUFF_INFO	pInfo,
		long					nMaxFrameNum,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PIC_MRAWFileSave(
		unsigned char			*pBuff,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PIC_MRAW16FileSaveOpen(
		LPCTSTR					lpszFileName,
		PPIC_IMAGE_BUFF_INFO	pInfo,
		PIC_RAW_BITDEPTH		pic_depth,
		unsigned long			nEffectiveBit,
		long					nMaxFrameNum,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PIC_MRAW16FileSave(
		unsigned short			*pBuff,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PIC_MRAWFileSaveClose(
		unsigned long			*pErrorCode
		);


/*
 *	File Load
 */
unsigned long WINAPI
PIC_RAWFileLoad(
		LPCTSTR					lpszFileName,
		PIC_IMAGE_BUFF_INFO		*pInfo,
		PIC_RAW_BITDEPTH		pic_depth,
		void					*pBuff,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PIC_RAWFileLoad2(
		LPCTSTR					lpszFileName,
		PIC_IMAGE_BUFF_INFO		*pInfo,
		PIC_RAW_BITDEPTH		pic_depth,
		void					*pBuff,
		PIC_ORG_IMAGE_INFO		*pOrgInfo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PIC_MRAWFileLoadOpen(
		LPCTSTR					lpszFileName,
		PIC_IMAGE_BUFF_INFO		*pInfo,
		PIC_MRAW				*pPicMRaw,
		PIC_RAW_BITDEPTH		pic_depth,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI
PIC_MRAWFileLoadOpen2(
		LPCTSTR					lpszFileName,
		PIC_IMAGE_BUFF_INFO		*pInfo,
		PIC_MRAW				*pPicMRaw,
		PIC_RAW_BITDEPTH		pic_depth,
		PIC_ORG_IMAGE_INFO		*pOrgInfo,
		unsigned long			*pErrorCode
		);

unsigned long WINAPI 
PIC_MRAWFileLoad(
		PIC_MRAW		*pPicMRaw,
		void			*pBuff,
		long			nFrame,
		unsigned long	*pErrorCode
		);

unsigned long WINAPI 
PIC_MRAWFileLoadClose(
		PIC_MRAW		*pPicMRaw,
		unsigned long	*pErrorCode
		);

/*
 *  CPU Instruction Set
*/
unsigned long WINAPI 
PIC_SetInstructionSet(
		PIC_INSTRUCTIONSET		instset,
		unsigned long			*pErrorCode
		);

/*
 *	Filter
 */
unsigned long WINAPI
PIC_Fil_BayerConvert(
		PPIC_IMAGE_BUFF_INFO	pInfo,
		void					*pSrc,
		void					*pDes,
		PPIC_BAYER_CONVERT_INFO	pBayerInfo,
		unsigned long			*pErrorCode
		);

#ifdef	__cplusplus
}
#endif	/*	__cplusplus	*/

#endif	/*	_PICLIB_H_	*/

