/**
 * gbe.h
 *
 * Copyright (c) 2002-2017 Silicon Software GmbH, All Rights Reserved.
 *
 * \file gbe.h
 *
 * \brief GigE Vision functions
 *
 * \author Silicon Software GmbH
 */
 
#ifndef __SISO_GBE_H__
#define __SISO_GBE_H__

#if defined(_MSC_VER) && (_MSC_VER < 1600)
#include <msinttypes/stdint.h>
#else
#include <stdint.h>
#endif

#include <sys/types.h>

/** handle structure for boards */
struct BoardHandle;
/** handle structure for cameras */
struct CameraHandle;

#ifdef __cplusplus
namespace GenApi { class CNodeMapRef; }
#endif

/** flags: none */
#define GBE_FLAGS_NONE        0x0
/** flags: stop on error */
#define GBE_FLAGS_STOPONERROR 0x1

/* property types */

/** property type is unsigned int (32bit) */
#define GBE_PROPERTY_TYPE_UINT32 0x1
 /** property type is unsigned int (32bit) */
#define GBE_PROPERTY_TYPE_UINT   GBE_PROPERTY_TYPE_UINT32
/** property type is unsigned int (16bit) */
#define GBE_PROPERTY_TYPE_UINT16 0x2
/** property type is unsigned int (64bit) */
#define GBE_PROPERTY_TYPE_UINT64 0x3

/* properties */

/** enable LAG mode [in/out] */
#define GBE_BOARDPROP_LAG                    "lag"
/** set payload width in bytes for port 0 [in] */
#define GBE_BOARDPROP_PAYLOADWIDTH0          "payloadWidth0"
/** set payload width in bytes for port 1 [in] */
#define GBE_BOARDPROP_PAYLOADWIDTH1          "payloadWidth1"
/** set payload width in bytes for port 2 [in] */
#define GBE_BOARDPROP_PAYLOADWIDTH2          "payloadWidth2"
/** set payload width in bytes for port 3 [in] */
#define GBE_BOARDPROP_PAYLOADWIDTH3          "payloadWidth3"
/** current IP for portgroup 0 [out] */
#define GBE_BOARDPROP_IP0                    "ip0"
/** current IP for portgroup 1 [out] */
#define GBE_BOARDPROP_IP1                    "ip1"
/** current IP for portgroup 2 [out] */
#define GBE_BOARDPROP_IP2                    "ip2"
/** current IP for portgroup 3 [out] */
#define GBE_BOARDPROP_IP3                    "ip3"
/** current GVSP image port for portgroup 0 [out] */
#define GBE_BOARDPROP_GVSPIMAGEPORT0         "gvspImagePort0"
/** current GVSP image port for portgroup 1 [out] */
#define GBE_BOARDPROP_GVSPIMAGEPORT1         "gvspImagePort1"
/** current GVSP image port for portgroup 2 [out] */
#define GBE_BOARDPROP_GVSPIMAGEPORT2         "gvspImagePort2"
/** current GVSP image port for portgroup 3 [out] */
#define GBE_BOARDPROP_GVSPIMAGEPORT3         "gvspImagePort3"
/** current CloseImageTimeout for portgroup 0 [in/out] */
#define GBE_BOARDPROP_CLOSEIMAGETIMEOUT0     "closeImageTimeout0"
/** current CloseImageTimeout for portgroup 1 [in/out] */
#define GBE_BOARDPROP_CLOSEIMAGETIMEOUT1     "closeImageTimeout1"
/** current CloseImageTimeout for portgroup 2 [in/out] */
#define GBE_BOARDPROP_CLOSEIMAGETIMEOUT2     "closeImageTimeout2"
/** current CloseImageTimeout for portgroup 3 [in/out] */
#define GBE_BOARDPROP_CLOSEIMAGETIMEOUT3     "closeImageTimeout3"

/** start cable diagnostic for port 0 (if supported) [writeonly] */
#define GBE_BOARDPROP_START_CABLEDIAGNOSTIC0 "startCableDiagnostic0"
/** start cable diagnostic for port 1 (if supported) [writeonly] */
#define GBE_BOARDPROP_START_CABLEDIAGNOSTIC1 "startCableDiagnostic1"
/** start cable diagnostic for port 2 (if supported) [writeonly] */
#define GBE_BOARDPROP_START_CABLEDIAGNOSTIC2 "startCableDiagnostic2"
/** start cable diagnostic for port 3 (if supported) [writeonly] */
#define GBE_BOARDPROP_START_CABLEDIAGNOSTIC3 "startCableDiagnostic3"

/** POE on/off state of port 0 (if supported) [in/out] (uint16) */
#define GBE_BOARDPROP_POE0                   "poe0"
/** POE on/off state of port 1 (if supported) [in/out] (uint16) */
#define GBE_BOARDPROP_POE1                   "poe1"
/** POE on/off state of port 2 (if supported) [in/out] (uint16) */
#define GBE_BOARDPROP_POE2                   "poe2"
/** POE on/off state of port 3 (if supported) [in/out] (uint16) */
#define GBE_BOARDPROP_POE3                   "poe3"

/** POE state of port 0 (if supported) [out] (uint64) */
#define GBE_BOARDPROP_POE_STATE0             "poeState0"
/** POE state of port 1 (if supported) [out] (uint64) */
#define GBE_BOARDPROP_POE_STATE1             "poeState1"
/** POE state of port 2 (if supported) [out] (uint64) */
#define GBE_BOARDPROP_POE_STATE2             "poeState2"
/** POE state of port 3 (if supported) [out] (uint64) */
#define GBE_BOARDPROP_POE_STATE3             "poeState3"

/** POE sensors of port 0 (if supported) [out] (uint64) */
#define GBE_BOARDPROP_POE_SENSOR0            "poeSensor0"
/** POE sensors of port 1 (if supported) [out] (uint64) */
#define GBE_BOARDPROP_POE_SENSOR1            "poeSensor1"
/** POE sensors of port 2 (if supported) [out] (uint64) */
#define GBE_BOARDPROP_POE_SENSOR2            "poeSensor2"
/** POE sensors of port 3 (if supported) [out] (uint64) */
#define GBE_BOARDPROP_POE_SENSOR3            "poeSensor3"

/** Link state of port 0 (if supported) [out] */
#define GBE_BOARDPROP_LINKSTATE0             "linkState0"
/** Link state of port 1 (if supported) [out] */
#define GBE_BOARDPROP_LINKSTATE1             "linkState1"
/** Link state of port 2 (if supported) [out] */
#define GBE_BOARDPROP_LINKSTATE2             "linkState2"
/** Link state of port 3 (if supported) [out] */
#define GBE_BOARDPROP_LINKSTATE3             "linkState3"

/** Connection state of port 0 (if supported) [out] (uint32) */
#define GBE_BOARDPROP_CONNECTIONSTATE0       "connectionState0"
/** Connection state of port 1 (if supported) [out] (uint32) */
#define GBE_BOARDPROP_CONNECTIONSTATE1       "connectionState1"
/** Connection state of port 2 (if supported) [out] (uint32) */
#define GBE_BOARDPROP_CONNECTIONSTATE2       "connectionState2"
/** Connection state of port 3 (if supported) [out] (uint32) */
#define GBE_BOARDPROP_CONNECTIONSTATE3       "connectionState3"

/** RXER count of port 0 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_RXERCOUNT0             "rxerCount0"
/** RXER count of port 1 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_RXERCOUNT1             "rxerCount1"
/** RXER count of port 2 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_RXERCOUNT2             "rxerCount2"
/** RXER count of port 3 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_RXERCOUNT3             "rxerCount3"

/** Idle error count of port 0 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_IDLEERRORCOUNT0        "idleErrorCount0"
/** Idle error count of port 1 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_IDLEERRORCOUNT1        "idleErrorCount1"
/** Idle error count of port 2 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_IDLEERRORCOUNT2        "idleErrorCount2"
/** Idle error count of port 3 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_IDLEERRORCOUNT3        "idleErrorCount3"

/** Fault flags of port 0 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_FAULTFLAGS0            "faultFlags0"
/** Fault flags of port 1 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_FAULTFLAGS1            "faultFlags1"
/** Fault flags of port 2 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_FAULTFLAGS2            "faultFlags2"
/** Fault flags of port 3 (if supported) [out] (uint16) */
#define GBE_BOARDPROP_FAULTFLAGS3            "faultFlags3"

/** set/get MTU size [in/out] */
#define GBE_CAMPROP_MTU                      "mtu"
/** set/get command packet timeout (ms) [in/out] */
#define GBE_CAMPROP_COMMANDPACKET_TIMEOUT    "commandPacketTimeout"
/** set/get command packet retries count [in/out] */
#define GBE_CAMPROP_COMMANDPACKET_RETRIES    "commandPacketRetries"
/** get number of action signals [out] */
#define GBE_CAMPROP_NUMBEROFACTIONSIGNALS    "numberOfActionSignals"
/** set/get action device key [in/out] */
#define GBE_CAMPROP_ACTIONDEVICEKEY          "actionDeviceKey"
/** set/get action group key for action 0 [in/out] */
#define GBE_CAMPROP_ACTIONGROUPKEY0          "actionGroupKey0"
/** set/get action group mask for action 0 [in/out] */
#define GBE_CAMPROP_ACTIONGROUPMASK0         "actionGroupMask0"
/** set/get action group key for action 1 [in/out] */
#define GBE_CAMPROP_ACTIONGROUPKEY1          "actionGroupKey1"
/** set/get action group mask for action 1 [in/out] */
#define GBE_CAMPROP_ACTIONGROUPMASK1         "actionGroupMask1"
/** set/get action group key for action 2 [in/out] */
#define GBE_CAMPROP_ACTIONGROUPKEY2          "actionGroupKey2"
/** set/get action group mask for action 2 [in/out] */
#define GBE_CAMPROP_ACTIONGROUPMASK2         "actionGroupMask2"
/** set/get action group key for action 3 [in/out] */
#define GBE_CAMPROP_ACTIONGROUPKEY3          "actionGroupKey3"
/** set/get action group mask for action 4 [in/out] */
#define GBE_CAMPROP_ACTIONGROUPMASK3         "actionGroupMask3"

/** set/get firmware action device key (if supported) [in/out] */
#define GBE_CAMPROP_FWAC_DEVICEKEY           "fwacDeviceKey"
/** set/get firmware action group key (if supported) [in/out] */
#define GBE_CAMPROP_FWAC_GROUPKEY            "fwacGroupKey"
/** set/get firmware action group mask (if supported) [in/out] */
#define GBE_CAMPROP_FWAC_GROUPMASK           "fwacGroupMask"

/** set/get firmware action enable flag (if supported) [in/out] */
#define GBE_CAMPROP_FWAC_ENABLE              "fwacEnable"
/** set/get firmware action command acknowledge timeout (if supported) [in/out] */
#define GBE_CAMPROP_FWAC_TIMEOUT             "fwacTimeout"
/** set/get firmware action command pending count (if supported) [out] */
#define GBE_CAMPROP_FWAC_PENDING             "fwacPending"

/** set/get extended ID mode (if supported) [in/out] */
#define GBE_CAMPROP_EXTENDED_ID              "extendedId"

#pragma pack(push)
#pragma pack(1)

/** structure for camera infos */
struct CameraInfo
{
    /** manufactor name */
    char manufactor_name[32];
    /** model name */
    char model_name[32];
    /** device version */
    char device_version[32];
    /** manufacturer info */
    char manufacturer_info[48];
    /** serial number */
    char serial_number[16];
    /** user name */
    char user_name[16];

    /** MAC address */
    uint8_t mac[6];

    /** IP address */
    uint32_t ipv4;
};

/** event type "Discovery Change": the camera environment for the board has changed (Board Event) */
#define GBE_EVENT_DISCOVERY_CHANGE         100
/** event type "Cable Diagnostic Status": cable diagnostic status changed (Board Event) */
#define GBE_EVENT_CABLEDIAGNOSTIC_STATUS   110
/** event type "Temperature Alert": temperature alert (Board Event) */
#define GBE_EVENT_TEMPERATURE_ALERT        120
/** event type "PHY IRQ": phy interrupt occurred (Board Event) */
#define GBE_EVENT_PHY_IRQ                  130
/** event type "Camera Connection Lost": the connection to the connected camera has been lost (Camera Event) */
#define GBE_EVENT_CAMERA_CONNECTION_LOST   200
/** event type "GEV Event": an GigE Vision event occurred (Camera Event) */
#define GBE_EVENT_GEVEVENT                 300
/** event type "Corrupted Frame": Event of type "Corrupted Frame" occurred (Camera Event) */
#define GBE_EVENT_CORRUPTED_FRAME          400
/** event type "External Frame Loss": Event of type "External Frame Loss" occurred (Camera Event) */
#define GBE_EVENT_EXTERNAL_FRAME_LOSS      401
/** event type "Lost Frame": Event of type "Lost Frame" occurred (Camera Event) */
#define GBE_EVENT_LOST_FRAME               402
/** event type "Broken Frame": Event of type "Broken Frame" occurred (Camera Event) */
#define GBE_EVENT_BROKEN_FRAME             403
/** event type "ActionCommand Lost": Event of type "Action Command Lost" occurred (Camera Event) */
#define GBE_EVENT_ACTIONCOMMAND_LOST       404
/** event type "ActionCommand Timeout": Event of type "Action Command Timeout" occurred (Camera Event) */
#define GBE_EVENT_ACTIONCOMMAND_TIMEOUT    405

/** data structure for events received from the board and camera callback functions */
struct GbeEventInfo
{
    /** event type identifier */
    uint16_t type;
    /** event data */
    union {
        /** GBE_EVENT_DISCOVERY_CHANGE */
        struct {
            /** the port where the camera environment has changed (GBE_EVENT_DISCOVERY_CHANGE) */
            uint32_t port;
            /** discovery change flags [Bit 0 = 1: camera added, Bit 0 = 0: camera removed] (GBE_EVENT_DISCOVERY_CHANGE) */
            uint32_t flags;
        } discovery;
        /** GBE_EVENT_CABLEDIAGNOSTIC_STATUS */
        struct {
            /** status type (0 = Start, 1 = Stop, 2 = Status) (GBE_EVENT_CABLEDIAGNOSTIC_STATUS) */
            uint32_t type;
            /** the physical port of the cable diagnostics (GBE_EVENT_CABLEDIAGNOSTIC_STATUS) */
            uint16_t port;
            /** the differential pair (GBE_EVENT_CABLEDIAGNOSTIC_STATUS) */
            uint16_t differentialPair;
            /** the cable diagnostic result (0 = OK, 1 = Open Cable Fault, 2 = Short Cable Fault (Same Pair), 4 = Timeout, 5 = Short Cable Fault (Cross Pair)) (GBE_EVENT_CABLEDIAGNOSTIC_STATUS) */
            uint16_t result;
            /** the cable diagnostic distance to cable fault (Unit: centimeters (Marvell PHYs)) (GBE_EVENT_CABLEDIAGNOSTIC_STATUS) */
            uint16_t faultDistance;
        } cablediagnostic;
        /** GBE_EVENT_TEMPERATURE_ALERT */
        struct {
            /** the physical port of the temperature alert (GBE_EVENT_TEMPERATURE_ALERT) */
            uint16_t port;
        } temperature_alert;
        /** GBE_EVENT_PHY_IRQ */
        struct {
            /** occurred irq type (GBE_EVENT_PHY_IRQ)
                0 = Link Up
                1 = Remote Fault
                2 = Link Down
                4 = Parallel Detect Fault
                6 = Receive Error
            */
            uint32_t type;
            /** the physical source port of the irq (GBE_EVENT_PHY_IRQ) */
            uint16_t port;
        } phy_irq;
        /** GBE_EVENT_GEVEVENT */
        struct {
            /** the request id field from the GigE Vision Event network packet (GBE_EVENT_GEVEVENT) */
            uint16_t reqId;
            /** the index field from the GigE Vision Event network packet (GBE_EVENT_GEVEVENT) */
            uint16_t index;
            /** the event id field from the GigE Vision Event network packet (GBE_EVENT_GEVEVENT) */
            uint16_t eventId;
            /** the stream channel index field from the GigE Vision Event network packet (GBE_EVENT_GEVEVENT) */
            uint16_t streamChannelIndex;
            /** the block id field from the GigE Vision Event network packet (==> blockId64 % 65536) (GBE_EVENT_GEVEVENT) */
            uint16_t blockId;
            /** the timestamp field from the GigE Vision Event network packet (GBE_EVENT_GEVEVENT) */
            uint64_t timestamp;
            /** the data length field from the GigE Vision EventData network packet (GBE_EVENT_GEVEVENT) */
            uint16_t dataLen;
            /** pointer to the data field from the GigE Vision EventData network packet (GBE_EVENT_GEVEVENT) */
            const uint8_t* data;
            /** the block id (64 bit) field from the GigE Vision Event network packet (GBE_EVENT_GEVEVENT) */
            uint64_t blockId64;
        } gevevent;
        /** GBE_EVENT_CORRUPTED_FRAME */
        struct {
            /** blockId */
            uint32_t blockId;
            /** packetId */
            uint16_t packetId;
        } corrupted_frame;
        /** GBE_EVENT_EXTERNAL_FRAME_LOSS */
        struct {
            /** pbId */
            uint32_t pbId;
            /** cbId */
            uint32_t cbId;
        } external_frame_loss;
        /** GBE_EVENT_LOST_FRAME */
        struct {
            /** frameId */
            uint32_t frameId;
        } lost_frame;
        /** GBE_EVENT_BROKEN_FRAME */
        struct {
            /** frameId */
            uint32_t frameId;
        } broken_frame;
        /** GBE_EVENT_ACTIONCOMMAND_LOST */
        struct {
            /* no data */
        } actioncommand_lost;
        /** GBE_EVENT_ACTIONCOMMAND_TIMEOUT */
        struct {
            /** actCount */
            uint16_t actCount;
        } actioncommand_timeout;
    } data;
};

#pragma pack(pop)

/* callback function definitions */

/**
	\brief Callback function definition for Board Events
	\param boardHandle the handle to the board the event is triggered from
	\param eventInfo event details/data
	\param userData user data pointer from the matching call to Gbe_registerBoardEventCallback
	\return use 0 to indicate success (currently the return value will be ignored)
*/
typedef int (*Gbe_BoardEventCallbackFunc_t)(struct BoardHandle *boardHandle, const GbeEventInfo * const eventInfo, const void* userData);

/**
	\brief Callback function definition for Camera Events
	\param cameraHandle the handle to the camera the event is triggered from
	\param eventInfo event details/data
	\param userData user data pointer from the matching call to Gbe_registerCameraEventCallback
	\return use 0 to indicate success (currently the return value will be ignored)
*/
typedef int (*Gbe_CameraEventCallbackFunc_t)(struct CameraHandle *cameraHandle, const GbeEventInfo * const eventInfo, const void* userData);

#ifdef __cplusplus
extern "C" {
#endif

/**
	\brief Initializes the Gbe board
	\param board board id
	\param init_flag initialization flags, should be 0
	\param board_handle_ptr pointer to the board handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_initBoard(int board, int init_flag, struct BoardHandle** board_handle_ptr);

/**
	\brief Releases the Gbe board
	\param board_handle board handle instance
*/
void Gbe_freeBoard(struct BoardHandle *board_handle);

/**
	\brief Check if the service connection is still valid
	\param board_handle board handle instance
*/
int Gbe_isServiceConnectionValid(struct BoardHandle *board_handle);

/**
	\brief Scans the network for cameras
	\param board_handle board handle instance
	\param port port number
	\param discovery_timeout timeout for the camera discovery
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_scanNetwork(struct BoardHandle *board_handle, int port, int discovery_timeout);

/**
	\brief Get the number of cameras found on a port
	\param board_handle board handle instance
	\param port port number
	\return number of cameras found
*/
int Gbe_getCameraCount(struct BoardHandle *board_handle, int port);

/**
	\brief Get the first camera found on a port
	\param board_handle board handle instance
	\param port port number
	\param camera_handle_ptr pointer to the camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getFirstCamera(struct BoardHandle *board_handle, int port, struct CameraHandle **camera_handle_ptr);

/**
	\brief Get camera by index on a port
	\param board_handle board handle instance
	\param port port number
	\param index index of the camera
	\param camera_handle_ptr pointer to the camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getCameraByIndex(struct BoardHandle *board_handle, int port, unsigned int index, struct CameraHandle **camera_handle_ptr);

/**
	\brief Get camera by MAC on a port
	\param board_handle board handle instance
	\param port port number
	\param mac MAC address of the camera
	\param camera_handle_ptr pointer to the camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getCameraByMac(struct BoardHandle *board_handle, int port, uint8_t mac[6], struct CameraHandle **camera_handle_ptr);

/**
	\brief Get camera by IP on a port
	\param board_handle board handle instance
	\param port port number
	\param ip IP address of the camera
	\param camera_handle_ptr pointer to the camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getCameraByIp(struct BoardHandle *board_handle, int port, uint32_t ip, struct CameraHandle **camera_handle_ptr);

/**
	\brief Get camera by camera user name on a port
	\param board_handle board handle instance
	\param port port number
	\param user_name user name of the camera (from the camera bootstrap registers)
	\param camera_handle_ptr pointer to the camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getCameraByUserName(struct BoardHandle *board_handle, int port, char* user_name, struct CameraHandle **camera_handle_ptr);

/**
	\brief Get camera info
	\param camera_handle camera handle instance
	\return camera info structure
	\see CameraInfo
*/
struct CameraInfo* Gbe_getCameraInfo(struct CameraHandle *camera_handle);

/**
	\brief Releases the camera
	\param camera_handle camera handle instance
*/
void Gbe_freeCamera(struct CameraHandle *camera_handle);

/**
	\brief Establish camera connection
	\param camera_handle camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_connectCamera(struct CameraHandle *camera_handle);

/**
	\brief Establish camera connection
	\param camera_handle camera handle instance
	\param filename path to external xml file
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_connectCameraWithExternalXml(struct CameraHandle *camera_handle, const char* filename);

/**
	\brief Release camera connection
	\param camera_handle camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_disconnectCamera(struct CameraHandle *camera_handle);

/**
	\brief Establish local UDP socket
	\param board_handle board handle instance
	\param port port number
	\param local_udp_port local udp port
	\param flags connection flags
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_connectUdpSocket(struct BoardHandle *board_handle, int port, uint16_t *local_udp_port, uint32_t flags);

/**
	\brief Remove local UDP socket
	\param board_handle board handle instance
	\param port port number
	\param local_udp_port local udp port
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_disconnectUdpSocket(struct BoardHandle *board_handle, int port, uint16_t local_udp_port);

/**
	\brief Send UDP payload via UDP socket
	\param board_handle board handle instance
	\param port port number
	\param local_udp_port local udp port
	\param remote_ipv4 remote ip address
	\param remote_port remote udp port
	\param buffer pointer to datagram
	\param length size of datagram
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_sendUdpPayload(struct BoardHandle *board_handle, int port, uint16_t local_udp_port, uint32_t remote_ipv4, uint16_t remote_port, const void* buffer, size_t length);

/**
	\brief Recv UDP datagram via camera socket
	\param board_handle board handle instance
	\param port port number
	\param local_udp_port local udp port
	\param buffer pointer to datagram buffer
	\param length size of datagram buffer
	\param timeout timeout in milliseconds
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_recvUdpDatagram(struct BoardHandle *board_handle, int port, uint16_t local_udp_port, void* buffer, size_t *length, int32_t timeout);

/**
	\brief Sends a FORCEIP command over selected port
	\param board_handle board handle instance
	\param port port number
	\param mac MAC address of the target camera
	\param ipv4 IP address to force (use 0 for restarting ip configuration cycle)
	\param ipv4_mask IP address mask to force
	\param ipv4_gateway IP gateway address to force
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_sendForceIpCommand(struct BoardHandle *board_handle, int port, uint8_t mac[6], uint32_t ipv4, uint32_t ipv4_mask, uint32_t ipv4_gateway);

/**
	\brief Sends an ACTION command over selected port
	\param board_handle board handle instance
	\param port port number
	\param ipv4 target IP address of the command
	\param device_key Device Key of the command
	\param group_key Group Key of the command
	\param group_mask Group Mask of the command
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_sendActionCommand(struct BoardHandle *board_handle, int port, uint32_t ipv4, uint32_t device_key, uint32_t group_key, uint32_t group_mask);

/**
	\brief Switch established camera connection to another board/port
	\param camera_handle camera handle instance
	\param board_handle board handle instance
	\param port port number
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_switchCameraConnection(struct CameraHandle *camera_handle, struct BoardHandle *board_handle, int port);

/**
	\brief Set GenICam Integer value
	\param camera_handle camera handle instance
	\param name feature name
	\param value value to write
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_setIntegerValue(struct CameraHandle *camera_handle, const char* name, int64_t value);

/**
	\brief Get GenICam Integer value
	\param camera_handle camera handle instance
	\param name feature name
	\param value_ptr pointer to value
	\return Gbe error code
	\see Gbe_getErrorDescription
*/
int Gbe_getIntegerValue(struct CameraHandle *camera_handle, const char* name, int64_t* value_ptr);

/**
	\brief Get GenICam Integer limit values
	\param camera_handle camera handle instance
	\param name feature name
	\param min_value_ptr pointer to min value
	\param max_value_ptr pointer to max value
	\param inc_value_ptr pointer to inc value
	\return Gbe error code
	\see Gbe_getErrorDescription
*/
int Gbe_getIntegerValueLimits(struct CameraHandle *camera_handle, const char* name, int64_t* min_value_ptr, int64_t* max_value_ptr, int64_t *inc_value_ptr);

/**
	\brief Set GenICam Boolean value
	\param camera_handle camera handle instance
	\param name feature name
	\param value value to write (0 = false, otherwise true)
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_setBooleanValue(struct CameraHandle *camera_handle, const char* name, unsigned int value);

/**
	\brief Get GenICam Boolean value
	\param camera_handle camera handle instance
	\param name feature name
	\param value_ptr pointer to value
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getBooleanValue(struct CameraHandle *camera_handle, const char* name, unsigned int* value_ptr);

/**
	\brief Set GenICam Float value
	\param camera_handle camera handle instance
	\param name feature name
	\param value value to write
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_setFloatValue(struct CameraHandle *camera_handle, const char* name, double value);

/**
	\brief Get GenICam Float value
	\param camera_handle camera handle instance
	\param name feature name
	\param value_ptr pointer to value
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getFloatValue(struct CameraHandle *camera_handle, const char* name, double* value_ptr);

/**
	\brief Get GenICam Float limit values
	\param camera_handle camera handle instance
	\param name feature name
	\param min_value_ptr pointer to min value
	\param max_value_ptr pointer to max value
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getFloatValueLimits(struct CameraHandle *camera_handle, const char* name, double* min_value_ptr, double* max_value_ptr);

/**
	\brief Set GenICam String value
	\param camera_handle camera handle instance
	\param name feature name
	\param value value to write
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_setStringValue(struct CameraHandle *camera_handle, const char* name, const char* value);

/**
	\brief Get GenICam String value
	\param camera_handle camera handle instance
	\param name feature name
	\param value_ptr pointer to value
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getStringValue(struct CameraHandle *camera_handle, const char* name, const char** value_ptr);

/**
	\brief Set GenICam Enumeration value
	\param camera_handle camera handle instance
	\param name feature name
	\param value value to write
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_setEnumerationValue(struct CameraHandle *camera_handle, const char* name, const char* value);

/**
	\brief Get GenICam Enumeration value
	\param camera_handle camera handle instance
	\param name feature name
	\param value_ptr pointer to value
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getEnumerationValue(struct CameraHandle *camera_handle, const char* name, const char** value_ptr);

/**
	\brief Execute GenICam Command
	\param camera_handle camera handle instance
	\param name feature name
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_executeCommand(struct CameraHandle *camera_handle, const char* name);

/**
	\brief Check status of a GenICam Command
	\param camera_handle camera handle instance
	\param name feature name
	\return 0 = command in inactive, otherwise active
*/
int Gbe_isCommandDone(struct CameraHandle *camera_handle, const char* name);

/**
	\brief Reset stream channel of a camera connection
	\param camera_handle camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_resetStreamChannel(struct CameraHandle *camera_handle);

/**
	\brief Start image acquisition
	\param camera_handle camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_startAcquisition(struct CameraHandle *camera_handle);

/**
	\brief Stop image acquisition
	\param camera_handle camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_stopAcquisition(struct CameraHandle *camera_handle);

/**
	\brief Get the GenICam XML data for the camera
	\param camera_handle camera handle instance
	\param buffer user supplied buffer for the xml data (use 0 to get the needed buffer size!)
	\param buffer_size size of the user supplied buffer for the xml data (as return value: real size of the xml data)
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_getGenICamXML(struct CameraHandle *camera_handle, char *buffer, size_t *buffer_size);

/**
	\brief Get error description
	\param errcode error code (defined in gbe_error.h)
	\return error description as string
*/
const char* Gbe_getErrorDescription(int errcode);

/**
	\brief Enable/Disable GenICam checks (i.e. locked/available/implemented)
	\param camera_handle camera handle instance
	\param flag 0 = false, otherwise true
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
	\warning expert function
*/
int Gbe_enableChecks(struct CameraHandle *camera_handle, unsigned int flag);

/**
	\brief Lock GenICam transport layer parameters
	\param camera_handle camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
	\warning expert function
*/
int Gbe_lockTLParams(struct CameraHandle *camera_handle);

/**
	\brief Unlock GenICam transport layer parameters
	\param camera_handle camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
	\warning expert function
*/
int Gbe_unlockTLParams(struct CameraHandle *camera_handle);

/**
	\brief Invalidates the register cache for the camera
	\param camera_handle camera handle instance
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
	\warning expert function
*/
int Gbe_invalidateCache(struct CameraHandle *camera_handle);

/**
	\brief Read register content from camera
	\param camera_handle camera handle instance
	\param buffer user buffer to be filled
	\param address camera register space address
	\param length size of camera register space to be read
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
	\warning expert function
*/
int Gbe_registerReadFromCamera(struct CameraHandle *camera_handle, void* buffer, uint64_t address, size_t length);

/**
	\brief Write register content to camera
	\param camera_handle camera handle instance
	\param buffer user buffer to be written
	\param address camera register space address
	\param length size of camera register space to be written
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
	\warning expert function
*/
int Gbe_registerWriteToCamera(struct CameraHandle *camera_handle, const void* buffer, uint64_t address, size_t length);

/**
	\brief Set board property
	\param board_handle board handle instance
	\param propertyName name of the property
	\param propertyValuePtr pointer to the value of the property
	\param propertyType type of the property (see PROPERTY_TYPE_* defines)
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
	\warning expert function
*/
int Gbe_setBoardPropertyWithType(struct BoardHandle *board_handle, const char* propertyName, const void* propertyValuePtr, int propertyType);

/**
	\brief Get board property
	\param board_handle board handle instance
	\param propertyName name of the property
	\param propertyValuePtr pointer to the value of the property
	\param propertyType type of the property (see GBE_PROPERTY_TYPE_* defines)
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
	\warning expert function
*/
int Gbe_getBoardPropertyWithType(struct BoardHandle *board_handle, const char* propertyName, void* propertyValuePtr, int propertyType);

/**
	\brief Set camera property
	\param camera_handle camera handle instance
	\param propertyName name of the property
	\param propertyValuePtr pointer to the value of the property
	\param propertyType type of the property (see PROPERTY_TYPE_* defines)
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
	\warning expert function
*/
int Gbe_setCameraPropertyWithType(struct CameraHandle *camera_handle, const char* propertyName, const void* propertyValuePtr, int propertyType);

/**
	\brief Get camera property
	\param camera_handle camera handle instance
	\param propertyName name of the property
	\param propertyValuePtr pointer to the value of the property
	\param propertyType type of the property (see GBE_PROPERTY_TYPE_* defines)
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
	\warning expert function
*/
int Gbe_getCameraPropertyWithType(struct CameraHandle *camera_handle, const char* propertyName, void* propertyValuePtr, int propertyType);

#ifdef __cplusplus
/**
	\brief Get internal CNodeMapRef of camera connection 
	\param camera_handle camera handle instance
	\return Valid pointer or 0 in case of error
	\warning expert function
*/
GenApi::CNodeMapRef* Gbe_getCNodeMapRef(struct CameraHandle *camera_handle);
#endif

/**
	\brief Register callback function for board Gbe Events 
	\param board_handle board handle instance
	\param callback pointer to the user callback function (0 means deregister)
	\param userData user data pointer
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_registerBoardEventCallback(struct BoardHandle *board_handle, Gbe_BoardEventCallbackFunc_t callback, const void* userData);

/**
	\brief Register callback function for camera Gbe Events 
	\param camera_handle camera handle instance
	\param callback pointer to the user callback function (0 means deregister)
	\param userData user data pointer
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_getErrorDescription
*/
int Gbe_registerCameraEventCallback(struct CameraHandle *camera_handle, Gbe_CameraEventCallbackFunc_t callback, const void* userData);

/**
	\brief Save all current camera parameters to file (name/value pairs)
	\param cameraHandle camera handle instance
	\param paramFileName target file path
	\param flags reserved (currently unused)
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_LoadCameraParameters
	\see Gbe_getErrorDescription
*/
int Gbe_SaveCameraParameters(struct CameraHandle *cameraHandle, const char* paramFileName, uint32_t flags);

/**
	\brief Load a camera parameters file (name/value pairs) and applies value to camera
	\param cameraHandle camera handle instance
	\param paramFileName source file path
	\param flags GBE_FLAGS_NONE or GBE_FLAGS_STOPONERROR
	\return Gbe error code (defined in gbe_error.h)
	\see Gbe_SaveCameraParameters
	\see Gbe_getErrorDescription
*/
int Gbe_LoadCameraParameters(struct CameraHandle *cameraHandle, const char* paramFileName, uint32_t flags);

/**
	\brief Apply chunk data to the GenICam data model
	\param cameraHandle camera handle instance
	\param buffer pointer to the chunk data array
	\param length length of the chunk data array in bytes
	\param flags reserved (use 0)
	\return Gbe error code (defined in gbe_error.h)
*/
int Gbe_ApplyChunkData(struct CameraHandle *cameraHandle, const void* buffer, size_t length, uint32_t flags);

/**
	\brief Synchronize system wide Master/Slave initialization (call this function before Fg_Init(...))
	\param board board index
	\param slave request synchronization as master (0) or slave (0)
	\param flags reserved (use 0)
	\param timeout timeout in milliseconds
	\return Gbe error code (defined in gbe_error.h)
*/
int Gbe_RequestMasterSlaveInit(unsigned int board, int slave, unsigned int flags, unsigned int timeout);

/**
	\brief Notify slave processes about a completed Fg_Init(...) as master process
	\param board board index
	\return Gbe error code (defined in gbe_error.h)
*/
int Gbe_NotifyMasterInitialized(unsigned int board);

/* INTERNAL FUNCTIONS */

/**
	\brief Generic interface for calling an internal functions (not for public use!)
	\param id function id
	\param in input parameters
	\param out output parameters
	\return Gbe error code (defined in gbe_error.h)
*/
int Gbe_callInternalFunction(int id, const void* const in, void* const out);

/** Id for Siso Internal Function 'Reset PHY' */
#define SISO_INTERNAL_FUNCTION__RESET_PHY 1

#pragma pack(push)
#pragma pack(1)

/** Input Structure for Siso Internal Function 'Reset PHY' */
struct SifInputResetPhy {
	/** board id */
	int board;
	/** port mask 0x0 .. 0xf */
	int portMask;
};

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
