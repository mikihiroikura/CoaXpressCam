/**
* siso_genicam.h
*
* Copyright (c) 2002-2017 Silicon Software GmbH, All Rights Reserved.
*
* \file   siso_genicam.h
*
* \brief Function declarations for control of GenICam based cameras connected to
*        a Silicon Software frame grabber
*
* \author Silicon Software GmbH
*
*/

#ifndef __SISO_GENICAM_H__
#define __SISO_GENICAM_H__

#include "fgrab_struct.h"
#include "siso_genicam_error.h"
#include <sys/types.h>

/** handle structure for boards */
typedef struct SgcBoardHandle SgcBoardHandle;
/** handle structure for cameras */
typedef struct SgcCameraHandle SgcCameraHandle;

#pragma pack(push)
#pragma pack(1)

/**
\brief structure for camera information
*/
typedef struct SgcCameraInfo
{
  char* deviceVendorName;      /**< vendor name */
  char* deviceModelName;       /**< model name */
  char* deviceFamilyName;      /**< family name */
  char* deviceVersion;         /**< device version */
  char* deviceFirmwareVersion; /**< device firmware version */
  char* deviceManufacturerInfo;/**< manufacturer info */
  char* deviceSerialNumber;    /**< serial number */
  char* deviceUserId;          /**< user id */
} SgcCameraInfo;

#pragma pack(pop)

/**
\brief Event types handled by Sgc_RegisterEventCallback
*/
#define SGC_EVENT_CAMERA_CONNECTION_LOST          100 /**< event type "Camera Connection Lost": the connection to the connected camera has been lost */
#define SGC_EVENT_LINK_CONNECTION_STATE_CHANGED   200 /**< event type "link connection state changed*/
#define SGC_EVENT_NUMBER_OF_LINKS_CHANGED         300 /**< event type */
/**
\brief additional data for event SGC_EVENT_LINK_CONNECTION_STATE_CHANGED
*/
#define SGC_ELCS_CONNECTED    1 /**< event type "link state is changed to connected*/
#define SGC_ELCS_DISCONNECTED 2 /**< event type "link state is changed to disconnected*/

/**
\brief physical link states for frame grabber ports
*/
#define SGC_PLS_UNDEFINED        0 /**< frame grabber connected to camera is not initialized*/
#define SGC_PLS_CONNECTED        1 /**< frame grabber is connected to camera on physical layer*/
#define SGC_PLS_DISCONNECTED     2 /**< frame grabber is disconnected to camera on physical layer*/
#define SGC_PLS_CONNECTING       3 /**< frame grabber is connecting to camera on physical layer*/

/**
\brief flags for Sgc_ScanPortsEx() function
*/
#define SGC_SP_FLAG_KEEP_PHYCONNECTION         0x0001 /**< Skip interaction on physical layer and assume physical layer is setup*/
#define SGC_SP_FLAG_SKIP_AUTO_PORTADJUSTMENT   0x0002 /**< do not adjust adjust port ordering and keep 1:1 connection*/
#define SGC_SP_FLAG_SKIP_DEVICE_CONFIGURATION  0x0004 /**< skip adjustment of camera device configuration and keep current camera state*/
#define SGC_SP_FLAG_SKIP_MULTIFRAMEGRABBERSYNC 0x0008 /**< skips waiting for master frame grabber on a slave frame grabber system */
#define SGC_SP_FLAG_USE_CAMERA_DEFAULT_SPEED   0x0010 /**< try to connect to camera at the default speed of the camera */

/**
\brief camera states for connection profiles
*/
#define SGC_CP_CAMERA_STATE_INVALID             0 /**< camera handle invalid */
#define SGC_CP_CAMERA_STATE_NOT_CONNECTED       1 /**< camera is not connected */
#define SGC_CP_CAMERA_STATE_LINK_ONLY_CONNECTED 2 /**< camera is only low level connected (no GenICam connection available) */
#define SGC_CP_CAMERA_STATE_FULL_CONNECTED      3 /**< camera is full connected (GenICam connection available) */

#pragma pack(push)
#pragma pack(1)

/**
\brief    data structure for events received from the board callback functions
\remarks  SgcEventInfo is a structure, that gets filled by the callback interface
          of siso_genicam. Which event is notified can be determined by evaluating
          the member eventID (event type identifier). the event types correspond to
          the SGC_EVENT_CXXX constants (defines).
          Specific events types correspond to specific additional information.
          These information is represented as different components of the union.
          When evaluating the additional information, the member eventID needs to be
          determined first.
*/
typedef struct SgcEventInfo
{
  uint16_t eventID; /**< event type identifier */
  /** event data */
  union {
    /** SGC_EVENT_CAMERA_CONNECTION_LOST */
    struct {
      uint32_t cameraIndex; /**< the camera index where the camera disconnection occurred */
    } camera_connection_lost;
    /** SGC_EVENT_LINK_CONNECTION_ESTABLISHED */
    /** SGC_EVENT_LINK_CONNECTION_LOST */
    struct {
      uint32_t phyIndex; /**< physical port, where the state change was detected/performed*/
      uint32_t linkConnectionState ; /**< new state of connection, see SGC_ELCS_XXX*/
    } link_connection_state_changed;
  } data;
} SgcEventInfo;

/**
\brief    data structure representing the state of a loaded connection profile
\remarks  SgcConnectionProfile, that gets filled by the the function Sgc_LoadConnectionProfile
          of siso_genicam. It stores the board handle and camera related data
          (camera count, camera handles, camera infos, camera connection states).
*/
typedef struct SgcConnectionProfile {
    SgcBoardHandle* boardHandle;     /**< board handle */
    int cameraCount;                 /**< number of cameras in this profile */
    SgcCameraHandle** cameraHandles; /**< array of camera handles [0..cameraCount] */
    SgcCameraInfo** cameraInfos;     /**< array of camera infos [0..cameraCount] */
    int* cameraConnectionStates;     /**< array of camera connection states [0..cameraCount], see SGC_CP_CAMERA_STATE_XXX */
} SgcConnectionProfile;

#pragma pack(pop)

/**
\brief Flags
*/
#define SGC_FLAGS_NONE        0x0 /**< flag: none */
#define SGC_FLAGS_STOPONERROR 0x1 /**< flag: stop on error */

/**
\brief possible data types for Sgc_getBoardPropertyWithType and Sgc_getCameraPropertyWithType
*/
#define SGC_PROPERTY_TYPE_UINT   0x1  /**< property type: UNSIGNED INT */
#define SGC_PROPERTY_TYPE_DOUBLE 0x2  /**< property type: DOUBLE */

/**
\brief prototype of event callback procedure

reserved for later usage

\param boardHandle    board handle, at which the event occurred
\param cameraHandle   camera handle, at which the event occurred
\param eventInfo      event details/data
\param userData       additional payload passed from user at registration of event handler
*/
typedef int (*SgcEventCallback_t)(SgcBoardHandle* boardHandle, SgcCameraHandle* cameraHandle , const SgcEventInfo * const eventInfo, const void* userData);

/**
\brief predefined port masks
*/
#define PORTMASK0 1 /**< Bit of port mask for frame grabber port 0*/
#define PORTMASK1 2 /**< Bit of port mask for frame grabber port 1*/
#define PORTMASK2 4 /**< Bit of port mask for frame grabber port 2*/
#define PORTMASK3 8 /**< Bit of port mask for frame grabber port 3*/

/**
\brief predefined link speeds
*/
#define LINK_SPEED_NONE      0 /**< define for default / unused link speed */
#define LINK_SPEED_1250   1250 /**< define for link speed 1.25 Gbps*/
#define LINK_SPEED_2500   2500 /**< define for link speed 2.5 Gbps */
#define LINK_SPEED_3125   3125 /**< define for link speed 3.125 Gbps*/
#define LINK_SPEED_5000   5000 /**< define for link speed 5 Gbps */
#define LINK_SPEED_6250   6250 /**< define for link speed 6.25 Gbps*/
#define LINK_SPEED_10000 10000 /**< define for link speed 10 Gbps*/

/**
\brief predefined link speeds according to CXP specs.
*/
#define CXP_SPEED_NONE LINK_SPEED_NONE /**< define for default / unused link speed */
#define CXP_SPEED_1250 LINK_SPEED_1250 /**< define for link speed 1.25 Gbps*/
#define CXP_SPEED_2500 LINK_SPEED_2500 /**< define for link speed 2.5 Gbps */
#define CXP_SPEED_3125 LINK_SPEED_3125 /**< define for link speed 3.125 Gbps*/
#define CXP_SPEED_5000 LINK_SPEED_5000 /**< define for link speed 5 Gbps */
#define CXP_SPEED_6250 LINK_SPEED_6250 /**< define for link speed 6.25 Gbps*/

/**
\brief  ErrorCodes
*/

/**
\brief connection properties
*/
#define PROP_SET_LINK_CONFIG_FREQ_S        "SET_LINK_CONFIG_FREQ"    /**< Link frequency, defined by IDs CXP_SPEED_XXX*/
#define PROP_SET_STREAM_PAKET_SIZE_S       "SET_STREAM_PAKET_SIZE"   /**< Stream packet size */

/**
\brief board properties
*/
#define BOARD_PROP_CXP_POWERSENSE                   "CxpPowerSense"                 /**< Return the value of the current in mA at the PowerOverCoaxPress Subsystem for a given frame grabber port.*/
#define BOARD_PROP_CXP_POWERVIN                     "CxpPowerVin"                   /**< Returns the value of voltage output in V at the PowerOverCoaxPress Subsystem for a given frame grabber port*/
#define BOARD_PROP_CXP_POWERADIN                    "CxpPowerAdIn"                  /**< Returns the value of temperature in Celsius at frame grabber */
#define BOARD_PROP_CXP_POWERSTATUS                  "CxpPowerStatus"                /**< Returns the state of the PowerOverCoaxPress subsystem for a given frame grabber port*/
#define BOARD_PROP_CXP_TRANSCEIVER_NOT_IN_TABLE     "CxpTransceiverNotInTable"      /**< Error indicator at physical layer*/
#define BOARD_PROP_CXP_TRANSCEIVER_DISPARITY_ERROR  "CxpTransceiverDisparityError"  /**< Error indicator at physical layer*/
#define BOARD_PROP_CLHS_FEC_ENABLE                  "CLHSFecEnable"                 /**< Enable/Disable Forward error correction */
#define BOARD_PROP_CLHS_SEND_REVISION_MESSAGE       "CLHSSendRevisionMessage"       /**< Force Sending of a revision message, AddValue = 1, forces a LinkReset at the message*/
#define BOARD_PROP_CLHS_READ_MDIO                   "CLHSReadMDIO"                  /**< internal interface for read access to physical layer*/
#define BOARD_PROP_CLHS_WRITE_MDIO                  "CLHSWriteMDIO"                 /**< internal interface for write access to physical layer*/
#define BOARD_PROP_CLHS_LINK_STATE                  "CLHSLinkState"                 /**< state of the physical link: disconnected, connected, connecting*/
#define BOARD_PROP_CLHS_LINK_DETECTION_ENABLE       "CLHSLinkDetectionEnable"       /**< enable or disable link detection at a specific frame grabber port. Value: 0, disable, 1 enable; AddValue: physical port*/
#define BOARD_PROP_NR_OF_CAMERAS                    "NrOfCameras"                   /**< Returns the number of detected cameras.*/
#define BOARD_PROP_TRANSCEIVERS_LOCKED_MASK         "TransceiversLockedMask"        /**< Returns the mask of transceivers locked (Bit n: channel n transceiver is locked).*/

/**
\brief camera properties
*/
#define CAM_PROP_STREAMPACKETSIZE_S    "STREAMPACKETSIZE"           /**< max. size of stream packages [in/out]*/
#define CAM_PROP_STREAMPACKET_SIZE_S   CAM_PROP_STREAMPACKETSIZE_S  /**< deprecated; use CAM_PROP_STREAMPACKETSIZE_S */
#define CAM_PROP_CONTROLPACKETSIZE_S   "CONTROLPACKETSIZE"          /**< max. size of CxP Control packages for that camera [out]*/
#define CAM_PROP_LINKSPEED_S           "LINKSPEED"                  /**< CXP-speed for a certain link as enum, values see CXP_SPEED_XXX [in/out]*/
#define CAM_PROP_DISCOVERYSPEED_S      "DISCOVERYSPEED"             /**< CXP-speed for a certain link as enum, values see CXP_SPEED_XXX [in/out]*/
#define CAM_PROP_ISCONNECTED_S         "CONNECTED"                  /**< State of CXP - Connection [out]*/
#define CAM_PROP_NROFLINKS_S           "NROFLINKS"                  /**< No. of camera links including master and extension links [in/out]*/
#define CAM_PROP_ISGENICAMCONNECTED_S  "ISGENICAMCONNECTED"         /**< State of GenICam connection (true/false) [out]*/
#define CAM_PROP_MASTERPORT_S          "MASTERPORT"                 /**< Master-port of this camera [out]*/
#define CAM_PROP_MASTERLINK_S          "MASTERLINK"                 /**< Master-State of the according link of a camera [in/out]*/
#define CAM_PROP_EXTENSIONLINK_S       "EXTENSIONLINK"              /**< Extension-State of the according link of a camera [in/out]*/
#define CAM_PROP_MASTERID_S            "MASTERID"                   /**< Master ID of this camera [in/out]*/
#define CAM_PROP_LINKORDER_INDEX_S     "LINKORDERINDEX"             /**< deprecated; don't use; the link order is given implicitly by the link index */
#define CAM_PROP_FGPORT_S              "FGPORT"                     /**< Index of the frame grabber port of an certain link [in/out]*/
#define CAM_PROP_APPLETPORT_S          "APPLETPORT"                 /**< Index of the applet port of an certain link [in/out]*/
#define CAM_PROP_APPLETMASTERID_S      "APPLETMASTERID"             /**< Master ID of this applet port [in/out]*/
#define CAM_PROP_APPLETEXTENSIONID_S   "APPLETLINKORDERINDEX"       /**< The ordering of applet ports [in/out]*/
#define CAM_PROP_ISTRANSCEIVERLOCKED_S "ISTRANSCEIVERLOCKED"        /**< Status of the transceiver lock [out]*/

/**
\brief flags for Sgc_initBoardEx
*/
#define SGC_INIT_FLAG_FORCE_PHY_RESET      0x01  /**< force hard reset of phys*/

#ifdef __cplusplus
extern "C"
{
#endif

  /**
  * \brief Create an Interface object for accessing cameras connected to a frame grabber
  *
  * This function has to be called first, before any access to cameras can be done.
  * By using the retrieved handle, a camera detection, connections to certain cameras
  * or an iteration over present cameras can be performed.
  *
  * The allocated resources have to be released by using the Sgc_freeBoard function.
  *
  * \param fg            frame grabber handle retrieved by FGLIB::fg_init(<APPLETNAME>)
  * \param initFlag      reserved for later usage
  * \param boardHandle   return value of allocated interface resources, needs to be initialized by NULL pointer
  * \retval              0 success, <> 0 see ERR_SGC_XXX constants
  * \see Sgc_freeBoard
  */
  int Sgc_initBoard(Fg_Struct* fg, int initFlag, SgcBoardHandle** boardHandle);

  /**
  * \brief Create an Interface object for accessing cameras connected to a frame grabber
  *
  * This function has to be called first, before any access to cameras can be done.
  * By using the retrieved handle, a camera detection, connections to certain cameras
  * or an iteration over present cameras can be performed.
  *
  * The allocated resources have to be released by using the Sgc_freeBoard function.
  *
  * \param fg           frame grabber handle retrieved by FGLIB::fg_init(<APPLETNAME>)
  * \param initFlag     reserved for later usage
  * \param boardHandle  return value of allocated interface resources, needs to be initialized by NULL pointer
  * \param portMask     bit mask, to enable  specific frame grabber ports, that
  *                     shall be used. Default value is 0xFFFF (all ports)
  * \param slaveMode    currently not used
  * \retval             0 success, <> 0 see ERR_SGC_XXX constants
  * \see Sgc_freeBoard
  */
  int Sgc_initBoardEx(Fg_Struct* fg, unsigned int initFlag, SgcBoardHandle** boardHandle, unsigned int portMask, unsigned int slaveMode);

  /**
  * \brief Release resources allocated by Sgc_initBoard
  * \param boardHandle Interface object to be released. Retrieved by Sgc_initBoard
  * \see Sgc_initBoard
  */
  void Sgc_freeBoard(SgcBoardHandle* boardHandle);

  /**
  * \brief Scans the defined ports of the frame grabber for present cameras
  *
  * This functions scans the frame grabber ports for present cameras (camera discovery).
  * All defined ports will be checked, if cameras are present. The link discovery procedure
  * will be initiated and possible link aggregations will be detected and established, according
  * to the underlaying interfaces (in this case: CoaxPress). The detected link topology and recommended
  * link speed according to the rules of discovery can be queried later by using the camera handles.
  * Therefor the number of cameras and associated camera handles can be retrieved to get further connection
  * details.
  *
  * \param boardHandle      Interface object to be released. Retrieved by Sgc_initBoard
  * \param portMask         bit mask representing the physical frame grabber ports to be scanned; currently, only a full scan is supported
  * \param discoveryTimeout timeout,
  * \param speed            preferred link speed, set to 0 in case of automatic detection at highest speed.
  * \retval                 0 in case of success, elsewise: error codes
  *                         ERR_SGC_TOPOLOGY_MISMATCH,
  *                         ERR_SGC_WAITFORLINK_TIMEOUT,
  *
  * \see Sgc_getCamera, Sgc_getCameraCount
  */
  int Sgc_scanPorts(SgcBoardHandle* boardHandle, const unsigned int portMask, const int discoveryTimeout, const int speed);

  /**
  * \brief Scans the defined ports of the frame grabber for present cameras
  *
  * This functions scans the frame grabber ports for present cameras (camera discovery).
  * All defined ports will be checked, if cameras are present. The link discovery procedure
  * will be initiated and possible link aggregations will be detected and established, according
  * to the underlaying interfaces (in this case: CoaxPress). The detected link topology and recommended
  * link speed according to the rules of discovery can be queried later by using the camera handles.
  * Therefor the number of cameras and associated camera handles can be retrieved to get further connection
  * details.
  *
  * \param boardHandle      Interface object to be released. Retrieved by Sgc_initBoard
  * \param portMask         bit mask representing the physical frame grabber ports to be scanned; currently, only a full scan is supported
  * \param discoveryTimeout timeout,
  * \param speed            preferred link speed, set to 0 in case of automatic detection at highest speed.
  * \param flags            Bit mask representing specific options, see SGC_SP_FLAG_XXXX
  * \retval                 0 in case of success, elsewise: error codes
  *                         ERR_SGC_TOPOLOGY_MISMATCH,
  *                         ERR_SGC_WAITFORLINK_TIMEOUT,
  *
  * \see Sgc_getCamera, Sgc_getCameraCount
  */
  int Sgc_scanPortsEx(SgcBoardHandle* boardHandle, const unsigned int portMask, const int discoveryTimeout, const int speed, const unsigned int flags);

  /**
  * \brief Check for camera
  * \param boardHandle Interface object to be released. Retrieved by Sgc_initBoard
  * \param port        port to check
  * \retval            true: a camera is present at the given port, otherwise false
  */
  unsigned int Sgc_isCameraAvailable(SgcBoardHandle* boardHandle, const unsigned int port);

  /**
  * \brief get manufacturer information from connected camera.
  *
  * This function returns camera specific information from a camera connected to the
  * frame grabber. In general this information is read from technology specific
  * bootstrap registers. The data is returned in form of the SgcCameraInfo
  * structure.
  * \remarks The returned pointer to this structure valid as long as the camera handle
  *          passed as an argument to this function
  *
  * \param cameraHandle      handle to frame grabber generated by Sgc_initBoard()
  * \retval                  camera information obtained from bootstrap registers
  */
   SgcCameraInfo* Sgc_getCameraInfo(SgcCameraHandle* cameraHandle);

  /**
  * \brief get a camera object associated to a certain frame grabber port
  *
  * \param boardHandle       handle to frame grabber generated by Sgc_initBoard()
  * \param port              port index (frame grabber port index)
  * \param cameraHandle      return value of allocated interface resources, needs to be initialized by NULL pointer
  * \retval                  0 success, <> 0 see ERR_SGC_XXX constants
  * \see Sgc_initBoard
  */
  int Sgc_getCamera(SgcBoardHandle* boardHandle, const unsigned int port, SgcCameraHandle** cameraHandle);

  /**
  * \brief Connect to camera by downloading the cameras internal GenICam XML file
  * A call of this function is necessary in order to control the camera by using GenICam
  * features e.g. "Width", Height" etc. and starting / stopping the camera acquisition as well.
  * The functions loads the GenICam form camera and creates a GnICam node map internally for
  * use by calls to Sgc_SetXXXValue, Sgc_getXXXValues.
  * Alternatively the function Sgc_connectCameraWithExternalXml might be called,
  * if no GenICam is available at the camera.
  * \retval                  0 success,
  *                          ERR_SGC_NOT_CONNECTED : no physical connection established
  *                          ERR_SGC_UNEXPECTED: URL is not valid, no GenICam present, error inside XML data
  *                          ERR_SGC_INVALID_CAMHANDLE: camera handle is not valid
  *
  * \see Sgc_connectCameraWithExternalXml
  */
  int Sgc_connectCamera(SgcCameraHandle* cameraHandle);

  /**
  * \brief Connect to Camera by using a certain GenICam XML File from the hard disk
  *
  * \ingroup Initialization
  *
  * \param cameraHandle Camera handle retrieved by calling
  * \param filename     filename of the GenICam XML file for the camera
  * \retval             0: success
  *                     ERR_SGC_FILENOTFOUND    file cannot be opened for reading
  *                     ERR_SGC_INVALIDFILENAME invalid filename, e.g. empty string
  * \see Sgc_connectCamera
  */
  int Sgc_connectCameraWithExternalXml(SgcCameraHandle* cameraHandle, const char* filename);

  /**
  * \brief Disconnect from camera
  *
  * \ingroup Initialization
  *
  * \param cameraHandle Camera handle retrieved by calling
    */
  int Sgc_disconnectCamera(SgcCameraHandle* cameraHandle);

  /**
  \brief switch on Power over CoaxPress/CameraLink/...
  *
  * This function switches the power supply of the given frame grabber ports
  * off and on again in order to perform a reset of the camera. This function
  * is only effectful, if the connected camera does not use an external power
  * supply.
  *
  * Important note: after switching the power on at a certain port, the cameras will
  * boot up. The booting of a camera takes some time, until an access to the camera
  * is possible. This dead time varies between different camera / camera types.
  * Please refer to the camera manufacturer's specification in order to get the information
  * how long the client has to wait after power on.
  *
  * \ingroup Initialization
  *
  * \param boardHandle Interface object to be released. Retrieved by Sgc_initBoard
  * \param portMask    bit mask containing the frame grabber ports to access
  *
  */
  int Sgc_powerSwitch(SgcBoardHandle* boardHandle, const unsigned int portMask);

  /**
  \brief switch on Power over CoaxPress
  *
  * This function allows to change the state of power supply at a certain frame grabber port.
  *
  * Important note: after switching the power on at a certain port, the cameras will
  * boot up. The booting of a camera takes some time, until an access to the camera
  * is possible. This dead time varies between different camera / camera types.
  * Please refer to the camera manufacturer's specification in order to get the information
  * how long the client has to wait after power on.
  *
  * \ingroup Initialization
  *
  * \param boardHandle  Interface object to be released. Retrieved by Sgc_initBoard
  * \param portMask     bit mask containing the frame grabber ports to access
  * \param command      command, how to switch power. values: 0 power off, 1: power on, 2: tear down power for a certain duration
  *
  */
  int Sgc_powerSwitchExt(SgcBoardHandle *boardHandle, const unsigned  int portMask, const unsigned int command);

  /**
  * \brief Register an callback procedure for event notifications
  *
  *
  * \param boardHandle  a handle for accessing the board
  * \param eventHandler structure containing the callback function
  * \param userData     additional data, that gets passed at each callback
  * \retval             0:in case of success,
  *                     <> 0: error code (defined in XXX_error.h)
  *
  * \see Sgc_getErrorDescription,
  *
  */
  int Sgc_RegisterEventCallback(SgcBoardHandle *boardHandle, SgcEventCallback_t eventHandler, void* userData);

  /**
  * \brief Set GenICam Integer value at camera
  *
  * \ingroup GenICam Access
  *
  * \param cameraHandle camera handle instance
  * \param name         feature name
  * \param value        value to be written
  * \retval             0:in case of success,
  *                     <> 0: error code (defined in XXX_error.h)
  *                     ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML* \remarks even if the functions prototype uses a 64 bit value as data, the library supports 32 bit integers only
  *          according to CoaxPress specification
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_setIntegerValue(SgcCameraHandle* cameraHandle, const char* name, int64_t value);

  /**
  \brief Read GenICam Integer value from camera
  *
  * \ingroup GenICam Access
  *
  * \param cameraHandle    camera handle instance
  * \param name            feature name
  * \param valuePtr        buffer for value to be read
  * \retval                 0 in case of success,
  *                         error code (defined in XXX_error.h)
  *                         ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML
  * \remarks even if the functions prototype uses a 64 bit value as data, the library supports 32 bit integers only
  *          according to CoaxPress specification
  * \see Sgc_connectCamera, Sgc_getErrorDescription,
  *
  */
  int Sgc_getIntegerValue(SgcCameraHandle* cameraHandle, const char* name, int64_t* valuePtr);

  /**
  * \brief Set GenICam boolean value at camera
  *
  * \ingroup GenICam Access
  *
  * \param cameraHandle camera handle instance
  * \param name          feature name
  * \param value         value to write ( 1 : true, 0: false)
  * \retval              0: in case of success,
  *                      <> 0: error code (defined in XXX_error.h)
  *                      ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_setBooleanValue(SgcCameraHandle* cameraHandle, const char* name, unsigned int value);

  /**
  * \brief Read GenICam boolean value at camera
  *
  * \ingroup GenICam Access
  *
  * \param cameraHandle camera handle instance
  * \param name         feature name
  * \param valuePtr     buffer for value to be read ( 1 : true, 0: false)
  * \retval             0: in case of success,
  *                     <> 0: error code (defined in XXX_error.h)
  *                     ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_getBooleanValue(SgcCameraHandle* cameraHandle, const char* name, unsigned int* valuePtr);

  /**
  * \brief Set GenICam floating point value at camera
  * \ingroup GenICam Access
  * \param cameraHandle camera handle instance
  * \param name          feature name
  * \param value         value to be set
  * \retval              0:    in case of success,
  *                      <> 0: error code (defined in XXX_error.h)
  *                      ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_setFloatValue(SgcCameraHandle* cameraHandle, const char* name, double value);

  /**
  * \brief Read GenICam floating point value at camera
  * \ingroup GenICam Access
  * \param cameraHandle camera handle instance
  * \param name          feature name
  * \param valuePtr      buffer for value to be read
  * \retval              0:    in case of success,
  *                      <> 0: error code (defined in XXX_error.h)
  *                      ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_getFloatValue(SgcCameraHandle* cameraHandle, const char* name, double* valuePtr);

  /**
  * \brief Set GenICam string value at camera
  *
  * \ingroup GenICam Access
  *
  * \param cameraHandle camera handle instance
  * \param name         feature name
  * \param value        buffer containing a NULL terminated string to be written to the camera
  * \retval             0:    in case of success,
  *                     <> 0: error code (defined in XXX_error.h)
  *                     ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML
  *
  * \remarks i.g. the string features at a camera are limited to a specific size. The Sgc_setStringValue does not do a check for this size.
  *          It is up to the caller to check the length before passing the string to this function.
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_setStringValue(SgcCameraHandle* cameraHandle, const char* name, const char* value);

  /**
  * \brief Read GenICam string value from camera
  *
  * \ingroup GenICam Access
  *
  * \param cameraHandle camera handle instance
  * \param name         feature name
  * \param valuePtr     return value
  * \retval             0 in case of success,
  *                     error code (defined in XXX_error.h)
  *                     ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML
  *
  * 	\see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */

  int Sgc_getStringValue(SgcCameraHandle* cameraHandle, const char* name, const char** valuePtr);

  /**
  *
  * \brief Set GenICam enumeration value at camera
  *
  * \ingroup GenICam Access
  *
  * \param cameraHandle camera handle instance
  * \param name         feature name
  * \param value        enum value to write as text, e.g. "Mono8"
  * \retval             0 in case of success,
  *                     error code (defined in XXX_error.h)
  *                     ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_setEnumerationValue(SgcCameraHandle* cameraHandle, const char* name, const char* value);

  /**
  * \brief Read GenICam integer representation of enumeration value from camera
  *
  * \ingroup GenICam Access
  *
  * \param cameraHandle camera handle instance
  * \param name feature name
  * \param value        buffer for value to be read (value returned as number)
  * \retval             0 in case of success,
  *                     error code (defined in XXX_error.h)
  *                     ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_getEnumerationValue(SgcCameraHandle* cameraHandle, const char* name, int64_t* value);

  /**
  * \brief Read GenICam string representation of enumeration value from camera
  *
  * \ingroup GenICam Access
  *
  * \param cameraHandle camera handle instance
  * \param name feature name
  * \param valuePtr     return value
  * \retval             0 in case of success,
  *                     error code (defined in XXX_error.h)
  *                     ERR_SGC_INVALID_FEATURE  feature name cannot be found at the currently loaded GenICam XML
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_getEnumerationValueAsString(SgcCameraHandle* cameraHandle, const char* name, const char** valuePtr);

  /**
  * \brief Execute a certain GenICam command at the camera
  * \param cameraHandle camera handle instance
  * \param name         name of the command
  * \retval             0 in case of success,
  *                     error code (defined in XXX_error.h)
  *                     ERR_SGC_INVALID_COMMAND  command name cannot be found at the currently loaded GenICam XML
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_executeCommand(SgcCameraHandle* cameraHandle, const char* name);

  /**
  * \brief Check, if a GenICam command is performed completely
  * \param cameraHandle camera handle instance
  * \param name         name of the command
  * \param value        returned data read from the camera by using GenICam
  * \retval             0 command is currently in process,
  *                     1:command is currently in not process
  *                     ERR_SGC_INVALID_COMMAND command name cannot be found at the currently loaded GenICam XML
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_isCommandDone(SgcCameraHandle* cameraHandle, const char* name, unsigned int* value);

  /**
  * \brief activates the the data flow towards the applet and activates the image acquisition at the camera
  * This function is used in combination with the Fg_StartAcquire() function of the standard frame grabber
  * SDK. Acquisition needs to be started from both sides always: from the applet side (Fg_startAcquire())
  * and from the camera side (Sgc_startAcquisition()). The order or the commands should be:
  * 1. start grabbing at the applet side (fg_StartAcquire()) and
  * 2. at the camera side.
  * This ensures a correct image transmission. If the sequence is changed, it cannot be guaranteed, that
  * the transferred images are correctly aligned at the beginning of a frame.
  * \param cameraHandle camera handle instance
  * \param startCamera   send the according GenICam command to activate the image acquisition at the camera
  * \retval              0 in case of success,
  *                      error code (defined in XXX_error.h)
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  */
  int Sgc_startAcquisition(SgcCameraHandle* cameraHandle, unsigned int startCamera);

  /**
  * \brief stops the the data flow towards the applet and stops the image acquisition at the camera (optionally)
  * \param cameraHandle camera handle instance
  * \param stopCamera    send the according GenICam command to stop the image acquisition at the camera
  * \retval              0 in case of success,
  *                      error code (defined in XXX_error.h)
  *
  * \see Sgc_getErrorDescription
  *
  */
  int Sgc_stopAcquisition(SgcCameraHandle* cameraHandle, unsigned int stopCamera);

  /**
  * \brief returns the currently loaded GenICam-XML
  *
  * \param cameraHandle camera handle instance
  * \param buffer        the buffer, where to copy the data, if NULL, the required buffer size is returned
  * \param bufferSize   the fill level or required buffer size of the buffer
  * \retval             0 in case of success,
  *                     error code (defined in XXX_error.h)
  *
  * \see Sgc_connectCamera, Sgc_getErrorDescription
  *
  */
  int Sgc_getGenICamXML(SgcCameraHandle* cameraHandle, char* buffer, size_t* bufferSize);

  /**
  * \brief sets the currently GenICam-XML from a buffer
  *
  * \param cameraHandle camera handle instance
  * \param buffer        the buffer, where to copy the data, if NULL, the required buffer size is returned
  * \param bufferSize   the size of the buffer
  * \retval              0 in case of success,
  *                      error code (defined in ERR_SGC_XXX)
  *
  * \see Sgc_getErrorDescription
  *
  */
  int Sgc_setGenICamXML(SgcCameraHandle* cameraHandle, const char* buffer, const size_t bufferSize);

  /**
  * \brief returns a descriptive text for the error code
  *
  * This function returns a descriptive text for an error code returned from a previous call to a Sgc_XXX
  * function.
  *
  * \param errorCode error code to be decoded.
  *
  * \retval          pointer to a null terminated string containing the according error code
  */
  const char* Sgc_getErrorDescription(const int errorCode);

  /**
  * \brief read a 32 bit integer value from a camera register
  *
  * This function can be used for a direct access to a camera without by bypassing the
  * GenICam XML implementation and register name translations. It might be used, if the
  * register address of the connected camera is known exactly. The byte order gets
  * converted into little endian.
  *
  * \param cameraHandle camera handle instance
  * \param address       address of the camera register (example 0x00002070 "Device version")
  * \param data          pointer to a buffer for the return value
  *
  * \retval              0: success
  *                      ERR_SGC_INVALID_ARG : the pointer to return value is not set
  *                      ERR_SGC_INVALID_CAMHANDLE : the camera handle is not valid
  */
  int Sgc_registerReadFromCamera(SgcCameraHandle* cameraHandle, uint32_t* data, uint64_t address);

  /**
  * \brief write a 32 bit integer value to a camera register
  *
  * This function can be used for a direct access to a camera without by bypassing the
  * GenICam XML implementation and register name translations. It might be used, if the
  * register address of the connected camera is known exactly. Data is assumed to
  * be little in little endian byte order
  *
  * \param cameraHandle camera handle instance
  * \param address       address of the camera register (example 0x00002070 "Device version")
  * \param data          value to write
  *
  * \retval              0: success
  *                      ERR_SGC_INVALID_CAMHANDLE : the camera handle is not valid
  *
  */
  int Sgc_registerWriteToCamera(SgcCameraHandle* cameraHandle, uint32_t data, uint64_t address);

  /**
  * \brief read a string value from a camera register
  * This function can be used for a direct access to a camera without by bypassing the
  * GenICam XML implementation and register name translations. It might be used, if the
  * register address of the connected camera is known exactly.
  *
  * The data will be copied to the buffer given at the data parameter. This buffer needs
  * to be large enough to store the whole string. The number of bytes to be read have to
  * be passed at the length parameter. The string is not necessarily 0 terminated.
  *
  * \param cameraHandle  camera handle instance
  * \param data          address of the camera register (example 0x00002070 "Device version")
  * \param address       address of the camera register (example 0x00002020 "DeviceModelName")
  * \param data          pointer to a buffer for the return value
  * \param length        number of bytes to be read
  * \retval              0: success, ERR_SGC_BUFFER_TOO_SMALL: Buffer too small
  */
  int Sgc_stringReadFromCamera(SgcCameraHandle* cameraHandle, char* data, uint64_t address, unsigned int length);

  /**
  * \brief read a number of bytes from a camera register without a timeout
  *
  * \see Sgc_memoryReadFromCameraWithTimeout
  */
  int Sgc_memoryReadFromCamera(SgcCameraHandle* cameraHandle, void* buffer, uint64_t address, size_t length);

  /**
  * \brief write a number of bytes to a camera register without a timeout
  *
  * \see Sgc_memoryWriteToCameraWithTimeout
  */
  int Sgc_memoryWriteToCamera(SgcCameraHandle* cameraHandle, const void* buffer, uint64_t address, size_t length);

  /**
  * \brief read a number of bytes from a camera register
  *
  * The data will be copied to the buffer given at the buffer parameter. This buffer needs
  * to be large enough to store the whole string. The number of bytes to be read have to
  * be passed at the length parameter. The string is not necessarily 0 terminated.
  *
  * This function can be used for a direct access to a camera by bypassing the
  * GenICam XML implementation and register name translations. It might be used, if the
  * register address of the connected camera is known exactly.
  *
  * \remarks The length of data transfer is checked against GenICam definition of the camera.
  *
  * \param cameraHandle           camera handle instance
  * \param address                address of the camera register (example 0x00002070 "Device version")
  * \param buffer                 pointer to a buffer for the return value
  * \param length                 number of bytes to be read
  * \param timeoutInMilliseconds  timeout for camera response and data transfer. if set to 0 the timeout is disabled
  * \retval              0: success, ERR_SGC_BUFFER_TOO_SMALL: Buffer too small
  */
  int Sgc_memoryReadFromCameraWithTimeout(SgcCameraHandle* cameraHandle, void* buffer, uint64_t address, size_t length, uint32_t timeoutInMilliseconds);

  /**
  * \brief write a number of bytes to a camera register
  *
  * The content of the buffer data will be written to the specified camera register.
  * This function can be used for a direct access to a camera register by bypassing the
  * GenICam XML implementation and register name translations. It might be used, if the
  * register address of the connected camera is known exactly.
  *
  * \remarks The length of data transfer is checked against GenICam definition of the camera
  *
  * \param cameraHandle           camera handle instance
  * \param address                address of the camera register (example 0x00002070 "Device version")
  * \param buffer                 pointer to a buffer for the return value
  * \param length                 number of bytes to be read
  * \param timeoutInMilliseconds  timeout for camera response and data transfer. if set to 0 the timeout is disbled
  * \retval              0: success, ERR_SGC_BUFFER_TOO_SMALL: Buffer too small
  *
  */
  int Sgc_memoryWriteToCameraWithTimeout(SgcCameraHandle* cameraHandle, const void* buffer, uint64_t address, size_t length, uint32_t timeoutInMilliseconds);

  /**
  * \brief get camera specific properties
  *
  * This generic function allows to get various camera properties like
  * current link speed, current stream packet size, state of connection etc.
  * The available properties are defined at the CAM_PROP_XXX constants.
  * Due to the variety of these parameters, the generic functions uses a
  * void pointer to give access to any of these. The type of data (pointer to
  * integer, pointer to double etc.) have to be passed at the property type parameter
  * in order to allow a minimum check for data security.
  * The available types are listed at the  SGC_PROPERTY_TYPE_XXX constants. Some parameter
  * need additional information to address the value correctly. In such cases, the
  * pointer named value has to be used.
  *
  * \param cameraHandle     camera handle instance
  * \param propertyName     string containing the name of property to be accessed, see CAM_PROP_XXX constants
  * \param value            pointer to data structure, where the result value should be used. Needs to be large enough to
  *                         carry the data completely.
  *                         For valid values see CAM_PROP_XXX constants
  * \param propertyType     type of data given at parameter propertyValuePtr (in / out parameter)
  *                         see SGC_PROPERTY_TYPE_XXX constants
  * \param additionalValue  additional parameter to address the the  property (multi-purpose)
  * \retval                 0: success
  *                         ERR_SGC_INVALID_ARG : a needed data structure was not initialized
  *                         ERR_SGC_INVALID_PROPERTY: tried to access an invalid property
  */
  int Sgc_getCameraPropertyWithType(SgcCameraHandle* cameraHandle, const char* propertyName, void* value, unsigned int* propertyType, void* additionalValue);

  /**
  * \brief set camera or camera connection specific properties
  *
  * This generic function allows to get various camera properties like
  * current link speed, current stream packet size, state of connection etc.
  * The available properties are defined at the CAM_PROP_XXX constants.
  * Due to the variety of these parameters, the generic functions uses a
  * void pointer to give access to any of these. The type of data (pointer to
  * integer, pointer to double etc.) have to be passed at the property type parameter
  * in order to allow a minimum check for data security.
  * The available types are listed at the  SGC_PROPERTY_TYPE_XXX constants. Some parameter
  * need additional information to address the value correctly. In such cases, the
  * pointer named value has to be used.
  *
  * \param cameraHandle     camera handle instance
  * \param propertyName     string containing the name of property to be accessed.
  *                         For valid values see CAM_PROP_XXX constants
  * \param value            pointer to data structure, where the result value should be used. Needs to be large enough to
  *                         carry the data completely.
  * \param propertyType     type of data given at parameter propertyValuePtr
  *                         see SGC_PROPERTY_TYPE_XXX constants
  * \param additionalValue  additional parameter to address the the  property (multi-purpose)
  * \retval                 0: success
  *                         ERR_SGC_INVALID_ARG : a needed data structure was not initialized or a data is not accepted
  *                         ERR_SGC_INVALID_PROPERTY: tried to access an invalid property
  */
  int Sgc_setCameraPropertyWithType(SgcCameraHandle* cameraHandle, const char* propertyName, const void* value, int propertyType, void* additionalValue);

  /**
  * \brief set global board properties
  *
  * This generic function allows to set any board property. The property itself is
  * identified by a null terminated string value according to the defined values.
  * The value has to be saved in variables inside the client and a pointer to this variable
  * has to be passed to this function (parameter propertyValuePtr). The data type of this variable
  * depends on the property itself and might be any standard C data type like integer, double, char[], etc.
  * To allow a check inside the function, the used data type has to be passed at the parameter property type.
  * Constants for definition of the data type passed at the propertyValue parameter can be seen at the
  * SGC_PROPERTY_TYPE_XXX definitions. Some properties need additional addressing. In such cases the
  * data has to be passed at the value parameter. In most cases, this pointer might be set to NULL.
  *
  * \param boardHandle      a handle to the according board
  * \param propertyName     property identifier, see PROP_XXX
  * \param propertyValue    pointer to data structure containing the data to be set e.g.
  * \param propertyType     data type of the variable passed at the propertyValuePtr parameter
  * \param additionalValue  additional value for addressing a property, might be NULL
  *
  * \retval                 0 in case of success,
  *                         error code (defined in ERR_SGC_XXX)
  *
  * \see Sgc_getErrorDescription, Sgc_getBoardPropertyWithType
  */
  int Sgc_setBoardPropertyWithType(SgcBoardHandle* boardHandle, const char* propertyName, const void* propertyValue, const int propertyType, void* additionalValue);

  /**
  * \brief get a global board property
  *
  * This generic function allows to get any board property, which is currently active.
  * The property itself is identified by a null terminated string value according to
  * the defined values.
  * The data type of this variable depends on the property itself and might be any
  * standard C data type like integer, double, char[], etc. To allow a check inside the
  * function, the used data type has to be passed at the parameter property type.
  * Constants for definition of the data type passed at the propertyValue parameter can be seen at the
  * SGC_PROPERTY_TYPE_XXX definitions. Some properties need additional addressing. In such cases the
  * data has to be passed at the value parameter. In most cases, this pointer might be set to NULL.
  *
  * \param boardHandle      a handle to the according board
  * \param propertyName     property identifier, see PROP_XXX
  * \param value            pointer to a buffer for returning the data
  * \param additionalValue  pointer to data for addressing specific properties
  * \param propertyType     data type of the variable passed at the propertyValuePtr parameter
  * \param value            additional value for addressing a property, might be NULL
  *
  * \retval                 0 in case of success,
  *                         error code (defined in ERR_SGC_XXX)
  * \see Sgc_getErrorDescription, Sgc_setBoardPropertyWithType
  *
  */
  int Sgc_getBoardPropertyWithType(SgcBoardHandle* boardHandle, const char* propertyName, const void* value, const int propertyType, const void* additionalValue);

  /**
  * \brief get number of the currently detected cameras
  *
  * This function can be used for iterations or loops to obtain the currently present camera objects.
  * It returns both: number of discovered/connected camera handles or unconnected camera handles,
  * which are created by Sgc_GetCamera(port) and might wait for connection.
  *
  * \param boardHandle a handle to the according board
  * \retval        >= 0: number of cameras ( # of camera handles)
  *                <> 0 error code (defined in ERR_SGC_XXX)
  *
  * \see Sgc_getErrorDescription
  */
  int Sgc_getCameraCount(SgcBoardHandle* boardHandle);

  /**
  * \brief get a handle of the currently detected cameras
  *
  * This function can be used for iterations or loops to obtain the currently present camera objects.
  * The range of the index is between 0 and Sgc_getNrOfCameras()-1.
  * It returns both: discovered/connected camera handles or unconnected camera handles, which are created
  * by Sgc_GetCamera(port) and might wait for connection.
  *
  * The camera handle will be returned in the cameraHandle parameter.
  *
  * \param boardHandle  a handle to the according board
  * \param cameraHandle return value as CameraHandle that can be used for further camera access
  *                      NULL in case of an error
  * \param index         index between 0 and Sgc_getNrOfCameras()-1
  *
  * \retval              0: success,
  *                      ERR_SGC_INVALID_ARG :  invalid out parameter
  *
  * \see Sgc_getErrorDescription
  */
  int Sgc_getCameraByIndex(SgcBoardHandle* boardHandle, const unsigned int index, SgcCameraHandle** cameraHandle);

  /**
  * \brief connects a camera handle to a CXP-GenICam camera
  *
  * \deprecated this function is replaced by Sgc_LinkConnect()
  *
  * \see Sgc_LinkConnect
  */
  int DEPRECATED(Sgc_CxpConnect(SgcBoardHandle* boardHandle, SgcCameraHandle* cameraHandle));

  /**
  * \brief disconnects a camera handle to a CXP - GenICam camera
  *
  * \deprecated this function is replaced by Sgc_LinkDisconnect()
  *
  * \see Sgc_LinkDisconnect
  */
  int DEPRECATED(Sgc_CxpDisconnect(SgcCameraHandle* cameraHandle));

  /**
  * \brief connects a camera handle a GenICam capable camera
  *
  * This function can be used when presetting the link connection parameters
  * manually. It configures the frame grabber port(s) associated to the camera
  * handle at the predefined link speeds, and handles the protocol commands needed
  * for creating a connection.
  * It might be used as an alternative to the camera detection by
  * using the discovery procedures (sgc_ScanPorts()).
  *
  * \param boardHandle  a handle to the according board
  * \param cameraHandle camera handle containing the necessary information for
  *                     creating the connection
  * \retval             0: success,
  *                     ERR_SGC_INVALID_ARG :  invalid out parameter
  *
  * \see Sgc_ScanPorts, Sgc_SetCameraProperty, Sgc_LoadBoardConfiguration, Sgc_LinkDisconnect
  */
  int Sgc_LinkConnect(SgcBoardHandle* boardHandle, SgcCameraHandle* cameraHandle);

  /**
  * \brief disconnects a camera handle from a GenICam capable camera
  *
  * \param cameraHandle camera handle containing the necessary information for
  *                     creating the connection
  *
  * \retval             0: success,
  *                     ERR_SGC_INVALID_ARG :  invalid out parameter
  *
  * \see Sgc_LinkConnect
  */
  int Sgc_LinkDisconnect(SgcCameraHandle* cameraHandle);

  /**
  * \brief reads an integer value from the camera
  *
  * \deprecated this function is replaced by Sgc_DirectRegisterRead64()
  *
  * \see Sgc_DirectRegisterRead64
  */
  int DEPRECATED(Sgc_DirectRegisterRead(SgcBoardHandle* boardHandle, const unsigned int port, const uint32_t address, uint32_t* data));

  /**
  * \brief reads an integer value to the camera
  *
  * This function can be used in order to read an integer value from a camera register directly
  *
  * \param boardHandle a handle to the according board
  * \param port        port identifier
  * \param address     camera register address
  * \param data        data to be written
  *
  * \retval            0: success,
  *                    ERR_SGC_INVALID_ARG :  invalid out parameter
  *
  */
  int Sgc_DirectRegisterRead64(SgcBoardHandle* boardHandle, const unsigned int port, const uint64_t address, uint32_t* data);

  /**
  * \brief write an integer value to the camera
  *
  * \deprecated this function is replaced by Sgc_DirectRegisterWrite64()
  *
  * \see Sgc_DirectRegisterWrite64
  */
  int DEPRECATED(Sgc_DirectRegisterWrite(SgcBoardHandle* boardHandle, const unsigned int port, const uint32_t address, const uint32_t data, const unsigned int switchByteOrder));

  /**
  * \brief write a block of data to the camera
  *
  * This function can be used in order to write an block of data to a camera register directly
  * All data according to the parameter buflen is written to the camera
  *
  * \param boardHandle     a handle to the according board
  * \param port            port identifier
  * \param address         camera register address
  * \param data            data read from the camera
  * \param switchByteOrder switches the byte order of data before writing (endianess)
  *
  * \retval               0: success,
  *                       ERR_SGC_INVALID_ARG :  invalid out parameter
  *
  */
  int Sgc_DirectRegisterWrite64(SgcBoardHandle* boardHandle, const unsigned int port, const uint64_t address, const uint32_t data, const unsigned int switchByteOrder);

  /**
  * \brief write a block of data to the camera
  *
  * \deprecated Use Sgc_DirectRegisterReadMem64
  *
  * \see Sgc_DirectRegisterReadMem64
  */
  int DEPRECATED(Sgc_DirectRegisterReadMem(SgcBoardHandle *boardHandle, const unsigned int port, const uint32_t address, unsigned char* data, uint32_t* buflen));

  /**
  * \brief write a block of data to the camera
  *
  * This function can be used in order to write an block of data to a camera register directly
  * All data according to the parameter buflen is written to the camera
  *
  * \param boardHandle    a handle to the according board
  * \param port           port identifier
  * \param address        camera register address
  * \param data           data read from the camera
  * \param buflen         length of the output buffer in bytes, after reading containing the number of bytes read
  *
  * \retval               0: success,
  *                       ERR_SGC_INVALID_ARG :  invalid out parameter
  *
  */
  int Sgc_DirectRegisterReadMem64(SgcBoardHandle *boardHandle, const unsigned int port, const uint64_t address, unsigned char* data, uint32_t* buflen);

  /**
  * \brief write a block of data to the camera
  *
  * \deprecated this function is replaced by Sgc_DirectRegisterWriteMem64()
  *
  * \see Sgc_DirectRegisterWriteMem64
  */
  int DEPRECATED(Sgc_DirectRegisterWriteMem(SgcBoardHandle *boardHandle, const unsigned int port, const uint32_t address, const unsigned char* data, const uint32_t buflen ));

  /**
  * \brief write a block of data to the camera
  *
  * This function can be used in order to write an block of data to a camera register directly
  * All data according to the parameter buflen is written to the camera
  *
  * \param boardHandle    a handle to the according board
  * \param port           port identifier
  * \param address        camera register address
  * \param data           data to be written
  * \param buflen         length of the input buffer
  *
  * \retval               0: success,
  *                       ERR_SGC_INVALID_ARG: invalid out parameter
  *
  */
  int Sgc_DirectRegisterWriteMem64(SgcBoardHandle *boardHandle, const unsigned int port, const uint64_t address, const unsigned char* data, const uint32_t buflen );

  /**
  * \brief write a block of data to the camera
  *
  * \deprecated this function is replaced by Sgc_DirectRegisterReadMemWithTimeOut64()
  *
  * \see Sgc_DirectRegisterReadMemWithTimeOut64
  */
  int DEPRECATED(Sgc_DirectRegisterReadMemWithTimeOut(SgcBoardHandle *boardHandle, const unsigned int port, const uint32_t address, unsigned char* data, uint32_t* buflen, uint32_t timeoutInMilliseconds));

  /**
  * \brief write a block of data to the camera
  *
  * This function can be used in order to write an block of data to a camera register directly
  * All data according to the parameter buflen is written to the camera
  *
  * \param boardHandle           a handle to the according board
  * \param port                  port identifier
  * \param address               camera register address
  * \param data                  data read from the camera
  * \param buflen                length of the output buffer in bytes, after reading containing the number of bytes read
  * \param timeoutInMilliseconds timeout in ms. 0 indicates infinite
  *
  * \retval                      0: success,
  *                              ERR_SGC_INVALID_ARG :  invalid out parameter
  *
  */
  int Sgc_DirectRegisterReadMemWithTimeOut64(SgcBoardHandle *boardHandle, const unsigned int port, const uint64_t address, unsigned char* data, uint32_t* buflen, uint32_t timeoutInMilliseconds);

  /**
  * \brief write a block of data to the camera
  *
  * \deprecated this function is replaced by Sgc_DirectRegisterWriteMemWithTimeOut64
  *
  * \see Sgc_DirectRegisterWriteMemWithTimeOut64
  */
  int DEPRECATED(Sgc_DirectRegisterWriteMemWithTimeOut(SgcBoardHandle *boardHandle, const unsigned int port, const uint32_t address, const unsigned char* data, const uint32_t buflen, uint32_t timeoutInMilliseconds));

  /**
  * \brief write a block of data to the camera
  *
  * This function can be used in order to write an block of data to a camera
  * register directly All data according to the parameter buflen is written
  * to the camera. A timeout can be set in order to take care of the cameras's
  * processing time.
  *
  * \param boardHandle           a handle to the according board
  * \param port                  port identifier
  * \param address               camera register address
  * \param data                  data to be written
  * \param buflen                length of the input buffer
  * \param timeoutInMilliseconds timeout in ms. 0 indicates infinite
  *
  * \retval                      0: success,
  *                              ERR_SGC_INVALID_ARG: invalid out parameter
  *
  */
  int Sgc_DirectRegisterWriteMemWithTimeOut64(SgcBoardHandle *boardHandle, const unsigned int port, const uint64_t address, const unsigned char* data, const uint32_t buflen, uint32_t timeoutInMilliseconds);

  /**
  * \brief save connection parameters of the board to a file
  *
  * This function can be used to store the current camera connection parameters
  * for all links/cameras to the hard disk. All Connection data like
  * number of links, ordering of the links, link speed and assignment to then
  * applet interface is exported and can be reloaded afterwards. It can be used
  * e.g. after an automatic discovery or a manual setup of link configuration
  * had taken place. The function can be used in combination with
  * Sgc_LoadBoardConfiguration(), which reloads the parameters, that had been
  * saved by using this method.
  *
  * \param boardHandle    a handle to the according board
  * \param configFileName filename
  *
  * \retval               0: success,
  *                       ERR_SGC_NO_DATA : currently no link configuration present
  *                                             (e.g. if no camera is found)
  *                       ERR_SGC_FILEACCESS: file access failure when saving to hard disk
  *                       ERR_SGC_INVALID_DATA: current link configuration is invalid
  *
  * \see Sgc_LoadBoardConfiguration, Sgc_CxpConnect, Sgc_ScanPorts, Sgc_SetCameraProperty
  *
  */
  int Sgc_SaveBoardConfiguration(SgcBoardHandle* boardHandle, const char* configFileName);

  /**
  * \brief load connection parameters from a file
  *
  * This function can be used to load the current connection parameters
  * for all links/cameras from the hard disk in order to create camera
  * connections. It can be used as an alternative approach for setting up
  * a connection instead of the camera discovery. The predefined connection
  * parameters will be used and the connection can be established afterwards
  * by using exactly these parameters. In case of an error condition
  * the calls to established will fail, which can can be evaluated by the
  * software.
  *
  * \param boardHandle    a handle to the according board
  * \param configFileName filename
  *
  * \retval               0: success,
  *                       ERR_SGC_INVALID_ARG:  invalid out parameter
  *                       ERR_SGC_INVALIDFILENAME: file not found
  *                       ERR_SGC_INVALID_DATA: the data inside the file is invalid
  *
  * \see Sgc_CxpConnect, Sgc_ScanPorts, Sgc_SetCameraProperty
  *
  */
  int Sgc_LoadBoardConfiguration(SgcBoardHandle* boardHandle, const char* configFileName);

  /**
  * \brief save the current camera parameters to a file
  *
  * This function can be used to save the values of all camera registers
  * based on the camera's GenICam interface to a file. Purpose of this
  * method is the possibility of exporting and reloading the current state
  * of parametrization of the camera.
  *
  * \param cameraHandle  a handle to the according camera
  * \param paramFileName filename, where to save the data
  * \param flags         specific flags see :
  *
  * \retval              0: success,
  *                      ERR_SGC_INVALID_ARG :  invalid out parameter
  *
  * \see Sgc_LoadCameraParameters, Sgc_connectCamera
  *
  */
  int Sgc_SaveCameraParameters(SgcCameraHandle* cameraHandle, const char* paramFileName, uint32_t flags);

  /**
  * \brief load the current camera parameters from a file
  *
  * This function can be used to load values of camera registers
  * from a file. Parameters are loaded and carried over to the camera
  * according to the camera's GenICam register description.
  *
  * \param cameraHandle  a handle to the according camera
  * \param paramFileName filename
  * \param flags         specific flags see :
  *
  * \retval              0: success,
  *                      ERR_SGC_INVALID_ARG :  invalid out parameter
  *
  * \see Sgc_SaveCameraParameters, Sgc_connectCamera,
  */
  int Sgc_LoadCameraParameters(SgcCameraHandle* cameraHandle, const char* paramFileName, uint32_t flags);

/**
* \brief load a connection profile
*
* This function initializes the board, loads the board configuration file
* and establishes connections to the cameras. The result of these operations is
* represented by the returned connection profile structure.
*
* The allocated resources have to be released by using the Sgc_UnloadConnectionProfile function.
*
* \param fg                         frame grabber handle retrieved by FGLIB::fg_init(<APPLETNAME>)
* \param boardConfigurationFilePath path to th board configuration file
* \param connectionProfilePtr       pointer address for the allocated connection profile structure
*
* \retval                           0 success, <> 0 see ERR_SGC_XXX constants
* \see Sgc_UnloadConnectionProfile
*/
int Sgc_LoadConnectionProfile(Fg_Struct* fg, const char* boardConfigurationFilePath, SgcConnectionProfile** connectionProfilePtr);

/**
* \brief unload a connection profile
*
* This function frees the resources allocated by a previous call to Sgc_LoadConnectionProfile function.
*
* \param connectionProfile address for the allocated connection profile structure
*
* \retval                  0: success,
*                          ERR_SGC_INVALID_ARG : invalid connection profile address

* \see Sgc_LoadConnectionProfile
*/
int Sgc_UnloadConnectionProfile(SgcConnectionProfile* connectionProfile);

/**
* \brief store current low level link profile into file
*
* This function stores the current low level link profile into a file in order to
* restore it on another (slave) board. This is only needed in a master/slave scenario.
* Currently only CXP is supported.
*
* \param boardHandle    a handle to the according board
* \param file           filename
*
* \retval                           0 success, <> 0 see ERR_SGC_XXX constants
* \see Sgc_RestoreLowLevelLinkProfile
*/
int Sgc_StoreLowLevelLinkProfile(SgcBoardHandle* boardHandle, const char* file);

/**
* \brief restore low level link profile from file to board
*
* This function applies a stored low level link profile from file to a (slave) board.
* This is only needed in a master/slave scenario. Currently only CXP is supported.
*
* Note: Every discovery cycle on this board or change in speed settings of the connected
* remote device reverts the link profile, so it must be restored again!
*
* \param boardHandle    a handle to the according board
* \param file           filename
*
* \retval                           0 success, <> 0 see ERR_SGC_XXX constants
* \see Sgc_StoreLowLevelLinkProfile
*/
int Sgc_RestoreLowLevelLinkProfile(SgcBoardHandle* boardHandle, const char* file);

/* INTERNAL FUNCTIONS*/
/**
* for internal use only
*
*/
int Sgc_callInternalFunction(int id, const void* const in, void* const out);

/** Id for Siso Internal Function 'Reset PHY' */
#define SISO_INTERNAL_FUNCTION__RESET_PHY        1
#define SISO_INTERNAL_FUNCTION__ENABLE_LOGGING   2
#define SISO_INTERNAL_FUNCTION__ENABLE_PROFILING 3

#ifdef __cplusplus
}
#endif
#endif
