/**
 * sisoIoTiffErrorCodes.h
 *
 * Copyright (c) 2002-2017 Silicon Software GmbH, All Rights Reserved.
 *
 * \file sisoIoTiffErrorCodes.h
 *
 * \brief Header file for all Tiff Error codes.
 *
 * \author Silicon Software GmbH
 */
 
#pragma once

enum TIF_ERRORS
{
	ERR_TIF_NO_ERROR                                    =  0,/**< no error*//* */
	ERR_TIF_UNDEFINED                                   = -10000001,/**< error which is not listed below*/
	ERR_TIF_UNDEFINED_PHOTOMETRIC_INTERPRETATION        = -10000002,/**<  */
	ERR_TIF_IMAGE_WIDTH_NDEF                            = -10000003,/**<  */
	ERR_TIF_IMAGE_HEIGHT_NDEF                           = -10000004,/**<  */
	ERR_TIF_IMAGE_BIT_PER_SAMPLE_NDEF                   = -10000005,/**<  */
	ERR_TIF_IMAGE_SAMPLE_PER_PIXEL_NDEF                 = -10000006,/**<  */
	ERR_TIF_BITS_PER_COMPONENT                          = -10000007,/**<  */
	ERR_TIF_READ_ENCODED_STRIP                          = -10000008,/**<  */
	ERR_TIF_SCANLINE                                    = -10000009,/**<  */
	ERR_TIF_LINE_LENGTH_IS_NOT_MULTIPLE_OF_8            = -10000010,/**<  */
	ERR_TIF_INVALID_ARG                                 = -10000090 /**< function called by using an invalid argument*/
};
