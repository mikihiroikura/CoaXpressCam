#ifndef KYFG_LIB_DEFINES_H_
#define KYFG_LIB_DEFINES_H_

#include "ky_cxp_defines.h"
#include "ky_lib_defines.h"

#ifdef __linux__
    #include <stddef.h>
#endif

#ifdef _MSC_VER
    #ifdef KYFG_EXPORTS
        #define KAYA_API __declspec(dllexport)
    #else
        #define KAYA_API __declspec(dllimport)
    #endif
#else
    #define KAYA_API
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t FGHANDLE;
#define PRIFGHANDLE PRIX32
static const FGHANDLE INVALID_FGHANDLE = ((FGHANDLE)-1);
static const FGHANDLE NULL_FGHANDLE = ((FGHANDLE)0);

typedef uint32_t CAMHANDLE;
#define PRICAMHANDLE PRIX32

// read http://stackoverflow.com/questions/3025050/error-initializer-element-is-not-constant-when-trying-to-initialize-variable-w
// for a nice explanation about difference in *constant* definitions in C and C++ languages
// and why in case of C 'old good' #define is prefered over 'static const' (which is definetely prefered in case of C++)
#ifdef __cplusplus
static const CAMHANDLE INVALID_CAMHANDLE = ((CAMHANDLE)-1);
#else
#define INVALID_CAMHANDLE ((CAMHANDLE)-1)
#endif


typedef uint32_t STREAM_HANDLE;
#define PRISTREAM_HANDLE PRIX32
#ifdef __cplusplus
static const STREAM_HANDLE INVALID_BUFFHANDLE = ((STREAM_HANDLE)-1);   // deprecated, use INVALID_STREAMHANDLE
static const STREAM_HANDLE INVALID_STREAMHANDLE = ((STREAM_HANDLE)-1);
#else
#define INVALID_BUFFHANDLE ((STREAM_HANDLE)-1)  // deprecated, use INVALID_STREAMHANDLE
#define INVALID_STREAMHANDLE ((STREAM_HANDLE)-1)
#endif
typedef STREAM_HANDLE BUFFHANDLE;// for backward compatibility, now BUFFHANDLE is deprecated

typedef uint64_t STREAM_BUFFER_HANDLE;
#define PRISTREAM_BUFFER_HANDLE PRIX64
#ifdef __cplusplus
static const STREAM_BUFFER_HANDLE INVALID_STREAM_BUFFER_HANDLE = ((STREAM_BUFFER_HANDLE)-1);
#else
#define INVALID_STREAM_BUFFER_HANDLE ((STREAM_BUFFER_HANDLE)-1)
#endif

typedef uint64_t KYHANDLE;
#define PRIKY_HANDLE PRIX64
#ifdef __cplusplus
static const KYHANDLE INVALID_KY_HANDLE = (KYHANDLE(-1));
#else
#define INVALID_KY_HANDLE ((KYHANDLE)-1)
#endif

// Information about connected cameras

#ifdef __cplusplus
static const int KY_MAX_CAMERAS = 16;
static const int KY_MAX_CAMERA_INFO_STRING_SIZE = 64;
#else
#define KY_MAX_CAMERAS 16
#define KY_MAX_CAMERA_INFO_STRING_SIZE	64
#endif


typedef struct _camera_info
{
    unsigned char       master_link;
    unsigned char       link_mask;
    CXP_LINK_SPEED      link_speed;
    uint32_t            stream_id;
    char                deviceVersion[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    char                deviceVendorName[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    char                deviceManufacturerInfo[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    char                deviceModelName[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    char                deviceID[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    char                deviceUserID[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    KYBOOL              outputCamera;
    KYBOOL              virtualCamera; //This parameter can be KYTRUE only in case of custom firmware implementations.
} KYFGCAMERA_INFO;

#pragma pack(push, 1)
typedef struct _camera_info2
{
    uint32_t        version;  // Version of this structure definition, must be set to 0 by caller.

    uint8_t             master_link;
    uint8_t             link_mask;
    CXP_LINK_SPEED      link_speed;
    uint32_t            stream_id;
    char                deviceVersion[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    char                deviceVendorName[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    char                deviceManufacturerInfo[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    char                deviceModelName[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    char                deviceID[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
    char                deviceUserID[KY_MAX_CAMERA_INFO_STRING_SIZE + 1];
	KYBOOL              outputCamera;
	KYBOOL              virtualCamera; //This parameter can be KYTRUE only in case of custom firmware implementations.
}KYFGCAMERA_INFO2;
#pragma pack(pop)

// Error and status reporting
typedef enum _fgstatus
{
    CSSTATUS_OK                                 = 0x2000,
    CSSTATUS_UNKNOWN_SIM_HANDLE                 = 0x2001,
    CSSTATUS_HW_NOT_FOUND                       = 0x2002,
    CSSTATUS_BUSY                               = 0x2003,
    CSSTATUS_FILE_NOT_FOUND                     = 0x2004,
    CSSTATUS_FILE_READ_ERROR                    = 0x2005,
    CSSTATUS_CONFIG_NOT_LOADED                  = 0x2006,
    CSSTATUS_INVALID_VALUE                      = 0x2007,
    CSSTATUS_MAX_CONNECTIONS                    = 0x2008,
    CSSTATUS_COULD_NOT_STOP                     = 0x2009,
    CSSTATUS_CANNOT_LOAD_IMAGE_FILE             = 0x200A,
    CSSTATUS_MEMORY_ERROR                       = 0x200B,
    CSSTATUS_UNKNOWN_SIM_CONTROL                = 0x200C,
    CSSTATUS_WRONG_PARAMETER_NAME               = 0x200D,
    CSSTATUS_WRONG_PARAMETER_TYPE               = 0x200E,
    CSSTATUS_GENICAM_EXCEPTION                  = 0x200F,
    CSSTATUS_OUT_OF_RANGE_ADDRESS               = 0x2010,
    CSSTATUS_PATH_INVALID                       = 0x2011,
    CSSTATUS_FILE_TYPE_INVALID                  = 0x2012,
    CSSTATUS_UNSUPPORTED_IMAGE                  = 0x2013,
    CSSTATUS_UNSUPPORTED_IMAGE_CONVERSION       = 0x2014,
    CSSTATUS_UNSUPPORTED_DEPTH_CONVERSION       = 0x2015,
    CSSTATUS_INVALID_VALUES_FILE                = 0x2016,
    CSSTATUS_FILE_WRITE_ERROR                   = 0x2017,
    CSSTATUS_BUFFER_NOT_LOADED                  = 0x2018,
    CSSTATUS_TRIGGER_NOT_SET                    = 0x2019,
    CSSTATUS_CANNOT_SET_USER_REGISTER_ADDRESS   = 0x201A,
    CSSTATUS_CANNOT_READ_USER_REGISTER          = 0x201B,
    CSSTATUS_CANNOT_WRITE_USER_REGISTER         = 0x201C,
    CSSTATUS_CANNOT_WRITE_REGISTER              = 0x201D,
    CSSTATUS_IMAGE_HEADER_INJECTION_SIZE_TOO_BIG = 0x201E,
    CSSTATUS_NO_EXTENDED_HW_FEATURES            = 0x201F,
    CSSTATUS_MAX_USER_ADDRESS_EXCEEDED          = 0x2020,


    FGSTATUS_OK                                 = 0x3000,
    FGSTATUS_UNKNOWN_HANDLE                     = 0x3001,
    FGSTATUS_HW_NOT_FOUND                       = 0x3002,
    FGSTATUS_BUSY                               = 0x3003,
    FGSTATUS_FILE_NOT_FOUND                     = 0x3004,
    FGSTATUS_FILE_READ_ERROR                    = 0x3005,
    FGSTATUS_CONFIG_NOT_LOADED                  = 0x3006,
    FGSTATUS_INVALID_VALUE                      = 0x3007,
    FGSTATUS_MAX_CONNECTIONS                    = 0x3008,
    FGSTATUS_MEMORY_ERROR                       = 0x3009,
    FGSTATUS_WRONG_PARAMETER_NAME               = 0x300A,
    FGSTATUS_WRONG_PARAMETER_TYPE               = 0x300B,
    FGSTATUS_GENICAM_EXCEPTION                  = 0x300C,
    FGSTATUS_OUT_OF_RANGE_ADDRESS               = 0x300D,
    FGSTATUS_COULD_NOT_START                    = 0x300E,
    FGSTATUS_COULD_NOT_STOP                     = 0x300F,
    FGSTATUS_XML_FILE_NOT_LOADED                = 0x3010,
    FGSTATUS_INVALID_VALUES_FILE                = 0x3011,
    FGSTATUS_NO_REQUIRED_PARAMETERS_SECTION     = 0x3012,
    FGSTATUS_WRONG_PARAMETERS_SECTION           = 0x3013,
    FGSTATUS_VALUE_HAS_NO_SELECTOR              = 0x3014,
    FGSTATUS_CALLBACK_NOT_ASSIGNED              = 0x3015,
    FGSTATUS_HANDLE_DOES_NOT_MATCH_CONFIG       = 0x3016,
    FGSTATUS_BUFFER_TOO_SMALL                   = 0x3017,
    FGSTATUS_BUFFER_UNSUPPORTED_SIZE            = 0x3018,
    FGSTATUS_GRABBER_FIRMWARE_NOT_SUPPORTED     = 0x3019,
    FGSTATUS_PARAMETER_NOT_WRITABLE             = 0x301A,
    FGSTATUS_CANNOT_START_HW_STREAM             = 0x301B,
    FGSTATUS_WRONG_SCHEMA_VERSION               = 0x301C,
    FGSTATUS_CAMERA_OR_GRABBER_SECTION_NOT_ARRAY= 0x301D,
    FGSTATUS_ROOT_IS_NOT_OBJECT                 = 0x301E,
    FGSTATUS_NO_PARAMETER_TYPE                  = 0x301F,
    FGSTATUS_FILE_CREATE_ERROR                  = 0x3020,
    FGSTATUS_COULD_NOT_STOP_STREAM              = 0x3021,
    FGSTATUS_BUFFER_MEMORY_OVERLAP              = 0x3022,
    FGSTATUS_UNSUPPORTED_PARAMETER_TYPE         = 0x3023,
    FGSTATUS_OPERATION_TIMEOUT                  = 0x3024,
    FGSTATUS_OPERATION_BLOCKED                  = 0x3025,
    FGSTATUS_PARAMETER_NOT_READABLE             = 0x3026,

    FGSTATUS_EXCEEDED_MAX_CAMERA_CONNECTIONS    = 0x3100,

    FGSTATUS_QUEUED_BUFFERS_NOT_SUPPORTED       = 0x3101,
    FGSTATUS_DESTINATION_QUEUE_NOT_SUPPORTED    = 0x3102,
    FGSTATUS_INVALID_STREAM_INFO_CMD            = 0x3103,
    FGSTATUS_INVALID_STREAM_BUFFER_INFO_CMD     = 0x3104,
    FGSTATUS_STREAM_NOT_CREATED                 = 0x3105,

    FGSTATUS_GRABBER_NOT_CONNECTED              = 0x3106,
    FGSTATUS_CAMERA_NOT_CONNECTED               = 0x3107,

    FGSTATUS_GRABBER_NOT_OPENED                 = 0x3108,
    FGSTATUS_CAMERA_NOT_OPENED                  = 0x3109,

    FGSTATUS_BUFFER_ALREADY_IN_INPUT_QUEUE      = 0x310A,
    FGSTATUS_STREAM_CANNOT_LOCK                 = 0x310B,  // a Stream is started and cannot be re-started
    FGSTATUS_STREAM_IS_LOCKED                   = 0x310C,  // a Stream is being used and cannot be deleted 

    FGSTATUS_CAMERA_NODES_NOT_INITIALIZED       = 0x3200,

    FGSTATUS_UPDATE_WRONG_VID                   = 0x3300,
    FGSTATUS_UPDATE_WRONG_BOARD_ID              = 0x3301,

    FGSTATUS_CANNOT_WRITE_IMAGE                 = 0x3400,

    FGSTATUS_FACILITY_DISABLED                  = 0x3FFD,
    FGSTATUS_FEATURE_NOT_IMPLEMENTED            = 0x3FFE,
    FGSTATUS_UNKNOWN_ERROR                      = 0x3FFF,

}FGSTATUS;
#define PRIFGSTATUS PRIX32

// Simulator control
typedef enum _sim_control
{
    SIM_STOP,
    SIM_START, // Free running at specified frame rate
    SIM_PAUSE
}SIM_CONTROL;

// IO Structure 
typedef enum _io_source
{
    KY_LOGIC_0      = 0x00,
    KY_OPTO_0       = 0x01,
    KY_OPTO_1       = 0x02,
    KY_OPTO_2       = 0x03,
    KY_OPTO_3       = 0x04,

    KY_LVDS_0       = 0x05,
    KY_LVDS_1       = 0x06,
    KY_LVDS_2       = 0x07,
    KY_LVDS_3       = 0x08,

    KY_TTL_0        = 0x09,
    KY_TTL_1        = 0x0A,
    KY_TTL_2        = 0x0B,
    KY_TTL_3        = 0x0C,
    KY_TTL_4        = 0x0D,
    KY_TTL_5        = 0x0E,
    KY_TTL_6        = 0x0F,
    KY_TTL_7        = 0x10,

    KY_LVTTL_0      = 0x11,
    KY_LVTTL_1      = 0x12,
    KY_LVTTL_2      = 0x13,
    KY_LVTTL_3      = 0x14,

    KY_CXPTRIG_0    = 0x15,

    KY_CXP0_IO_0    = 0x20,
    KY_CXP0_IO_1    = 0x21,
    KY_CXP0_IO_2    = 0x22,
    KY_CXP0_IO_3    = 0x23,

	KY_LOGIC_1      = 0x3F
}KY_IO;

// Type of video source used by Chameleon simulator
typedef enum _VideoSourceType
{
    VIDEO_SOURCE_NONE    = -1,
    VIDEO_SOURCE_PATTERN = 0,
    VIDEO_SOURCE_FILE    = 1,
    VIDEO_SOURCE_FOLDER  = 2
} VIDEO_SOURCE_TYPE;

// Ramp patern type
typedef enum _pattern_type
{
    PATTERN_XRAMP       = 0,
    PATTERN_XRAMP_COLOR = 1,
    PATTERN_YRAMP       = 2,
    PATTERN_YRAMP_COLOR = 3,
    PATTERN_XYRAMP      = 4,
    PATTERN_XYRAMP_COLOR= 5,
    PATTERN_FIXED       = 6
}PATTERN_TYPE;

// Valid flags for 'concurrency_mode' of KYFGLib_InitParameters
typedef enum _KYFGLIB_CONCURRENCY_FLAGS
{
    KYFGLIB_CONCURRENCY_SA_RESTART = 1, // Only valid in Linux, interrupts compatible with blocking calls in the same thread
}KYFGLIB_CONCURRENCY_FLAGS;

#pragma pack(push, 1)
typedef struct _KYFGLib_InitParameters
{
    uint32_t version; // Version of this structure definition, must be 1 or 2

    // since version 1:
    uint32_t  concurrency_mode;  // combination of KYFGLIB_CONCURRENCY_FLAGS, all unused bits must be set to 0
    uint32_t  logging_mode;      // reserved, must be set to 0

    // since version 2:
    KYBOOL noVideoStreamProcess; // Use library without requesting video stream facilities, e.g for camera control only
}KYFGLib_InitParameters;
#pragma pack(pop)

#ifdef __cplusplus
static const uint64_t KYDEVICE_ERROR_FWUPDATE_REQUIRED = 0x0001;
static const uint64_t KYDEVICE_ERROR_SODIMM_REQUIRED = 0x0002;
#else
#define KYDEVICE_ERROR_FWUPDATE_REQUIRED  0x0001
#define KYDEVICE_ERROR_SODIMM_REQUIRED 0x0002
#endif

#pragma pack(push, 1)
typedef struct _KYDEVICE_STATUS
{
uint32_t version;  // Version of this structure definition, must be set > 0 to get members filled.

    // Since version 1:
    uint64_t error_mask;
}KYDEVICE_STATUS;
#pragma pack(pop)

#ifdef __cplusplus
} // extern "C" {
#endif

#endif  // #ifndef KYFG_LIB_DEFINES_H_
