/**
 * sisoIoBmpErrorCodes.h
 *
 * Copyright (c) 2002-2017 Silicon Software GmbH, All Rights Reserved.
 *
 * \file sisoIoBmpErrorCodes.h
 *
 * \brief Definitions of error codes for parsing BMP files
 *
 * \author Silicon Software GmbH
 */
 
#pragma once

enum BMP_ERRORS
{
	ERR_BMP_NO_ERROR                              = 0,/**< */
	ERR_BMP_UNDEFINED                             = -1000000001,/**< */
	ERR_BMP_BIT_FORMAT_UNKNOWN                    = -1000000002,/**< */
	ERR_BMP_UNKNOWN_BIT_WIDTH                     = -1000000003,/**< */
	ERR_BMP_INVALID_SIZE                          = -1000000004,/**< */
	ERR_BMP_CANNOT_HANDLE_4_BIT_IMAGES            = -1000000005,/**< */
	ERR_BMP_CANNOT_HANDLE_INDEXED_COLORS          = -1000000006,/**< */
	ERR_BMP_CANNOT_HANDLE_ALPHA_CHANNELS          = -1000000007,/**< */
	ERR_BMP_CANNOT_HANDLE_COMPRESSED_IMAGES       = -1000000008,/**< */
	ERR_BMP_LINE_LENGTH_IS_NOT_MULTIPLE_OF_8      = -1000000009/**< */

};
