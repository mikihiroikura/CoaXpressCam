/**
 * sisoIoFileErrorCodes.h
 *
 * Copyright (c) 2002-2017 Silicon Software GmbH, All Rights Reserved.
 *
 * \file sisoIoFileErrorCodes.h
 *
 * \brief Header file for all File Error codes.
 *
 * \author Silicon Software GmbH
 */

#pragma once

enum FILE_IO_ERRORS
{

	ERR_FIO_NO_ERROR                               =  0,/**< Reading file successfull */
	ERR_FIO_UNDEFINED                              = -1000001,/**< other error, whichis not listed below */
	ERR_FIO_OPEN_FILE                              = -1000002,/**< */
	ERR_FIO_WRITE_FILE                             = -1000003,/**< */
	ERR_FIO_READ_FILE                              = -1000006,/**< */
	ERR_FIO_FORMAT_UNKNOWN                         = -1000007,/**< */
	ERR_FIO_NO_DATA_TO_SAVE                        = -1000008,/**< */
	ERR_FIO_FILE_DOES_NOT_EXIST                    = -1000009,/**< file not found*/
	ERR_FIO_HANDLE_IS_NULL                         = -1000010,/**< */
	ERR_FIO_CODE_UNTOUCHED                         = -1000011,/**< */
	ERR_FIO_NO_DATA_TO_READ                        = -1000012,/**< */
	ERR_FIO_CLOSE_HANDLE                           = -1000013,/**< */
	ERR_FIO_IMAGE_BITS_ARE_BIGGER_THAN_16          = -1000014,/**< */
	ERR_FIO_TARGET_BITWIDTH_IS_DIFFERENT           = -1000015,/**< */
	ERR_FIO_TARGET_COMPONENT_COUNT_IS_DIFFERENT    = -1000016,/**< */
	ERR_FIO_CANNOT_LOAD_RGB24_ON_RGB48_LINK        = -1000017,/**< */
	ERR_FIO_INVALID_FILENAME                       = -1000018/**< function is called by using an invalid filename */
};
