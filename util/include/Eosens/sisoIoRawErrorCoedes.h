/**
 * sisoIoRawErrorCoedes.h
 *
 * Copyright (c) 2002-2017 Silicon Software GmbH, All Rights Reserved.
 *
 * \file sisoIoRawErrorCoedes.h
 *
 * \brief Header file for all Raw Error codes.
 *
 * \author Silicon Software GmbH
 */


enum RAW_ERRORS
{
	ERR_RAW_NO_ERROR     = 0,/**< */
	ERR_RAW_UNDEFINED    = -100000001,/**< */
	ERR_RAW_FORMAT       = -100000002,/**< */  // ungültiges DateiFormat
	ERR_RAW_NULL_DATA    = -100000003/**< */
};
