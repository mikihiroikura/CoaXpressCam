/**
 * siso_genicam_error.h
 *
 * Copyright (c) 2002-2017 Silicon Software GmbH, All Rights Reserved.
 *
 * \file siso_genicam_error.h
 *
 * \brief Error codes returned by the library siso_genicam
 *
 * \author Silicon Software GmbH
 */
 
 
#ifndef __SISO_GENICAM_ERROR_H__
#define __SISO_GENICAM_ERROR_H__

/** success */
#define SGC_OK                                         0    /**< The call of the function was successful*/

/** \brief general errors */
#define ERR_SGC_INVALID_CAMHANDLE                     -1000 /**< The sgc_XX function was called by using an invalid or uninitialized Camera handle*/
#define ERR_SGC_INVALID_BOARDHANDLE                   -1001 /**< The sgc_XX function was called by using an invalid or uninitialized board handle*/
#define ERR_SGC_FILENOTFOUND                          -1002 /**< The a file under the given filename does not exits*/
#define ERR_SGC_INVALIDFILENAME                       -1003 /**< An invalid filename has been passed to a Sgc_function, e.g. ""*/
#define ERR_SGC_FILEACCESS                            -1006 /**< A file cannot be accessed*/
#define ERR_SGC_NO_DATA                               -1008 /**< A function call was done using an empty buffer, e.g an empty file*/ 
#define ERR_SGC_INDEX_OUT_OF_RANGE                    -1009 /**< Value out of possible range*/
#define ERR_SGC_NO_INTERFACE                          -1010 /**< No interface */
#define ERR_SGC_GENICAM_NOT_SUPPORTED                 -1011 /**< Board type does not support GenICam interface*/
#define ERR_SGC_BUFFER_TOO_SMALL                      -1012 /**< Received data exceeds user data buffer*/
#define ERR_SGC_ACCESS_DENIED                         -1013 /**< Access denied*/
#define ERR_SGC_INVALID_DATA                          -1014 /**< The data is invalid*/
#define ERR_SGC_BUSY                                  -1015 /**< Operation is blocked by another process / thread*/
#define ERR_SGC_NO_EVENT_HANDLER                      -1016 /**< No event handler registered*/
#define ERR_SGC_LOADING_FILE             ERR_SGC_FILEACCESS /**< Kept for Compatibility to older versions*/



/** \brief GenICam errors*/
#define ERR_SGC_NO_XML_CONNECTION                     -1100 /**< XML Connection is not present / has not been made before*/
#define ERR_SGC_INVALID_PROPERTY                      -1101 /**< Tried to access an unknown property*/
#define ERR_SGC_INVALID_FEATURE                       -1102 /**< An invalid GenICam feature has been passed to the access functions*/
#define ERR_SGC_INVALID_COMMAND                       -1103 /**< An invalid GenICam command has been passed to the command functions*/
#define ERR_SGC_CANNOT_READ_WRITE_ONLY                -1104 /**< A SGC_GetValue function tried to read a write only register*/
#define ERR_SGC_CANNOT_WRITE_READ_ONLY                -1105 /**< A SGC_GetValue function tried to write a read only register*/
#define ERR_SGC_GENICAM_EXCEPTION                     -1198 /**< GenICam layer raised an exception, which was caught*/

/** \brief Connection errors */
#define ERR_SGC_INVALID_LINKSPEED                     -1200 /**< Tried to set an invalid CoaxPress link speed , only CXP_SPEED_XXX-constants are allowed*/
#define ERR_SGC_INVALID_LINKINDEX                     -1201 /**< Tried to access an invalid  link, which has not been defined previously*/
#define ERR_SGC_CANNOTCONNECT_BITRATE                 -1202 /**< Connecting at the bit rate wasn't possible */
#define ERR_SGC_UNABLE_TO_SET_CXP_LINKCONFIG          -1203 /**< Error Setting linkConfig Register to predefined combination of link count and speed */
#define ERR_SGC_MISSING_MASTERFOR_EXTENSIONLINK       -1204 /**< Predefined Extension link does not match to camera (MasterHostID does not match between Master & extension)*/
#define ERR_SGC_STREAMPAKETSIZE_NOT_ACCEPTED          -1205 /**< The camera does not accept the given stream packet size*/
#define ERR_SGC_TOPOLOGY_MISMATCH                     -1206 /**< The given topology between Applet and camera does not match*/
#define ERR_SGC_WAITFORLINK_TIMEOUT                   -1207 /**< Timeout when establishing link */
#define ERR_SGC_INVALID_PORT                          -1208 /**< Function was called by using an invalid port mask or port index*/
#define ERR_SGC_NO_REMOTE_MASTER                      -1209 /**< A Master connect didn't take place at a second frame grabber*/
#define ERR_SGC_NO_CAMERA_FOUND                       -1210 /**< Automatic discovery didn't find any camera*/
#define ERR_SGC_NOT_CONNECTED                         -1211 /**< No physical connection */
#define ERR_SGC_NO_CAMERA_MASTER                      -1212 /**< No master was detected or set for the camera */
#define ERR_SGC_CAMERA_DISCOVERY_INCOMPLETE           -1213 /**< The camera discovery info is incomplete */

/** \brief fatal errors*/
#define ERR_SGC_PARAMTYPE_MISMATCH                    -1990 /**< Get / set functions are called by using an invalid data type*/
#define ERR_SGC_INVALID_ARG                           -1991 /**< A function call was done using an invalid data type*/ 
#define ERR_SGC_INTERNAL_TIMEOUT                      -1997 /**< An internal timeout while waiting on something occurred*/ 
#define ERR_SGC_INTERNAL                              -1998 /**< An internal error occurred */
#define ERR_SGC_UNEXPECTED                            -1999 /**< An exception occurred internally*/
#define ERR_SGC_NOTIMPLEMENTED                        -2000 /**< Function is currently not present*/

/** \brief General errors*/
#define ERR_SGC_BOARD_INITIALIZATION_ERROR            -40018 /**< Board initialization error */
#define ERR_SGC_INVALID_LIB                           -40019 /**< Incompatible underlaying library */
#define ERR_SGC_LIB_NOT_LOADED                        -40020 /**< underlaying library not loaded */

/** \brief CLHS_Protocol errors */
#define ERR_SGC_CLHS_XCORE_COMMAND_TIMEOUT            -1501 /**< No command word available in the X-Core (CLHS).*/
#define ERR_SGC_CLHS_TRANSMITBUFFER_FULL              -1502 /**< An transmission buffer was full, when trying to send data (CLHS)*/ 
#define ERR_SGC_CLHS_RECEIVE_ERROR                    -1503 /**< XCore failed receiving a message 10 times (CLHS)*/ 
#define ERR_SGC_CLHS_XCORE_TXBUSY_TIMEOUT             -1504 /**< XCore TX Busy time out (CLHS).*/
#define ERR_SGC_CLHS_SENDING_ERROR                    -1505 /**< XCore failed Sending a message 10 times (CLHS) (no Acknowledge received 10 times)*/ 
#define ERR_SGC_CLHS_XCORE_TXBUSY                     -1506 /**< XCore busy without error */ 

/** \brief CLHS Discovery errors */
#define ERR_SGC_CLHS_DISCOVERY_LLDEVICEID_WRITEACCESS_FAILED -1506 /**< LLDeviceID Rejected by camera*/ 
#define ERR_SGC_CLHS_DISCOVERY_LLDEVICEID_NOT_ACCEPTED       -1507 /**< LLDeviceID not readable from camera*/ 


/** \brief GenCP Protocol errors */
/** Errors according to GenCP Chapter 3.1.4*/
#define ERR_SGC_GENCP_CRC_ERROR                     -0x0641 /**< Corrupt CRC at command or acknowledge packet*/
#define ERR_SGC_GENCP_TIMEOUT                       -0x0642 /**< A timeout waiting for an acknowledge*/
#define ERR_SGC_GENCP_INVALID_PACKET                -0x0643 /**< An invalid packet (too short or timeout for completion */
#define ERR_SGC_GENCP_ACKNOWLEDGE_MISSING           -0x0644 /**< GenCP acknowledge is missing */
#define ERR_SGC_GENCP_UNKNONW_GENCP_ERROR           -0x0645 /**< unknown error code received from device / camera */
#define ERR_SGC_GENCP_INSUFFICIENT_DATA             -0x0646 /**< Too less data received at a read command*/
#define ERR_SGC_GENCP_NO_DATA                       -0x0647 /**< No data received at a read command*/
#define ERR_SGC_GENCP_PENDING_ACK                   -0x0648 /**< Pending Acknowledge received*/
#define ERR_SGC_GENCP_INVALID_ACK                   -0x0649 /**< Acknowledge of another ReqId received, while waiting on finalizing pending Ack.*/
#define ERR_SGC_GENCP_UNKNOWPROTOCOLLERROR          -0x064a /**< Unsupported error */
#define ERR_SGC_GENCP_UNEXPECTED_ACK                -0x064b /**< Invalid packet order, unexpected GenCP acknowledge received*/

/** Errors according to GenCP Chapter 4.3.2.1*/
#define ERR_SGC_GENCP_NATIVE_NOT_IMPLEMENTED        -0x8001 /**< Command not implemented in the device */
#define ERR_SGC_GENCP_NATIVE_INVALID_PARAMETER      -0x8002 /**< At least one of the command parameter of CD or SCD is invalid or out of range */
#define ERR_SGC_GENCP_NATIVE_INVALID_ADDRESS        -0x8003 /**< Attempt to access a not existing register address */
#define ERR_SGC_GENCP_NATIVE_WRITE_PROTECT          -0x8004 /**< Attempt to write to a read only register */
#define ERR_SGC_GENCP_NATIVE_BAD_ALIGNMENT          -0x8005 /**< Attempt to access register with an address which is not aligned according to the underlying technology */
#define ERR_SGC_GENCP_NATIVE_ACCESS_DENIED          -0x8006 /**< Attempt to read a non readable or write a non writable register address */
#define ERR_SGC_GENCP_NATIVE_GENCP_BUSY             -0x8007 /**< The command receiver is currently busy */
#define ERR_SGC_GENCP_NATIVE_MSG_TIMEOUT            -0x800B /**< Timeout waiting for an acknowledge */
#define ERR_SGC_GENCP_NATIVE_INVALID_HEADER         -0x800E /**< The header of the received command is invalid. This includes CCD and SCD fields but not the command payload */
#define ERR_SGC_GENCP_NATIVE_WRONG_CONFIG           -0x800F /**< The current receiver configuration does not allow the execution of the sent command */
#define ERR_SGC_GENCP_NATIVE_ERROR                  -0x8FFF /**< Generic error */



/** \brief CXP Protocol errors */

/** General CoaxPress errors */
#define ERR_SGC_CXP_NATIVE_TIMEOUT                        -0x100 /**< Acknowledgment timeout */

/** Errors according to CoaxPress 1.1 Chapter 8.6.3*/
#define ERR_SGC_CXP_NATIVE_INVALID_ADDRESS                 -0x40 /**< Invalid address */
#define ERR_SGC_CXP_NATIVE_INVALID_DATA_FOR_ADDRESS        -0x41 /**< Invalid data for the address */
#define ERR_SGC_CXP_NATIVE_INVALID_CONTROL_OPERATION_CODE  -0x42 /**< Invalid control operation code */
#define ERR_SGC_CXP_NATIVE_WRITE_ACCESS_DENIED             -0x43 /**< Write attempted to a read-only address */
#define ERR_SGC_CXP_NATIVE_READ_ACCESS_DENIED              -0x44 /**< Read attempted from a write-only address */
#define ERR_SGC_CXP_NATIVE_SIZE_FIELD_TOO_LARGE            -0x45 /**< Size field too large – command message (write) or acknowledgment message (read) would exceed packet size limit */
#define ERR_SGC_CXP_NATIVE_INCORRECT_SIZE_RECEIVED         -0x46 /**< Incorrect size received, Message size is inconsistent with message size indication */
#define ERR_SGC_CXP_NATIVE_MALFORMED_PACKET                -0x47 /**< Malformed packet */
#define ERR_SGC_CXP_NATIVE_CRC_ERROR_IN_COMMAND            -0x80 /**< Failed CRC test in last received command */

#endif
