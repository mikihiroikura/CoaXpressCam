/**
 * siso_auxport.h
 *
 * Copyright (c) 2002-2017 Silicon Software GmbH, All Rights Reserved.
 *
 *
 * \file siso_auxport.h
 *
 * \brief Header file for all exported functions for access to 
  * auxilliary ports of Silicon Software hardware.
 *
 * \author Silicon Software GmbH
 */
#pragma once
#ifndef SISO_AUX_PORT_H
#define SISO_AUX_PORT_H

#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif


/** Auxiliary port types */
enum SisoAuxPortType_Enum {
    SISO_AUX_PORT_ANY,  /**< Port type unspecified (all ports) */
    SISO_AUX_PORT_JTAG, /**< JTAG ports */
    SISO_AUX_PORT_I2C,  /**< I2C ports */
    SISO_AUX_PORT_UIQ,  /**< UIQ ports */
    SISO_AUX_PORT_SPI   /**< SPI ports */
};

typedef enum SisoAuxPortType_Enum SisoAuxPortType;

/** @name Port enumeration
 *
 * These functions can be used to enumerate the ports on a device.
 */
/*@{*/

/** Get number of available ports of given type
 *
 * \remarks Use SISO_AUX_PORT_ANY to get the total number of auxiliary ports on a given board.
 *
 * \param board_id Number of Siso board
 * \param port_type Port type
 * \returns Number of ports on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_BOARD_ID No Siso board found for the specified id
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_TYPE The value for port type parameter is invalid
 */
int SisoAuxPortGetNumberOfPorts(unsigned int board_id, SisoAuxPortType port_type);

/** Get number of first port of given type
 *
 * \remarks Ports are grouped by type and numbered consecutively. Together with SisoAuxPortGetNumberOfPorts()
 *        the range of numbers for a given port type can be easily determined.
 *        For example, if for a given board and SISO_AUX_PORT_I2C SisoAuxPortGetFirstNumberOfType() returns 2,
 *        and SisoAuxPortGetNumberOfPorts() returns 3, then the available I2C ports will be 2, 3 and 4.
 *
 * \param board_id Number of Siso board
 * \param port_type Port type
 * \returns Number of first port on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_BOARD_ID No Siso board found for the specified id
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_TYPE The value for port type parameter is invalid
 */
int SisoAuxPortGetFirstNumberOfType(unsigned int board_id, SisoAuxPortType port_type);

/** Get port type
 *
 * \remarks An alternative approach to opening ports of a given type is to enumerate all ports,
 *        get the type and open the port accordingly.
 *
 * \param board_id Number of Siso board
 * \param port_num Number of port
 * \param type_ptr Pointer to port type variable
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_BOARD_ID No Siso board found for the specified id
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_NUMBER No port found for the specified number
 * \retval #SISO_AUX_PORT_ERROR_INVALID_POINTER Either NULL was specified for the type pointer, or memory could not be accessed
 */
int SisoAuxPortGetType(unsigned int board_id, unsigned int port_num, SisoAuxPortType * type_ptr);

/*@}*/



/** Opaque handle for auxiliary port */
typedef struct SisoAuxPort_Struct * SisoAuxPort;

/** @name Port initialization and release
 *
 * These functions will initialize the ports on the requested device.
 * They need to be called before any other operation on the port can be
 * performed.
 */
/*@{*/

/** Initialise port handle
 *
 * \remarks Before a port can be opened and used, a handle needs to be created and initialised using SisoAuxPortInit().
 *        After using the port, all handles must be released using SisoAuxPortFree() before the program terminates.
 *
 * \param board_id Number of Siso board
 * \param port_num Number of port
 * \param type Port type
 * \param handle_ptr Pointer to port handle variable
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_BOARD_ID No Siso board found for the specified id
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_NUMBER No port found for the specified number
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_TYPE The value for port type parameter was invalid
 * \retval #SISO_AUX_PORT_ERROR_INVALID_POINTER Either NULL was specified for the type pointer, or memory could not be accessed
 * \retval #SISO_AUX_PORT_ERROR_OUT_OF_MEMORY Not enough memory to allocate handle
 */
int SisoAuxPortInit(unsigned int board_id, unsigned int port_num, SisoAuxPortType type, SisoAuxPort * handle_ptr);

/** Open port
 *
 * \remarks Before a port can be used for communication, it needs to be opened using SisoAuxPortOpen().
 *        After using a port for communication, it should be closed using SisoAuxPortClose().
 *        While any number of handles can be created for the same port by different threads or processes,
 *        only one handle should be used to open a port at any time. The Siso Runtime ensures that only one
 *        handle within a process can open the port. The user has to ensure that a handle is not used across
 *        threads. If a handle is shared by two or more threads, the user is responsible for proper
 *        synchronization. The same applies to opening and using ports from different processes.
 *
 * \param handle Port handle
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_BUSY The port is already open by another thread or process
 */
int SisoAuxPortOpen(SisoAuxPort handle);

/** Close auxiliary port
 *
 * \remarks After finishing communication on a port, it should be closed using SisoAuxPortClose().
 *
 * \param handle Port handle
 * \returns 0 on success, negative value on error
 * 
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 */
int SisoAuxPortClose(SisoAuxPort handle);

/** Free auxiliary port handle
 *
 * \remarks All handles that were created by SisoAuxPortInit() must be released using SisoAuxPortFree()
 *        before the program terminates. After calling SisoAuxPortFree() on a handle, the handle
 *        becomes invalid and must not be used in any function calls.
 *
 * \param handle Port handle
 * \returns 0 on success, negative value on error
 * 
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 */
int SisoAuxPortFree(SisoAuxPort handle);

/*@}*/



/** Property type */
enum SisoAuxPortPropertyType_Enum {
    SISO_AUX_PORT_PROPERTY_TYPE_INT,    /**< Integer value */
    SISO_AUX_PORT_PROPERTY_TYPE_FLOAT,  /**< Float value */
    SISO_AUX_PORT_PROPERTY_TYPE_STRING, /**< String (supply a character buffer) */
    SISO_AUX_PORT_PROPERTY_TYPE_BINARY  /**< Binary (further defined) */
};

typedef SisoAuxPortPropertyType_Enum SisoAuxPortPropertyType;

/** @name Port properties
 *
 * These properties are defined for ports
 */
/*@{*/

/* Integer properties */
#define SISO_AUX_PORT_INT_PROPERTY_FREQUENCY "Frequency:Int" /**< The port frequency as an integer value */

/* Float properties */
#define SISO_AUX_PORT_FLOAT_PROPERTY_FREQUENCY "Frequency:Float" /**< The port frequency as a float value */

/* String properties */
#define SISO_AUX_PORT_STRING_PROPERTY_NAME "Name:Str" /**< The symbolic name for the port as a string */
#define SISO_AUX_PORT_STRING_PROPERTY_TYPE "Type:Str" /**< The type of the port as a string */

/* JTAG port integer properties */
#define SISO_JTAG_PORT_INT_PROPERTY_NUM_TAPS "JTAGNumTaps:Int" /**< The number of TAPs in a JTAG chain as an integer value */
#define SISO_JTAG_PORT_INT_PROPERTY_STATE "JTAGState:Int" /**< The current state of the JTAG port as an integer value */

/* JTAG port binary properties */
#define SISO_JTAG_PORT_BINARY_PROPERTY_CHAIN "JTAGChain:Bin" /**< The JTAG chain (supply a SisoJTAGChain_Struct of the proper size) */

/* UIQ port properties */
#define SISO_UIQ_PORT_INT_PROPERTY_TRANSMISSION_MODE "UiqTransmissionMode:Int"  /**< UIQ transmission mode (number of bytes per transaction) */
#define SISO_UIQ_PORT_INT_PROPERTY_SPI_MODE "UiqSpiMode:Int"                    /**< UIQ SPI mode (0, 1, 2 or 3) */
#define SISO_UIQ_PORT_INT_PROPERTY_NUM_WORDS_AVAILABLE "UiqNumWords:Int"        /**< Available number of UIQ words */

/*@}*/

/** @name JTAG port state property values */
/*@{*/

#define SISO_JTAG_PORT_STATE_RESET    0 /**< Reset the JTAG state machine */
#define SISO_JTAG_PORT_STATE_IDLE     1 /**< JTAG idle state */
#define SISO_JTAG_PORT_STATE_SHIFT_DR 2 /**< Shift data to data register */
#define SISO_JTAG_PORT_STATE_SHIFT_IR 3 /**< Shift data to instruction register */

/*@}*/

/** @name JTAG device id values */
/*@{*/

/* JTAG invalid device id */
#define SISO_JTAG_PORT_DEVICE_ID_NONE      0  /**< The device is not expected to have an id */
#define SISO_JTAG_PORT_DEVICE_ID_INVALID (-1) /**< The device id is not known */

/*@}*/

/** JTAG TAP descriptor */
struct SisoJTAGTAP_Struct {
    unsigned int ir_length;     /**< Length of IR register (required) */
    unsigned int device_id;     /**< Device ID (verify or auto-detect) */
};

/** JTAG chain descriptor (must be allocated and initialised by user) 
 *
 * \remarks If device_id in a TAP descriptor is set to SISO_JTAG_PORT_DEVICE_ID_INVALID, the device id will be read and filled in automatically.
 *        If device_id is set to either SISO_JTAG_PORT_DEVICE_ID_NONE or a valid device id, the device id will be verified.
 *        An error will be returned if a device id does not match, or if SISO_JTAG_PORT_DEVICE_ID_INVALID was set for a device without IDCODES support.
 *        The JTAG chain has to be set by the user before using the device level transmission API.
 *        To allocate a JTAG chain descriptor for N TAPs, use:
 *            SisoJTAGChain_Struct * chain = (SisoJTAGChain_Struct *) malloc(sizeof(SisoJTAGChain_Struct) + (N-1)*sizeof(SisoJTAGTAP_Struct));
 *
 */
struct SisoJTAGChain_Struct {
    size_t num_taps;            /**< Number of TAP descriptor entries in chain descriptor */
    SisoJTAGTAP_Struct tap[1];  /**< TAP descriptor entries */
};

/** @name Port property interface
 *
 * These functions can be used to get and set certain operational parameters of a port.
 */
/*@{*/

/** Get port property
 *
 * \remarks Several properties of a port can be retrieved, such as the frequency at which the port operates,
 *        a symbolic name for the port or the port type. Depending on the port type, port type specific
 *        properties may be available for retrieval.
 *
 * \param handle Port handle
 * \param property Name of the property to get
 * \param type Type of the value pointer
 * \param value Pointer to value variable
 * \param size Size of the value variable
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PROPERTY The name of the property to get/set was invalid, or not available for the type of the port the handle refers to
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PROPERTY_TYPE The type of the value pointer does not match the property value
 * \retval #SISO_AUX_PORT_ERROR_INVALID_POINTER Either NULL was specified for the value pointer, or memory could not be accessed
 * \retval #SISO_AUX_PORT_ERROR_INVALID_SIZE The size does not match the type of the value pointer
 */
int SisoAuxPortGetProperty(SisoAuxPort handle, const char * property, SisoAuxPortPropertyType type, void * value, unsigned int size);

/** Set port property
 *
 * \remarks Several properties of a port can be set, such as the frequency at which the port operates.
 *        Depending on the port type, port type specific properties may be available for setting.
 *        Port properties can only be changed when the port is not open.
 *        When setting the frequency of a port, the actual frequency may not be the same as requested,
 *        but is guaranteed to be lower or equal the requested frequency.
 *
 * \param handle Port handle
 * \param property Name of the property to set
 * \param type Type of the value pointer
 * \param value Pointer to value variable
 * \param size Size of the value variable
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PROPERTY The name of the property to get/set was invalid, or not available for the type of the port the handle refers to
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PROPERTY_TYPE The type of the value pointer does not match the property value
 * \retval #SISO_AUX_PORT_ERROR_INVALID_POINTER Either NULL was specified for the value pointer, or memory could not be accessed
 * \retval #SISO_AUX_PORT_ERROR_INVALID_SIZE The size does not match the type of the value pointer
 * \retval #SISO_AUX_PORT_ERROR_READ_ONLY_PROPERTY The property can only be retrieved, not set
 * \retval #SISO_AUX_PORT_ERROR_FREQUENCY_TOO_LOW The requested frequency was too low
 */
int SisoAuxPortSetProperty(SisoAuxPort handle, const char * property, SisoAuxPortPropertyType type, void * value, unsigned int size);

/*@}*/



/** @name Transmission flags
 *
 * These flags control data transmission on a port.
 */
/*@{*/

/* General transmission flags */
#define SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BYTES (0x1) /**< transfer length is number of bytes to read or write */
#define SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BITS  (0x2) /**< transfer length is number of bits to read or write */

/* JTAG port transmission flags */
#define SISO_JTAG_PORT_RWFLAGS_USE_DEVICE_ID           0  /**< device_id field in API is JTAG device id */
#define SISO_JTAG_PORT_RWFLAGS_USE_TAP_NUMBER      (0x10) /**< device_id field in API is the number of the TAP in the chain descriptor */
#define SISO_JTAG_PORT_RWFLAGS_WAIT_ON_READ            0  /**< Wait on read if no write has been issued, or not enough data is available */
#define SISO_JTAG_PORT_RWFLAGS_SEND_ZEROES_ON_READ (0x20) /**< Send 0 bits on read if not enough data is available */
#define SISO_JTAG_PORT_RWFLAGS_RAW_SEND_ZEROES_ON_OTHER SISO_JTAG_PORT_RWFLAGS_SEND_ZEROES_ON_READ /**< Send 0 bits on raw write for shift register without data */
#define SISO_JTAG_PORT_RWFLAGS_SEND_ONES_ON_READ   (0x40) /**< Send 1 bits on read if not enough data is available */
#define SISO_JTAG_PORT_RWFLAGS_RAW_SEND_ONES_ON_OTHER SISO_JTAG_PORT_RWFLAGS_SEND_ONES_ON_READ /**< Send 1 bits on raw write for shift register without data */
#define SISO_JTAG_PORT_RWFLAGS_READ_MODE_MASK      (0x60) /**< Read mode mask */
#define SISO_JTAG_PORT_RWFLAGS_RAW_MODE            (0x80) /**< Write data to JTAG shift registers */
#define SISO_JTAG_PORT_RWFLAGS_STATE_IDLE         (0x100) /**< Transmit in IDLE state */
#define SISO_JTAG_PORT_RWFLAGS_STATE_SHIFT_DR     (0x200) /**< Transmit in SHIFT-DR state */
#define SISO_JTAG_PORT_RWFLAGS_STATE_SHIFT_IR     (0x300) /**< Transmit in SHIFT_IR state */
#define SISO_JTAG_PORT_RWFLAGS_STATE_MASK         (0x300) /**< Transmit state mask */
#define SISO_JTAG_PORT_RWFLAGS_MSB_FIRST                0 /**< Send and receive data starting with most significant bit */
#define SISO_JTAG_PORT_RWFLAGS_LSB_FIRST          (0x400) /**< Send and receive data starting with least significant bit */
#define SISO_JTAG_PORT_RWFLAGS_RAW_WRITE_TDI_DATA       0 /**< Write data to the TDI shift register */
#define SISO_JTAG_PORT_RWFLAGS_RAW_WRITE_TMS_DATA (0x800) /**< Write data to the TMS shift register */

/* I2C port transmission flags */
#define SISO_I2C_PORT_RWFLAGS_SEND_START               0  /**< Send START condition (and device id when using device level transmission API) */
#define SISO_I2C_PORT_RWFLAGS_DONT_SEND_START    (0x1000) /**< Don't send START condition (no device id will be sent when using device level transmission API) */
#define SISO_I2C_PORT_RWFLAGS_SEND_STOP                0  /**< Send STOP condition after transfer */
#define SISO_I2C_PORT_RWFLAGS_DONT_SEND_STOP     (0x2000) /**< Don't send STOP condition after transfer (don't terminate message) */
#define SISO_I2C_PORT_RWFLAGS_NAK_LAST_BYTE_READ       0  /**< Send NAK after last byte read */
#define SISO_I2C_PORT_RWFLAGS_ACK_LAST_BYTE_READ (0x4000) /**< Send ACK after last byte read (don't terminate transfer) */
#define SISO_I2C_PORT_RWFLAGS_ACK_POLLING        (0x8000) /**< Poll for ACK on first byte written */

/*@}*/

/** Flush type */
enum SisoAuxPortFlushType_Enum {
    SISO_AUX_PORT_FLUSH_ALL,
    SISO_AUX_PORT_FLUSH_INPUT,
    SISO_AUX_PORT_FLUSH_OUTPUT
};

typedef enum SisoAuxPortFlushType_Enum SisoAuxPortFlushType;

/** @name Port communication interface
 *
 * These functions can be used to send and receive data on a port.
 */
/*@{*/

/** Send raw data on auxiliary port
 *
 * \remarks Any device selection must be done by the user in data.
 *        The transfer length is either in bytes or bits, depending on whether
 *        SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BYTES or SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BITS is set (see flags).
 *        (At this stage of implementation, this is the only way to communicate on a JTAG port.)
 *
 * \param handle Port handle
 * \param data Data buffer
 * \param size Size of data buffer in bytes
 * \param length_ptr Pointer to transfer length variable; requested length on input, actual transfer length on output
 * \param flags Write flags
 * \param timeout Timeout in milliseconds (0 means return immediately, -1 means wait without timeout)
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_PORT_NOT_OPEN The port has not been opened yet
 * \retval #SISO_AUX_PORT_ERROR_INVALID_POINTER Either NULL was specified for the data pointer, or memory could not be accessed
 * \retval #SISO_AUX_PORT_ERROR_INVALID_SIZE The size was either zero or too big for the type of port the handle refers to
 * \retval #SISO_AUX_PORT_ERROR_IO_FAILURE A general input/output error occurred
 * \retval #SISO_AUX_PORT_ERROR_TIMEOUT The operation could not be completed within the time specified
 * \retval #SISO_AUX_PORT_ERROR_OUT_OF_MEMORY Not enough memory to allocate internal buffers
 * \retval #SISO_AUX_PORT_ERROR_INVALID_RWFLAGS The flags passed were invalid for the type of port the handle refers to, or not allowed in the combination given, or plain invalid
 * \retval #SISO_I2C_PORT_ERROR_ARBITRATION_LOST Arbitration on a multi-master I2C bus was lost
 * \retval #SISO_I2C_PORT_ERROR_ACK_NAK_MISMATCH Device did not answer or rejected the request
 */
int SisoAuxPortWrite(SisoAuxPort handle, const void * data, unsigned int size, unsigned int * length_ptr, unsigned int flags, unsigned int timeout);

/** Receive raw data on auxiliary port
 *
 * \remarks The transfer length is either in bytes or bits, depending on whether
 *        SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BYTES or SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BITS is set (see flags).
 *        (At this stage of implementation, this is the only way to communicate on a JTAG port.)
 *
 * \param handle Port handle
 * \param data Data buffer
 * \param size Size of data buffer in bytes
 * \param length_ptr Pointer to transfer length variable; requested length on input, actual transfer length on output
 * \param flags Read flags
 * \param timeout Timeout in milliseconds (0 means return immediately, -1 means wait without timeout)
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_PORT_NOT_OPEN The port has not been opened yet
 * \retval #SISO_AUX_PORT_ERROR_INVALID_POINTER Either NULL was specified for the data pointer, or memory could not be accessed
 * \retval #SISO_AUX_PORT_ERROR_INVALID_SIZE The size was either zero or too big for the type of port the handle refers to
 * \retval #SISO_AUX_PORT_ERROR_IO_FAILURE A general input/output error occurred
 * \retval #SISO_AUX_PORT_ERROR_TIMEOUT The operation could not be completed within the time specified
 * \retval #SISO_AUX_PORT_ERROR_INVALID_RWFLAGS The flags passed were invalid for the type of port the handle refers to, or not allowed in the combination given, or plain invalid
 * \retval #SISO_I2C_PORT_ERROR_ARBITRATION_LOST Arbitration on a multi-master I2C bus was lost
 * \retval #SISO_I2C_PORT_ERROR_ACK_NAK_MISMATCH Device did not answer or rejected the request
 */
int SisoAuxPortRead(SisoAuxPort handle, void * data, unsigned int size, unsigned int * length_ptr, unsigned int flags, unsigned int timeout);

/** Flush data on auxiliary port
 *
 * \remarks Possible error values:
 *        SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE
 *        SISO_AUX_PORT_ERROR_IO_FAILURE
 *        SISO_AUX_PORT_ERROR_TIMEOUT
 *        SISO_AUX_PORT_ERROR_INVALID_FLUSH_TYPE
 *
 * \param handle Port handle
 * \param flush buffers
 * \param timeout Timeout in milliseconds (0 means return immediately, -1 means wait without timeout)
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_PORT_NOT_OPEN The port has not been opened yet
 * \retval #SISO_AUX_PORT_ERROR_IO_FAILURE A general input/output error occurred
 * \retval #SISO_AUX_PORT_ERROR_TIMEOUT The operation could not be completed within the time specified
 * \retval #SISO_AUX_PORT_ERROR_INVALID_FLUSH_TYPE The value for flush type was invalid
 */
int SisoAuxPortFlush(SisoAuxPort handle, SisoAuxPortFlushType flush, unsigned int timeout);

/** Begin transfer block
 *
 * The AuxPort API is optimized for block transfers. When using the API to do many small transfers, the stability of the system can
 * suffer. With the function pair SisoAuxPortBeginTransferBlock() and SisoAuxPortEndTransferBlock() such transfers can be made more
 * efficient and secure.
 * Note, however, that a transfer block is exclusive across the whole system with regard to the board. The transfer block will
 * block access to the board for many other APIs not related to SisoAuxPort (for example, but not limited to, scanning and
 * initializing boards).
 * As a general rule, don't hold a transfer block for much more than a second, unless you know that no other API needs access to
 * the board while holding the transfer block.
 *
 * \remarks Possible error values:
 *        SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE
 *        SISO_AUX_PORT_ERROR_PORT_NOT_OPEN
 *        SISO_AUX_PORT_ERROR_IO_FAILURE
 *        SISO_AUX_PORT_ERROR_TIMEOUT
 *
 * \param handle Port handle
 * \param timeout Timeout in milliseconds (0 means return immediately, -1 means wait without timeout)
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_PORT_NOT_OPEN The port has not been opened yet
 * \retval #SISO_AUX_PORT_ERROR_IO_FAILURE A general input/output error occurred
 * \retval #SISO_AUX_PORT_ERROR_TIMEOUT The operation could not be completed within the time specified
 */
int SisoAuxPortBeginTransferBlock(SisoAuxPort handle, unsigned int timeout);

/** End transfer block
 *
 * \remarks Possible error values:
 *        SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE
 *        SISO_AUX_PORT_ERROR_PORT_NOT_OPEN
 *        SISO_AUX_PORT_ERROR_IO_FAILURE
 *
 * \param handle Port handle
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_PORT_NOT_OPEN The port has not been opened yet
 * \retval #SISO_AUX_PORT_ERROR_IO_FAILURE A general input/output error occurred
 */
int SisoAuxPortEndTransferBlock(SisoAuxPort handle);



/** Send data to device
 *
 * \remarks The transfer length is either in bytes or bits, depending on whether
 *        SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BYTES or SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BITS is set (see flags).
 *        For JTAG port, JTAG chain must be set before calling.
 *        (At this stage of implementation, JTAG chains and thus high level JTAG communication is not available.)
 *
 * \param handle Port handle
 * \param device_id Device id, or TAP number
 * \param data Data buffer
 * \param size Size of data buffer in bytes
 * \param length_ptr Pointer to transfer length variable; requested length on input, actual transfer length on output
 * \param flags Write flags
 * \param timeout Timeout in milliseconds (0 means return immediately, -1 means wait without timeout)
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_PORT_NOT_OPEN The port has not been opened yet
 * \retval #SISO_AUX_PORT_ERROR_INVALID_POINTER Either NULL was specified for the data pointer, or memory could not be accessed
 * \retval #SISO_AUX_PORT_ERROR_INVALID_SIZE The size was either zero or too big for the type of port the handle refers to
 * \retval #SISO_AUX_PORT_ERROR_IO_FAILURE A general input/output error occurred
 * \retval #SISO_AUX_PORT_ERROR_TIMEOUT The operation could not be completed within the time specified
 * \retval #SISO_AUX_PORT_ERROR_OUT_OF_MEMORY Not enough memory to allocate internal buffers
 * \retval #SISO_AUX_PORT_ERROR_INVALID_RWFLAGS The flags passed were invalid for the type of port the handle refers to, or not allowed in the combination given, or plain invalid
 * \retval #SISO_JTAG_PORT_ERROR_NO_CHAIN_SET No JTAG chain was set
 * \retval #SISO_I2C_PORT_ERROR_ARBITRATION_LOST Arbitration on a multi-master I2C bus was lost
 * \retval #SISO_I2C_PORT_ERROR_ACK_NAK_MISMATCH Device did not answer or rejected the request
 */
int SisoAuxPortSend(SisoAuxPort handle, unsigned int device_id, const void * data, unsigned int size, unsigned int * length_ptr, unsigned int flags, unsigned int timeout);

/** Receive data from device on auxiliary port
 *
 * \remarks The transfer length is either in bytes or bits, depending on whether
 *        SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BYTES or SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BITS is set (see flags).
 *        For JTAG port, JTAG chain must be set before calling.
 *        (At this stage of implementation, JTAG chains and thus high level JTAG communication is not available.)
 *
 * \param handle Port handle
 * \param device_id Device id, or TAP number
 * \param data Data buffer
 * \param size Size of data buffer in bytes
 * \param length_ptr Pointer to transfer length variable; requested length on input, actual transfer length on output
 * \param flags Read flags
 * \param timeout Timeout in milliseconds (0 means return immediately, -1 means wait without timeout)
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_PORT_NOT_OPEN The port has not been opened yet
 * \retval #SISO_AUX_PORT_ERROR_INVALID_POINTER Either NULL was specified for the data pointer, or memory could not be accessed
 * \retval #SISO_AUX_PORT_ERROR_INVALID_SIZE The size was either zero or too big for the type of port the handle refers to
 * \retval #SISO_AUX_PORT_ERROR_IO_FAILURE A general input/output error occurred
 * \retval #SISO_AUX_PORT_ERROR_TIMEOUT The operation could not be completed within the time specified
 * \retval #SISO_AUX_PORT_ERROR_INVALID_RWFLAGS The flags passed were invalid for the type of port the handle refers to, or not allowed in the combination given, or plain invalid
 * \retval #SISO_JTAG_PORT_ERROR_NO_CHAIN_SET No JTAG chain was set
 * \retval #SISO_I2C_PORT_ERROR_ARBITRATION_LOST Arbitration on a multi-master I2C bus was lost
 * \retval #SISO_I2C_PORT_ERROR_ACK_NAK_MISMATCH Device did not answer or rejected the request
 */
int SisoAuxPortReceive(SisoAuxPort handle, unsigned int device_id, void * data, unsigned int size, unsigned int * length_ptr, unsigned int flags, unsigned int timeout);

/** Send and receive data to/from device on auxiliary port
 *
 * \remarks The transfer length is either in bytes or bits, depending on whether
 *        SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BYTES or SISO_AUX_PORT_RWFLAGS_LENGTH_IN_BITS is set (see flags).
 *        For JTAG port, JTAG chain must be set before calling.
 *        (At this stage of implementation, JTAG chains and thus high level JTAG communication is not available.)
 *
 * \param handle Port handle
 * \param device_id Device id, or TAP number
 * \param data_out Send data buffer
 * \param size_out Size of send data buffer in bytes
 * \param length_out_ptr Pointer to send transfer length variable; requested length on input, actual transfer length on output
 * \param data_in Receive data buffer
 * \param size_in Size of receive data buffer in bytes
 * \param length_in_ptr Pointer to receive transfer length variable; requested length on input, actual transfer length on output
 * \param flags Read/Write flags
 * \param timeout Timeout in milliseconds (0 means return immediately, -1 means wait without timeout)
 * \returns 0 on success, negative value on error
 *
 * \retval #SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE Either NULL was specified for the handle, or the handle was already released, or invalid
 * \retval #SISO_AUX_PORT_ERROR_PORT_NOT_OPEN The port has not been opened yet
 * \retval #SISO_AUX_PORT_ERROR_INVALID_POINTER Either NULL was specified for the data pointer, or memory could not be accessed
 * \retval #SISO_AUX_PORT_ERROR_INVALID_SIZE The size was either zero or too big for the type of port the handle refers to
 * \retval #SISO_AUX_PORT_ERROR_IO_FAILURE A general input/output error occurred
 * \retval #SISO_AUX_PORT_ERROR_TIMEOUT The operation could not be completed within the time specified
 * \retval #SISO_AUX_PORT_ERROR_OUT_OF_MEMORY Not enough memory to allocate internal buffers
 * \retval #SISO_AUX_PORT_ERROR_INVALID_RWFLAGS The flags passed were invalid for the type of port the handle refers to, or not allowed in the combination given, or plain invalid
 * \retval #SISO_JTAG_PORT_ERROR_NO_CHAIN_SET No JTAG chain was set
 * \retval #SISO_I2C_PORT_ERROR_ARBITRATION_LOST Arbitration on a multi-master I2C bus was lost
 * \retval #SISO_I2C_PORT_ERROR_ACK_NAK_MISMATCH Device did not answer or rejected the request
 */
int SisoAuxPortTransmit(SisoAuxPort handle, unsigned int device_id, const void * data_out, unsigned int size_out, unsigned int * length_out_ptr, void * data_in, unsigned int size_in, unsigned int * length_in_ptr, unsigned int flags, unsigned int timeout);

/*@}*/



/** @name Function results
 *
 * These values can be returned by functions.
 */
/*@{*/

#define SISO_AUX_PORT_SUCCESS                          0   /**< Function returned successfully */
#define SISO_AUX_PORT_ERROR                          (-1)  /**< An internal error occurred */
#define SISO_AUX_PORT_ERROR_INVALID_POINTER          (-2)  /**< A pointer passed as parameter was either NULL, or memory could not be accessed */
#define SISO_AUX_PORT_ERROR_INVALID_SIZE             (-3)  /**< The size of memory passed by pointer was invalid */
#define SISO_AUX_PORT_ERROR_INVALID_BOARD_ID         (-4)  /**< No board could be found for the specified id */
#define SISO_AUX_PORT_ERROR_INVALID_PORT_TYPE        (-5)  /**< The port type specified was invalid */
#define SISO_AUX_PORT_ERROR_INVALID_PORT_NUMBER      (-6)  /**< No port could be found for the specified number */
#define SISO_AUX_PORT_ERROR_INVALID_PORT_HANDLE      (-7)  /**< The port handle was invalid */
#define SISO_AUX_PORT_ERROR_BUSY                     (-8)  /**< The port was already opened using another handle */
#define SISO_AUX_PORT_ERROR_OUT_OF_MEMORY            (-9)  /**< Not enough memory */
#define SISO_AUX_PORT_ERROR_PORT_NOT_OPEN           (-10)  /**< The port has not been opened yet */
#define SISO_AUX_PORT_ERROR_OVERFLOW                (-11)  /**< The operation exceeded the available buffers */
#define SISO_AUX_PORT_ERROR_NOT_IMPLEMENTED         (-49)  /**< The function or feature has not been implemented yet */
#define SISO_AUX_PORT_ERROR_INVALID_RWFLAGS         (-51)  /**< The flags passed were invalid for the type of port the handle refers to, or not allowed in the combination given, or plain invalid */
#define SISO_AUX_PORT_ERROR_IO_FAILURE              (-52)  /**< A general input/output error occurred */
#define SISO_AUX_PORT_ERROR_TIMEOUT                 (-53)  /**< The operation could not be completed within the time specified */
#define SISO_AUX_PORT_ERROR_INVALID_FLUSH_TYPE      (-54)  /**< The value for flush type was invalid */
#define SISO_AUX_PORT_ERROR_NO_DATA                 (-55)  /**< No data is available */
#define SISO_AUX_PORT_ERROR_INVALID_PROPERTY       (-101)  /**< The name of the property to get/set was invalid, or not available for the type of the port the handle refers to */
#define SISO_AUX_PORT_ERROR_INVALID_PROPERTY_TYPE  (-102)  /**< The type of the value pointer does not match the property value */
#define SISO_AUX_PORT_ERROR_READ_ONLY_PROPERTY     (-103)  /**< The property can only be retrieved, not set */
#define SISO_AUX_PORT_ERROR_FREQUENCY_TOO_LOW      (-110)  /**< The requested frequency was too low */
#define SISO_AUX_PORT_ERROR_FREQUENCY_TOO_HIGH     (-111)  /**< The requested frequency was too high */
#define SISO_JTAG_PORT_ERROR                       (-200)  
#define SISO_JTAG_PORT_ERROR_INVALID_CHAIN         (-201)  /**< The JTAG chain specified was invalid */
#define SISO_JTAG_PORT_ERROR_INVALID_IR_LENGTH     (-202)  /**< The length of the instruction register specified in the JTAG chain was invalid */
#define SISO_JTAG_PORT_ERROR_INVALID_DEVICE_ID     (-203)  /**< The device id specified in the JTAG chain was invalid */
#define SISO_JTAG_PORT_ERROR_CHAIN_LENGTH_MISMATCH (-204)  /**< The length of the JTAG chain specified was invalid */
#define SISO_JTAG_PORT_ERROR_NO_CHAIN_SET          (-205)  /**< No JTAG chain was set */
#define SISO_JTAG_PORT_ERROR_DATA_LENGTH_MISMATCH  (-206)  /**< TMS data length does not match TDI data length on RAW send */
#define SISO_I2C_PORT_ERROR_ARBITRATION_LOST       (-301)  /**< Arbitration on a multi-master I2C bus was lost */
#define SISO_I2C_PORT_ERROR_ACK_NAK_MISMATCH       (-302)  /**< Device did not answer or rejected the request */
#define SISO_UIQ_PORT_ERROR_INVALID_TRANSMISSION_MODE   (-401)  /**< provided Transmission mode is invalid */
#define SISO_UIQ_PORT_ERROR_INVALID_SPI_MODE       (-402)  /**< provided SPI mode is invalid */

/*@}*/


#ifdef __cplusplus
}
#endif

#endif /* SISO_AUX_PORT_H */
