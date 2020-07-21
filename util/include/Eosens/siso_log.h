/**
* siso_log.h
*
* Copyright (c) 2017 Silicon Software GmbH, All Rights Reserved.
*
* \file siso_log.h
*
* \brief Function declarations for the user logging interface
*
* \author Silicon Software GmbH
*
*/

#ifndef __SISO_LOG_H__
#define __SISO_LOG_H__

#if defined(_WIN32)
#include <windows.h>
/** typedef for process ids */
typedef DWORD tProcessId;
/** typedef for thread ids */
typedef DWORD tThreadId;
#else
#include <unistd.h>
#include <pthread.h>
/** typedef for process ids */
typedef pid_t tProcessId;
/** typedef for thread ids */
typedef pthread_t tThreadId;
#endif

/** logging mode: OFF */
#define SISOLOG_MODE_OFF     0
/** logging mode: DEFAULT */
#define SISOLOG_MODE_DEFAULT 1

/** logging level: WARN */
#define SISOLOG_LOGLEVEL_WARN  4
/** logging level: ERROR */
#define SISOLOG_LOGLEVEL_ERROR 5
/** logging level: FATAL */
#define SISOLOG_LOGLEVEL_FATAL 6

/** logging return value: SUCCESS */
#define SISOLOG_OK                        0
/** logging return value: ERROR - Internal error */
#define SISOLOG_ERROR_INTERNAL_ERROR     -1
/** logging return value: ERROR - Library not loaded */
#define SISOLOG_ERROR_LIB_NOT_LOADED     -2
/** logging return value: ERROR - Library already loaded */
#define SISOLOG_ERROR_LIB_ALREADY_LOADED -3
/** logging return value: ERROR - Unresolved symbol */
#define SISOLOG_ERROR_UNRESOLVED_SYMBOL  -4

#pragma pack(push)
#pragma pack(1)

/**
\brief structure for logging tags
*/
typedef struct tSisoLogTag {
    /** logging tag: name */
    const char* const name;
    /** logging tag: value */
    const char* const value;
} tSisoLog_Tag;

#pragma pack(pop)

/* callback function definitions */

/**
    \brief Callback function definition for log events
    \param pid process id of the log event source process
    \param tid thread id of the log event source thread
    \param logger name of the log event source logger
    \param level logging level of the log event
    \param msg logging message of the log event
    \param tagcount number of attached logging tags of the log event
    \param tags logging tags array
    \param tags logging tags array
    \param userdata user data pointer from the matching call to SisoLog_RegisterLogMsgCallback
*/
typedef void FuncType_LogMsgCallback(tProcessId pid, tThreadId tid, const char* const logger, unsigned int level, const char* const msg, unsigned int tagcount, const tSisoLogTag* const tags, void* userdata);

#ifdef __cplusplus
extern "C" {
#endif

/**
    \brief The function SisoLog_InitLibrary() initializes internal structures of the library.

    This function initializes internal structures in the library and keeps them in memory until
    #SisoLog_FreeLibrary() is called.

    \returns 0 on success, or a non-zero error value otherwise
*/
int SisoLog_InitLibrary();

/**
    \brief The function SisoLog_FreeLibrary() releases internal structures of the library.

    This function releases all internal structures that were initialized by a successful call
    #Fg_InitLibraries().

    \returns 0 on success, or a non-zero error value otherwise
*/
int SisoLog_FreeLibrary();

/**
    \brief The function SisoLog_SetMode() set the mode for the library.

    Allowed values are: SISOLOG_MODE_OFF and SISOLOG_MODE_DEFAULT.

    \param mode the mode value to be set
    \returns 0 on success, or a non-zero error value otherwise
*/
int SisoLog_SetMode(unsigned int mode);

/**
    \brief The function SisoLog_GetMode() returns the current mode for the library.

    \param mode pointer to the mode variable where the current mode will be stored to
    \returns 0 on success, or a non-zero error value otherwise
*/
int SisoLog_GetMode(unsigned int* mode);

/**
    \brief Register the user callback for log events

    \param callback pointer to the user callback function
    \param userdata a use specific pointer, which will be returned on each callback
    \returns 0 on success, or a non-zero error value otherwise
*/
int SisoLog_RegisterLogMsgCallback(FuncType_LogMsgCallback* callback, void* userdata);

#ifdef __cplusplus
}
#endif

#endif // __SISO_LOG_H__
