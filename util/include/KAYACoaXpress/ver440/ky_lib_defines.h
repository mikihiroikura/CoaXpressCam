#ifndef KY_LIB_DEFINES_H_
#define KY_LIB_DEFINES_H_

#ifndef _WIN32
    #define __STDC_LIMIT_MACROS
#endif
#include <stdint.h>
#include <limits.h>
#include <float.h>
//#ifndef UINT32_MAX // TODO: Check the need for this in CentOS - issue 1446
//#   define UINT32_MAX  0x00000000ffffffffULL   /* maximum unsigned int32 value */
//#endif
#ifndef INT64_MAX
#   define INT64_MAX   0x7fffffffffffffffLL    /* maximum signed int64 value */
#endif
#ifndef INT64_MIN
#   define INT64_MIN   0x8000000000000000LL    /* minimum signed int64 value */
#endif

// KY_EXPORTS definition
#ifdef _MSC_VER
    #ifdef KY_EXPORTS
        #define KY_API __declspec(dllexport)
    #else
        #define KY_API __declspec(dllimport)
    #endif
#else
    #define KY_API
#endif

// KYFG_CALLCONV definition
#ifdef __GNUC__
#define KYFG_CALLCONV
#ifndef KY_INLINE
    #define KY_FORCE_INLINE_GCC __attribute__((always_inline))
    #define KY_FORCE_INLINE_MSVC
    #define KY_INLINE __inline
#endif //KY_INLINE
#elif defined(_MSC_VER)
#ifndef KY_INLINE
    #define KY_FORCE_INLINE_GCC
    #define KY_FORCE_INLINE_MSVC __forceinline
    #define KY_INLINE
#endif //KY_INLINE
// TODO: specify KYFG_CALLCONV for ALL external API
#define KYFG_CALLCONV __cdecl
#else
#define KYFG_CALLCONV
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char KYBOOL;
#define KYTRUE   1
#define KYFALSE  0

// Parameter camera type
typedef enum _cam_property_type
{
    PROPERTY_TYPE_UNKNOWN	= -1,
    PROPERTY_TYPE_INT		= 0x00,
    PROPERTY_TYPE_BOOL		= 0x01,
	PROPERTY_TYPE_STRING	= 0x02,
	PROPERTY_TYPE_FLOAT		= 0x03,
	PROPERTY_TYPE_ENUM		= 0x04,
    PROPERTY_TYPE_COMMAND	= 0x05,
	PROPERTY_TYPE_REGISTER	= 0x06
}KY_CAM_PROPERTY_TYPE;


typedef enum _KY_DATA_TYPE
{
    KY_DATATYPE_UNKNOWN     = 0,       /* Unknown data type */
    KY_DATATYPE_STRING      = 1,       /* NULL-terminated C string (ASCII encoded). */
    KY_DATATYPE_STRINGLIST  = 2,       /* Concatenated INFO_DATATYPE_STRING list. End of list is signaled with an additional NULL. */
    KY_DATATYPE_INT16       = 3,       /* Signed 16 bit integer. */
    KY_DATATYPE_UINT16      = 4,       /* Unsigned 16 bit integer */
    KY_DATATYPE_INT32       = 5,       /* Signed 32 bit integer */
    KY_DATATYPE_UINT32      = 6,       /* Unsigned 32 bit integer */
    KY_DATATYPE_INT64       = 7,       /* Signed 64 bit integer */
    KY_DATATYPE_UINT64      = 8,       /* Unsigned 64 bit integer */
    KY_DATATYPE_FLOAT64     = 9,       /* Signed 64 bit floating point number. */
    KY_DATATYPE_PTR         = 10,      /* Pointer type (void*). Size is platform dependent (32 bit on 32 bit platforms). */
    KY_DATATYPE_BOOL8       = 11,      /* Boolean value occupying 8 bit. 0 for false and anything for true. */
    KY_DATATYPE_SIZET       = 12,      /* Platform dependent unsigned integer (32 bit on 32 bit platforms). */
    KY_DATATYPE_BUFFER      = 13,      /* Like a INFO_DATATYPE_STRING but with arbitrary data and no NULL termination. */
}KY_DATA_TYPE;

// DEPRECATED definition
#ifdef __GNUC__
#define KY_DEPRECATED(func, text) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define KY_DEPRECATED(func, text) __declspec(deprecated(text)) func
// TODO: specify KYFG_CALLCONV for ALL external API
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

#ifndef _WIN32
    #include <inttypes.h>
/**/
    #define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

    #if(GCC_VERSION <= 40407)
        #define nullptr 0
    #endif
	// We do NOT want to #define 'nullptr' as 0 when compiling in Visual Studio 2012, which does support keyword 'nullptr' 
	// (as well as other C++ 11 features), but still #defines __cplusplus as 199711L
	#if __cplusplus < 201103L
		#define nullptr 0
	#endif

#endif

#ifdef __GNUC__
#define VARIABLE_IS_NOT_USED __attribute__ ((unused))
#else
#define VARIABLE_IS_NOT_USED
#endif

KY_DEPRECATED(KY_API const char* KY_DeviceDisplayName(int index), "Function 'KY_DeviceDisplayName' is deprecated, please use function 'KY_DeviceInfo' to retrieve device name");

typedef struct _KY_DEVICE_INFO
{
    char     szDeviceDisplayName[256];
    int      nBus;
    int      nSlot;
    int      nFunction;
    uint32_t DevicePID;
    KYBOOL   isVirtual;
}KY_DEVICE_INFO;

static const char* VARIABLE_IS_NOT_USED DEVICE_NEWINTERRUPTSOURCE_SUPPORTED = "FW_Dma_Capable_NewInterruptSource_Imp";
static const char* VARIABLE_IS_NOT_USED DEVICE_QUEUED_BUFFERS_SUPPORTED = "FW_Dma_Capable_QueuedBuffers_Imp";



#ifdef __cplusplus
} // extern "C" {
#endif

#endif // #ifndef KY_LIB_DEFINES_H_
