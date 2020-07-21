/*
 *	PDCERROR.h
 *	Photron Device Control SDK
 *	Error Code
 *
 *	Copyright (C) 2006-2012 PHOTRON LIMITED
 */

#ifndef	_PDC_ERROR_H_
#define	_PDC_ERROR_H_

/*	Return Value	*/
#define	PDC_SUCCEEDED				1
#define	PDC_FAILED					0

/*	Error Code	*/
#define	PDC_ERROR_NOERROR			1	/*	Normal	*/
#define	PDC_ERROR_UNINITIALIZE		2	/*	Uninitialize	*/
#define	PDC_ERROR_ILLEGAL_DEV_NO	3	/*	Illegal device number	*/
#define	PDC_ERROR_ILLEGAL_CHILD_NO	4	/*	Illegal child number	*/
#define	PDC_ERROR_ILLEGAL_VALUE		5	/*	Illegal parameter	*/
#define	PDC_ERROR_ALLOCATE_FAILED	6	/*	Memory allocate failed	*/
#define	PDC_ERROR_INITIALIZED		7	/*	Initialized	*/
#define	PDC_ERROR_NO_DEVICE			8	/*	Device not found	*/
#define	PDC_ERROR_TIMEOUT			9	/*	Time out	*/
#define	PDC_ERROR_FUNCTION_FAILED	10	/*	Function failed	*/
#define	PDC_ERROR_FUNCTION_DISABLE	11	/*	Function disabled	*/
#define	PDC_ERROR_NO_DATA			12	/*	Recorded data does not exist	*/
#define	PDC_ERROR_UNKNOWN_FRAME		13	/*	Unknown frame number	*/
#define	PDC_ERROR_CAMERAMODE		14	/*	Camera mode error	*/
#define	PDC_ERROR_NO_ENDLESS		15	/*	No endress mode recording	*/
#define	PDC_ERROR_FILEREAD_FAILED	16	/*	File read failed	*/
#define	PDC_ERROR_FILEWRITE_FAILED	17	/*	File write failed	*/
#define	PDC_ERROR_IMAGE_SIZEOVER	18	/*	Image size error	*/
#define	PDC_ERROR_FRAME_AREAOVER	19	/*	Frame area error	*/
#define	PDC_ERROR_PLAYMODE			20	/*	Play mode error	*/
#define	PDC_ERROR_NOT_SUPPORTED		21	/*	Not supported	*/
#define	PDC_ERROR_DROP_FRAME		22	/*	Frame is not captured	*/
#define PDC_ERROR_FILE_OPEN_ALREADY	23	/*	File has been already opened	*/
#define	PDC_ERROR_FILE_NOTOPEN		24	/*	File is not opened	*/

#define	PDC_ERROR_LOAD_FAILED		100	/*	Module load failed	*/
#define	PDC_ERROR_FREE_FAILED		101	/*	Module free failed	*/
#define	PDC_ERROR_LOADED			102	/*	Module loaded	*/
#define	PDC_ERROR_NOTLOADED			103	/*	Module is not loaded	*/
#define	PDC_ERROR_UNDETECTED		104	/*	Device not yet detect	*/
#define	PDC_ERROR_OVER_DEVICE		105	/*	Device kind is too multi	*/
#define	PDC_ERROR_INIT_FAILED		106	/*	Initialize failed	*/
#define	PDC_ERROR_OPEN_ALREADY		107	/*	Device is opened	*/
#define	PDC_ERROR_NOTOPEN			108	/*	Device is not opened	*/
#define	PDC_ERROR_LIVEONLY			110	/*	Not become LIVE mode	*/
#define	PDC_ERROR_PLAYBACKONLY		111	/*	Not become PLAYBACK mode	*/

#define	PDC_ERROR_SEND_ERROR		200	/*	Send error	*/
#define	PDC_ERROR_RECEIVE_ERROR		201	/*	Receive error	*/
#define	PDC_ERROR_CLEAR_ERROR		202	/*	Initialize error	*/
#define	PDC_ERROR_COMMAND_ERROR		203	/*	Command error	*/

#define	PDC_ERROR_COMPARE_DATA_ERROR	204	/*	Compare data error	*/

#define PDC_ERROR_ENABLE_CONFIG_ERROR	300	/* Enable Configuration FPGA Failed	*/
#define	PDC_ERROR_REG_ACCESS_ERROR	301	/*	Register Access Failed	*/

#endif	/*	_PDC_ERROR_H_	*/

