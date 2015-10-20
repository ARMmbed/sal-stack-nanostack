/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */
#ifndef _NS_SOCKET_API_H
#define _NS_SOCKET_API_H

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \file socket_api.h
 * \brief 6LoWPAN Library Socket API
 *
 * \section socket-com Common Socket API
 *  - socket_open(), A function to open a socket
 *  - socket_free(), A function to free a socket
 *  - socket_raw_open(), A function to open a raw ICMP socket
 *
 *
 * \section socket-read Socket Read API at Callback
 *  - socket_read(), A function to read received data buffer from a socket
 *  - socket_read_info(), A function to read signal information and length info from a socket
 *  - socket_read_session_address(), A function to read a session info for TCP event
 *
 * \section socket-tx Socket TX API
 * - socket_send(), A function to write data buffer to a socket (TCP Client Only)
 * - socket_sendto(), A function to write data to socket to specific destination
 *
 * \section sock-connect TCP Socket Connection Handle
 *  - socket_listen(), A function to set the socket to the listening mode
 *  - socket_connect(), A function to connect to remote peer
 *  - socket_close(), A function to close a connection
 *
 * Sockets are common abstraction model for network communication and are used in most operating systems.
 * The 6LoWPAN Library API follows BSD socket API conventions closely with some extensions necessitated by the event-based scheduling model.
 *
 * Library supports following socket types:
 * | Socket name    |   Socket description           |
 * | :------------: | :----------------------------: |
 * | SOCKET_UDP     |   UDP socket type              |
 * | SOCKET_TCP     |   TCP socket type              |
 *
 * \section socket-raw ICMP RAW Socket Instruction
 * ICMP RAW socket can be created by socket_raw_open(). When using ICMP sockets the minimum payload length is 8 bytes, where first 4 bytes form the ICMP header.
 * The first byte is for type, second for code and last two are for checksum which must always be set to zero.
 * Stack will calculate the checksum automatically before transmitting the packet. Header is followed by the payload.

 * | ICMP Type |    ICMP Code | Checksum  | Payload    | Notes           |
 * | :-------: | :----------: | :-------: | :--------: | :--------------:|
 * | 1         | 1            | 2         | n (min. 4) | Length in bytes |
 * | 0xXX      | 0xXX         | 0x00 0x00 | n bytes    | Transmit        |
 * | 0xXX      | 0xXX         | 0xXX 0xXX | n bytes    | Receive         |
 *
 * ICMP echo request with 4 bytes of payload (ping6).
 *
 * | ICMP Type |    ICMP Code | Checksum  | Payload             |
 * | :-------: | :----------: | :-------: | :-----------------: |
 * | 0x80      | 0x00         | 0x00 0x00 | 0x00 0x01 0x02 0x03 |
 *
 * ICMP echo response for the message above.
 *
 * | ICMP Type |    ICMP Code | Checksum  | Payload             |
 * | :-------: | :----------: | :-------: | :-----------------: |
 * | 0x81      | 0x00         | 0xXX 0xXX | 0x00 0x01 0x02 0x03 |
 *
 * \section socket-receive Socket receiving
 * When there is data to read from the socket, a receive callback function is called with the socket event parameter from the library.
 * The state of the TCP socket then changes and the socket TX process is ready (SOCKET_TX_FAIL or SOCKET_TX_DONE).
 * Socket event has event_type and socket_id fields. The receive callback function must be defined when socket is opened using socket_open() API.
 *
 * All supported socket event types are listed here:
 *
 * | Event Type                 | Value | Description                                                         |
 * | :------------------------: | :---: | :-----------------------------------------------------------------: |
 * | SOCKET_EVENT_MASK          | 0xF0  | NC Socket event mask                                                |
 * | SOCKET_DATA                | 0x00  | Data received, Read data length from d_len field                    |
 * | SOCKET_BIND_DONE           | 0x10  | TCP connection ready                                                |
 * | SOCKET_TX_FAIL             | 0x50  | Socket data send failed                                             |
 * | SOCKET_CONNECT_CLOSED      | 0x60  | TCP connection closed                                               |
 * | SOCKET_CONNECT_FAIL_CLOSED | 0x70  | TCP connection closed � no ACK received                             |
 * | SOCKET_NO_ROUTER           | 0x80  | No route available to destination                                   |
 * | SOCKET_TX_DONE             | 0x90  | Last Socket TX process Done, at TCP case Whole TCP Process is ready |
 *
 * When socket call back is called application need to handle event and read data from the socket socket_read().
 * Read operation release memory from stack when read is called or after callback automatic.
 *
 *
 * \section socket-tcp How to Use TCP sockets
 *
 * | API                           | Socket Type   | Description                                                    |
 * | :---------------------------: | :-----------: | :------------------------------------------------------------: |
 * | socket_open()                 | Server/Client | Open socket to specific or dynamically port number.              |
 * | socket_close()                | Server/Client | Close opened TCP connection                                      |
 * | socket_listen()               | Server        | Set server port to listen state                                  |
 * | socket_connect()              | Client        | Connect client socket to specific destination                |
 * | socket_send()                 | Client        | Send data to session based destination                           |
 * | socket_sendto()               | Server/Client | Send data to specific destination                            |
 * | socket_read_session_address() | Server/Client | Function read socket TCP session address and port information. |
 *
 * When TCP socket is opened it is in closed state. It must be set either listen or to connect state before it can be used to receive or transmit data.
 *
 * Socket can be set to listen mode by calling socket_listen() function. After the call the socket can accept an incoming connection from a remote host.
 * Listen mode closes the connection automatically after server timeout or when client or application closes the connection manually by socket_close() function.
 *
 * TCP socket can be connected to a remote host by calling socket_connect() with correct arguments. After the function call (non-blocking) application must wait for the socket event to confirm the successful state change of the socket.
 * After the event of successful state change data can be sent using socket_send()  by client and socket_send() by server call.
 * Connection can be closed by calling socket_close() function or by server timeout.
 *
 * \section socket-udpicmp How to Use UDP and RAW Socket
 *
 * UDP socket is ready to receive and send data immediately after successful call of socket_open() and NET_READY event is received.
 * Data can be transmitted by using socket_sendto() function. ICMP socket works with same function call.
 *
 */

#include "ns_address.h"

/** Protocol IDs used when opened sockets*/
/** Doesn't reserve anything. Socket in free. */
#define SOCKET_NONE     0
/** UDP socket type */
#define SOCKET_UDP      17
/** Normal TCP socket type */
#define SOCKET_TCP      6
/** Local Sockets for Tun interface functionality to APP-APP trough any BUS */
#define SOCKET_LOCAL 1

/**
 * \brief A function to open a raw ICMP socket.
 *
 * \param passed_fptr pointer to socket callback function
 *
 * \return positive socket opened
 * \return -1 no free sockets
 * \return -2 port reserved
 */
extern int8_t socket_raw_open(void (*passed_fptr)(void *));

#define SOCKET_ICMP     32
/** ICMP Socket instruction
 *
 * If used ICMP socket, minimum payload length is 8-bytes, where first 4-bytes form ICMP header.
 * First byte is for type, second for code and last two are for checksum witch must always be zero.
 * Stack will calculate the checksum automatically when send. Payload comes after the header.
 *
 * This applies for reading and sending.
 *
 *      ICMP TYPE   ICMP Code   ICMP Checksum (2-bytes)     Payload n-bytes (min. 4 bytes)
 *      --------    --------    -------- --------           -------- -------- ....... -------
 *      0xXX        0xXX        0x00    0x00                0xXX    0xXX    ......  0xXX
 *
 * Example data flow for ping:
 *
 * ICMP echo request with 4-bytes payload (Ping6).
 *      0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03
 *
 * ICMP echo response for the message above.
 *      0x81, 0x00, 0xXX, 0xXX, 0x00, 0x01, 0x02, 0x03
 */

/*!
 * \struct socket_callback_t
 * \brief Socket Callback function structure type.
 */
typedef struct socket_callback_t {
    uint8_t event_type; /**< Socket Callback Event check list below*/
    int8_t socket_id; /** Socket id queue which socket cause call back */
    int8_t interface_id; /** Network Interface ID where Packet came */
    uint16_t  d_len;  /** Data length if event type is SOCKET_DATA */
    uint8_t LINK_LQI; /** LINK LQI info if interface cuold give */
} socket_callback_t;

/** IPv6 wildcard address IN_ANY */
extern const uint8_t ns_in6addr_any[16];

/**
 * \brief Used to initialize a socket for communication.
 *
 * Note: Maximum of seven sockets at the same time is supported.
 *
 * \param protocol defines the protocol to use.
 * \param identifier is e.g. in case of UDP, the UDP port.
 * \param passed_fptr is a function pointer to a function that is called whenever a data frame is received to this socket.
 *
 * \return 0 or greater on success; return value is the socket ID.
 * \return -1 on failure.
 * \return -2 on port reserved.
 *
 */
extern int8_t socket_open(uint8_t protocol, uint16_t identifier, void (*passed_fptr)(void *));

/**
 * \brief A function to free a socket.
 *
 * \param socket ID of the socket to be released.
 *
 * \return 0 socket released.
 * \return -1 socket not released.
 */
extern int8_t socket_free(int8_t socket);

/**
 * \brief A function to set the socket to the listening mode.
 *
 * \param socket socket ID to bind
 * \return 0 on success.
 * \return -1 on failure.
 */
extern int8_t socket_listen(int8_t socket);

/**
 * \brief A function to connect to remote peer (TCP).
 *
 * \param socket socket id
 * \param address address of remote peer
 * \param randomly_take_src_number 1=enable find next free random port number for current one
 *
 * \return 0 on success.
 * \return -1 in case of invalid socket id or parameter
 * \return -2 if memory allocation fails
 * \return -3 if socket is in listening state
 * \return -4 if socket is already connected
 * \return -5 connect is not supported on this type of socket
 * \return -6 if TCP session state is wrong
 * \return -7 if source address selection fails
 */
extern int8_t socket_connect(int8_t socket, ns_address_t *address, uint8_t randomly_take_src_number);

/**
 * \brief Bind socket to address.
 *
 * Note: `identifier` in the address structure identifies port to bind, it needs to differ from 0.
 *
 * Used by the application to bind socket to port.
 *
 * \param socket socket id
 * \param address address to bind
 *
 * \return 0 on success.
 * \return -1 if given address is NULL.
 * \return -2 if port is already bound to another socket.
 * \return -3 if trying to bind to port 0.
 * \return -4 if socket is already bound to port.
 */
extern int8_t socket_bind(int8_t socket, const ns_address_t *address);

/**
 * \brief Bind a local address to socket based on destination address and
 *  address selection preferences.
 *  Binding happens to the same address that socket_connect() would bind to.
 *  Reference: RFC5014 IPv6 Socket API for Source Address Selection.
 *
 * \param socket socket id
 * \param dst_address destination address to which socket wants to communicate.
 *
 * \return 0 on success.
 * \return -1 if given address is NULL or socket ID is invalid.
 * \return -2 if memory allocation failed.
 * \return -3 if socket is already bound to address.
 * \return -4 if interface can't be found.
 * \return -5 if source address selection fails.
 */
extern int8_t socket_bind2addrsel(int8_t socket, const ns_address_t *dst_address);

/**
 * \brief A function to close a connection.
 *
 * \param socket refers to the socket ID to close.
 * \param address refers to the destination client address. When using as a client, a null pointer shall be passed.
 *
 * \return 0 on success.
 * \return -1 if a given socket ID is not found, if a socket type is wrong or tcp_close() returns a failure.
 * \return -2 if no active tcp session was found.
 * \return -3 if referred socket ID port is declared as a zero.
 *
 * Note: randomly_take_src_number is highly recommend to use always. Stack will generate new source port between 49152-65534.
 */
extern int8_t socket_close(int8_t socket, ns_address_t *address);

/**
 * \brief Used to send data via a connected TCP socket by client.
 *
 * Note: Socket connection must be ready before using this function.
 * Stack will automatically use the address of the remote connected host as the destination address for the packet.
 *
 * \param socket socket id
 * \param buffer pointer to data
 * \param length data length
 *
 * \return 0 done
 * \return -1 invalid socket id
 * \return -2 Socket memory allocation fail
 * \return -3 TCP state not established
 * \return -4 Socket tx process busy
 * \return -5 Socket is not connected
 * \return -6 Packet too short
 */
extern int8_t socket_send(int8_t socket, uint8_t *buffer, uint16_t length);

/**
 * \brief A function to read received data buffer from a socket.
 *
 * Used by the application to get data from a socket.
 *
 * \param socket refers to socket ID to be read from.
 * \param src_addr is NULL, or points to a structure into which the sender's address is stored.
 * \param buffer is a pointer to an array to where the read data is written to.
 * \param length is the maximum length of the allocated buffer.
 *
 * \return greater than 0, indicating the length of the data copied to buffer.
 * \return 0 if no data is available to read.
 * \return -1 invalid input parameters.
 * \return -2 given buffer length is too short to store all data available.
 */
extern int16_t socket_read(int8_t socket, ns_address_t *src_addr, uint8_t *buffer, uint16_t length);

/**
 * \brief Used to send UDP, TCP or raw ICMP data via the socket.
 *
 * Used by the application to send data.
 *
 * \param socket refers to socket ID.
 * \param address is a pointer to the destination address information.
 * \param buffer is a pointer to a data to be sent.
 * \param length is a length of the data to be sent.
 * \return 0 on success.
 * \return -1 invalid socket id.
 * \return -2 Socket memory allocation fail.
 * \return -3 TCP state not established.
 * \return -4 Socket tx process busy.
 * \return -6 Packet too short.
 */
extern int8_t socket_sendto(int8_t socket, ns_address_t *address, uint8_t *buffer, uint16_t length);

/**
 * \brief A function to read a session info for TCP event.
 *
 *
 * \param socket refers to the ID to be read from.
 * \param address is a pointer to the address structure to where the session address information is read to.
 *
 * \return 0 on success.
 * \return -1 if no socket is found or TCP is not compiled into this project.
 * \return -2 if no session information is found.
 *
 * Note: Function should be called only at Socket call back call when socket event is SOCKET_BIND_DONE or SOCKET_TX_DONE.
 * The following sections introduce those functions.
 */
extern int8_t socket_read_session_address(int8_t socket, ns_address_t *address);


/** Flags for SOCKET_IPV6_ADDR_PREFERENCES - opposites 16 bits apart */
#define SOCKET_IPV6_PREFER_SRC_TMP              0x00000001 /**< Prefer Temporary address (RFC 4941); default */
#define SOCKET_IPV6_PREFER_SRC_PUBLIC           0x00010000 /**< Prefer Public address (RFC 4941) */
#define SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT    0x00000100 /**< Prefer 6LoWPAN short address */
#define SOCKET_IPV6_PREFER_SRC_6LOWPAN_LONG     0x01000000 /**< Prefer 6LoWPAN long address */

/** Options for SOCKET_IPV6_ADDRESS_SELECT */
#define SOCKET_SRC_ADDRESS_MODE_PRIMARY     0 /**< Default value always */
#define SOCKET_SRC_ADDRESS_MODE_SECONDARY   1 /**< This mode typically select secondary address */


/** Protocol levels used for socket_setsockopt */
#define SOCKET_IPPROTO_IPV6         41

/** Option names for protocol level SOCKET_IPPROTO_IPV6 */
/** Specify traffic class for outgoing packets, as int16_t (RFC 3542 S6.5 says int); valid values 0-255, or -1 for system default */
#define SOCKET_IPV6_TCLASS                  1
/** Specify hop limit for outgoing unicast packets, as int16_t (RFC 3493 S5.1 says int); valid values 0-255, or -1 for system default */
#define SOCKET_IPV6_UNICAST_HOPS            2
/** Specify hop limit for outgoing multicast packets, as int16_t (RFC 3493 S5.2 says int); valid values 0-255, or -1 for system default */
#define SOCKET_IPV6_MULTICAST_HOPS          3
/** Specify source address preferences, as uint32_t - see RFC 5014; valid values combinations of SOCKET_IPV6_PREFER_xxx flags */
#define SOCKET_IPV6_ADDR_PREFERENCES        4
/** Specify PMTU preference, as int8_t (RFC 3542 S11.1 says int); valid values -1 (PMTUD for unicast, default), 0 (PMTUD always), 1 (PMTUD off) */
#define SOCKET_IPV6_USE_MIN_MTU             5
/** Specify to not fragment datagrams, as int8_t (RFC 3542 S11.2 says int); valid values 0 (fragment to path MTU, default), 1 (no fragmentation, TX fails if bigger than outgoing interface MTU) */
#define SOCKET_IPV6_DONTFRAG                6

#define SOCKET_LINK_LAYER_SECURITY          0xfd /** Not standard Enable or disable socket security  at Link layer (For 802.15.4)*/

#define SOCKET_INTERFACE_SELECT             0xfe /** Not standard Socket interface ID */
/** Deprecated - use SOCKET_IPV6_ADDR_PREFERENCES instead */
#define SOCKET_IPV6_ADDRESS_SELECT          0xff /** Not standard Socket address mode */

/**
 * \brief Set an option for a socket
 *
 * Used to specify miscellaneous options for a socket. Supported levels and
 * names defined above.
 *
 * \param socket socket id
 * \param level protocol level
 * \param opt_name option name (interpretation depends on level)
 * \param opt_value pointer to value for the specified option
 * \param opt_len size of the data pointed to by value
 *
 * \return 0 on success
 * \return -1 invalid socket id
 * \return -2 invalid/unsupported option
 * \return -3 invalid option value
 */
extern int8_t socket_setsockopt(int8_t socket, uint8_t level, uint8_t opt_name, const void *opt_value, uint16_t opt_len);

/**
 * \brief Get an option for a socket
 *
 * Used to read miscellaneous options for a socket. Supported levels and
 * names defined above. If the buffer is smaller than the option, the output
 * is silently truncated; otherwise opt_len is modified to indicate the actual
 * length.
 *
 * \param socket socket id
 * \param level protocol level
 * \param opt_name option name (interpretation depends on level)
 * \param opt_value pointer to output buffer
 * \param opt_len pointer to length of output buffer; updated on exit
 *
 * \return 0 on success
 * \return -1 invalid socket id
 * \return -2 invalid/unsupported option
 */
extern int8_t socket_getsockopt(int8_t socket, uint8_t level, uint8_t opt_name, void *opt_value, uint16_t *opt_len);

#ifdef __cplusplus
}
#endif
#endif /*_NS_SOCKET_H*/
