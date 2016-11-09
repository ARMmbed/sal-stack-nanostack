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
 * \section socket-com Common socket API
 *  - socket_open(), A function to open a socket.
 *  - socket_close(), A function to close a socket.
 *
 * \section socket-read Socket read API at callback
 *  - socket_read(), A function to read received data buffer from a socket.
 *  - socket_recvmsg(), A function to read received data buffer from a socket to Posix defined message structure
 *  - socket_read_session_address(), A function to read session info for a TCP event.
 *
 * \section socket-tx Socket TX API
 * - socket_send(), A function to write data buffer to a socket.
 * - socket_sendto(), A function to write data to a specific destination in the socket.
 * - socket_senmsg(), A function which support socket_send and socket_sendto functionality which supports ancillary data
 *
 * \section sock-connect TCP socket connection handle
 *  - socket_listen(), A function to set the socket to listening mode.
 *  - socket_connect(), A function to connect to a remote peer.
 *  - socket_shutdown(), A function to shut down a connection.
 *
 * Sockets are a common abstraction model for network communication and are used in most operating systems.
 * 6LoWPAN Library API follows BSD socket API conventions closely with some extensions necessitated
 * by the event-based scheduling model.
 *
 * Library supports the following socket types:
 * | Socket name    |   Socket description           |
 * | :------------: | :----------------------------: |
 * | SOCKET_UDP     |   UDP socket type              |
 * | SOCKET_TCP     |   TCP socket type              |
 * | SOCKET_ICMP    |   ICMP raw socket type         |
 *
 * \section socket-raw ICMP RAW socket instruction
 * An ICMP raw socket can be created with the function socket_open() and the
 * identifier 0xffff. When using ICMP sockets, the minimum packet length is 4
 * bytes which is the 4 bytes of ICMP header. The first byte is for type, second
 * for code and last two are for the checksum that must always be set to zero.
 * The stack will calculate the checksum automatically before transmitting the packet.
 * THe header is followed by the payload if there is any.
 * NOTE: While it is perfectly legal to send an ICMP packet without payload,
 * some packet sniffing softwares might regard such packets as malformed.
 * In such a case, a dummy payload can be attached by providing a socket_sendto()
 * function with a pointer to your desired data buffer.
 * Moreover, the current implementation of the stack allows only certain ICMP types, for example
 * ICMP echo, reply, destination unreachable, router advertisement, router solicitation.
 * It will drop any other unimplemented types. For example, Private experimentation type (200) will
 * be dropped by default.

 * | ICMP Type |    ICMP Code | Checksum  | Payload    | Notes           |
 * | :-------: | :----------: | :-------: | :--------: | :--------------:|
 * | 1         | 1            | 2         | n bytes    | Length in bytes |
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
 * When there is data to read from the socket, a receive callback function is called with the event type parameter set to SOCKET_DATA.
 * The application must read the received data with socket_read() or socket_recvmsg() during the callback because the stack will release the data
 * when returning from the receive callback.
 *
 * Socket event has event_type and socket_id fields. The receive callback function must be defined when socket is opened using socket_open() API.
 *
 * All supported socket event types are listed here:
 *
 * | Event Type                 | Value | Description                                                         |
 * | :------------------------: | :---: | :-----------------------------------------------------------------: |
 * | SOCKET_EVENT_MASK          | 0xF0  | NC Socket event mask.                                               |
 * | SOCKET_DATA                | 0x00  | Data received, read data length available in d_len field.           |
 * | SOCKET_BIND_DONE           | 0x10  | TCP connection ready.                                               |
 * | SOCKET_TX_FAIL             | 0x50  | Socket data send failed.                                            |
 * | SOCKET_CONNECT_CLOSED      | 0x60  | TCP connection closed.                                              |
 * | SOCKET_CONNECTION_RESET    | 0x70  | TCP connection reset.                                               |
 * | SOCKET_NO_ROUTER           | 0x80  | No route available to destination.                                  |
 * | SOCKET_TX_DONE             | 0x90  | Last socket TX process done, in TCP, whole TCP process is ready.    |
 * | SOCKET_NO_RAM              | 0xA0  | No RAM available.                                                   |
 *
 *
 * \section socket-tcp How to use TCP sockets:
 *
 * | API                           | Socket Type   | Description                                                      |
 * | :---------------------------: | :-----------: | :------------------------------------------------------------:   |
 * | socket_open()                 | Server/Client | Open socket to specific or dynamic port number.                  |
 * | socket_shutdown()             | Client        | Shut down opened TCP connection.                                 |
 * | socket_listen()               | Server        | Set server port to listen state.                                 |
 * | socket_connect()              | Client        | Connect client socket to specific destination.                   |
 * | socket_close()                | Server/Client | Closes the TCP Socket.                   |
 * | socket_send()                 | Client        | Send data to session based destination.                          |
 * | socket_sendto()               | Server/Client | Send data to specific destination.                               |
 * | socket_read_session_address() | Server/Client | Read socket TCP session address and port information.            |
 *
 * When the TCP socket is opened it is in closed state. It must be set either to listen or to connect state before it can be used to receive or transmit data.
 *
 * A socket can be set to listen mode with the socket_listen() function. After the call, the socket can accept an incoming connection from a remote host.
 * The listen mode closes the connection automatically after server timeout or when the client or application closes the connection manually by socket_shutdown() function.
 *
 * A TCP socket can be connected to a remote host with socket_connect() with correct arguments. After the function call, a (non-blocking) application must wait for the socket event to confirm the successful state change of the socket.
 * After the successful state change, data can be sent using socket_send() by client and socket_send() by server.
 * The connection can be shut down with socket_shutdown() function or by server timeout.
 *
 * \section socket-udpicmp How to use UDP and RAW socket:
 *
 * A UDP socket is ready to receive and send data immediately after a successful call of socket_open() and a NET_READY event is received.
 * Data can be transmitted with the socket_sendto() function. An ICMP socket works with same function call.
 *
 */

#include "ns_address.h"

/** \name Protocol IDs used with sockets. */
///@{
/** UDP socket type. */
#define SOCKET_UDP      17
/** Normal TCP socket type. */
#define SOCKET_TCP      6
/** ICMPv6 raw socket type */
#define SOCKET_ICMP     32
/** Raw IPv6 socket type (socket_open identifier is IPv6 protocol number) */
#define SOCKET_RAW      2
/** REMOVED Feature in this release socket open return error Local Sockets for Tun interface functionality to APP-APP trough any BUS */
#define SOCKET_LOCAL    1
///@}

/** ICMP socket instruction
 *
 * ICMP header is comprised of 4 bytes. The first byte is for type, second for code and
 * the last two are for checksum that must always be zero.
 * The stack will calculate the checksum automatically when sending the packet.
 * The payload comes after the header and it can be of any length. It can also be set to 0.
 *
 * This applies for reading and sending.
 *
 *      ICMP TYPE   ICMP Code   ICMP Checksum (2 bytes)     Payload n bytes
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
    uint8_t event_type;     /**< Socket Callback Event check list below */
    int8_t socket_id;       /**< Socket id queue which socket cause call back */
    int8_t interface_id;    /**< Network Interface ID where Packet came */
    uint16_t  d_len;        /**< Data length if event type is SOCKET_DATA */
    uint8_t LINK_LQI;       /**< LINK LQI info if interface cuold give */
} socket_callback_t;

/*!
 * \struct ns_msghdr_t
 * \brief Normal IP socket message structure for socket_recvmsg() and socket_sendmsg().
 */

typedef struct ns_msghdr {
    void *msg_name;                 /**< Optional address send use for destination and receive it will be source. MUST be ns_address_t */
    uint_fast16_t msg_namelen;      /**< Length of optional address use sizeof(ns_address_t) when msg_name is defined */
    ns_iovec_t *msg_iov;            /**< Message data vector list */
    uint_fast16_t  msg_iovlen;      /**< Data vector count in msg_iov */
    void *msg_control;              /**< Ancillary data list of ns_cmsghdr_t pointer */
    uint_fast16_t  msg_controllen;  /**< Ancillary data length */
    int flags;                      /**< Flags for received messages */
} ns_msghdr_t;

/*!
 * \struct ns_cmsghdr_t
 * \brief Control messages.
 */
typedef struct ns_cmsghdr {
    uint16_t cmsg_len;      /**< Ancillary data control messages length including cmsghdr */
    uint8_t cmsg_level;     /**< Originating protocol level should be SOCKET_IPPROTO_IPV6 */
    uint8_t cmsg_type;      /**< Protocol Specific types for example SOCKET_IPV6_PKTINFO,  */
} ns_cmsghdr_t;


/** \name socket_recvmsg() message error flags.
 * \anchor MSG_HEADER_FLAGS
 */
///@{
/** In msg_flags, indicates that given data buffer was smaller than received datagram. Can also be passed as an flag parameter to recvmsg. */
#define NS_MSG_TRUNC    1
/** Indicates that given ancillary data buffer was smaller than enabled at socket msg->msg_controllen define proper writed data lengths. */
#define NS_MSG_CTRUNC   2
///@}
/*!
 * \struct ns_in6_pktinfo_t
 * \brief IPv6 packet info which is used for socket_recvmsg() socket_sendmsg().
 */
typedef struct ns_in6_pktinfo {
    uint8_t ipi6_addr[16];    /**< src/dst IPv6 address */
    int8_t  ipi6_ifindex;    /**< send/recv interface index */
} ns_in6_pktinfo_t;

#define CMSG_HEADER_ALIGN sizeof(long)

#define CMSG_DATA_ALIGN CMSG_HEADER_ALIGN

#ifndef NS_ALIGN_SIZE
#define NS_ALIGN_SIZE(length, aligment_base) \
    ((length + (aligment_base -1 )) & ~(aligment_base -1))
#endif

/**
 * \brief Parse first control message header from message ancillary data.
 *
 * \param msgh Pointer for socket message.
 *
 * \return Pointer to first control message header , Could be NULL when control_message is undefined.
 */
#define NS_CMSG_FIRSTHDR(msgh) \
    ((msgh)->msg_controllen >= sizeof(struct ns_cmsghdr) ? \
            (struct ns_cmsghdr *)(msgh)->msg_control : \
            (struct ns_cmsghdr *)NULL )
/**
 * \brief Parse next control message from message by current control message header.
 *
 * \param msgh Pointer for socket message.
 * \param cmsg Pointer for last parsed control message
 *
 * \return Pointer to Next control message header , Could be NULL when no more control messages data.
 */

ns_cmsghdr_t *NS_CMSG_NXTHDR(const ns_msghdr_t *msgh, const ns_cmsghdr_t *cmsg);

/**
 * \brief Get Data pointer from control message header.
 *
 * \param cmsg Pointer (ns_cmsghdr_t) for last parsed control message
 *
 * \return Data pointer.
 */
#define NS_CMSG_DATA(cmsg) \
    ((uint8_t *)(cmsg) + NS_ALIGN_SIZE(sizeof(ns_cmsghdr_t), CMSG_DATA_ALIGN))

/**
 * \brief Returns the total space required for a cmsg header plus the specified data, allowing for all padding
 *
 * \param length  size of the ancillary data
 *
 * For example, the space required for a SOCKET_IPV6_PKTINFO is NS_CMSG_SPACE(sizeof(ns_in6_pktinfo))
 *
 * \return Total size of CMSG header, data and trailing padding.
 */
#define NS_CMSG_SPACE(length) \
    (NS_ALIGN_SIZE(sizeof(ns_cmsghdr_t), CMSG_DATA_ALIGN) + NS_ALIGN_SIZE(length, CMSG_HEADER_ALIGN))

/**
 * \brief Calculate length to store in cmsg_len with taking into account any necessary alignment.
 *
 * \param length  size of the ancillary data
 *
 * For example, cmsg_len for a SOCKET_IPV6_PKTINFO is NS_CMSG_LEN(sizeof(ns_in6_pktinfo))
 *
 * \return Size of CMSG header plus data, without trailing padding.
 */
#define NS_CMSG_LEN(length) \
    (NS_ALIGN_SIZE(sizeof(ns_cmsghdr_t), CMSG_DATA_ALIGN) + length)


/** IPv6 wildcard address IN_ANY */
extern const uint8_t ns_in6addr_any[16];

/**
 * \brief Create and initialize a socket for communication.
 *
 * \param protocol Defines the protocol to use.
 * \param identifier The socket port. 0 indicates that port is not specified and it will be selected automatically when using the socket.
 * \param passed_fptr A function pointer to a function that is called whenever a data frame is received to this socket.
 *
 * \return 0 or greater on success; Return value is the socket ID.
 * \return -1 on failure.
 * \return -2 on port reserved.
 */
int8_t socket_open(uint8_t protocol, uint16_t identifier, void (*passed_fptr)(void *));

/**
 * \brief A function to close a socket.
 *
 * \param socket ID of the socket to be closed.
 *
 * \return 0 socket closed.
 * \return -1 socket not closed.
 */
int8_t socket_close(int8_t socket);

/**
 * \brief A function to set a socket to listening mode.
 *
 * \param socket The socket ID.
 * \param backlog The pending connections queue size. (Not yet implemented).
 * \return 0 on success.
 * \return -1 on failure.
 */
int8_t socket_listen(int8_t socket, uint8_t backlog);

/**
 * \brief A function to accept a new connection on an socket.
 *
 * NOT YET IMPLEMENTED - PLACEHOLDER FOR FUTURE TCP CHANGES
 *
 * \param socket_id The socket ID of the listening socket.
 * \param addr Either NULL pointer or pointer to structure where the remote address of the connecting host is copied.
 * \param passed_fptr A function pointer to a function that is called whenever a data frame is received to the new socket.
 * \return 0 or greater on success; return value is the new socket ID.
 * \return -1 on failure.
 */
int8_t socket_accept(int8_t socket_id, ns_address_t *addr, void (*passed_fptr)(void *));

/**
 * \brief A function to connect to remote peer (TCP).
 *
 * \param socket The socket ID.
 * \param address The address of a remote peer.
 * \deprecated \param randomly_take_src_number Ignored - random local port is always chosen if not yet bound
 *
 * \return 0 on success.
 * \return -1 in case of an invalid socket ID or parameter.
 * \return -2 if memory allocation fails.
 * \return -3 if the socket is in listening state.
 * \return -4 if the socket is already connected.
 * \return -5 connect is not supported on this type of socket.
 * \return -6 if the TCP session state is wrong.
 * \return -7 if the source address selection fails.
 */
int8_t socket_connect(int8_t socket, ns_address_t *address, uint8_t randomly_take_src_number);

/**
 * \brief Bind socket to address.
 *
 * Used by the application to bind a socket to a port and/or an address. Binding can
 * be done only once. The port or address cannot be changed after binding.
 *
 * \param socket Socket ID of the socket to bind.
 * \param address Address structure containing the port and address to bind.
 *
 * \return 0 on success.
 * \return -1 if the given address is NULL.
 * \return -2 if the port is already bound to another socket.
 * \return -3 if address is not us.
 * \return -4 if the socket is already bound.
 * \return -5 bind is not supported on this type of socket.
 *
 */
int8_t socket_bind(int8_t socket, const ns_address_t *address);

/**
 * \brief Bind a local address to a socket based on the destination address and
 *  the address selection preferences.
 *  Binding happens to the same address that socket_connect() would bind to.
 *  Reference: RFC5014 IPv6 Socket API for Source Address Selection.
 *
 * \param socket The socket ID.
 * \param dst_address The destination address to which the socket wants to communicate.
 *
 * \return 0 on success.
 * \return -1 if the given address is NULL or socket ID is invalid.
 * \return -2 if the memory allocation failed.
 * \return -3 if the socket is already bound to address.
 * \return -4 if the interface cannot be found.
 * \return -5 if the source address selection fails.
 * \return -6 bind2addrsel is not supported on this type of socket.
 */
int8_t socket_bind2addrsel(int8_t socket, const ns_address_t *dst_address);

/**
 * Options for the socket_shutdown() parameter 'how'.
 */
#define SOCKET_SHUT_RD      0   ///< Disables further receive operations.
#define SOCKET_SHUT_WR      1   ///< Disables further send operations.
#define SOCKET_SHUT_RDWR    2   ///< Disables further send and receive operations.

/**
 * \brief A function to shut down a connection.
 *
 * \param socket The ID of the socket to be shut down.
 * \param how How socket is to be shut down, one of SOCKET_SHUT_XX.
 *
 * \return 0 on success.
 * \return -1 if the given socket ID is not found, if the socket type is wrong or TCP layer returns a failure.
 * \return -2 if no active TCP session was found.
 */
int8_t socket_shutdown(int8_t socket, uint8_t how);

/**
 * \brief Send data via a connected TCP socket by client.
 *
 * Note: The socket connection must be ready before using this function.
 * The stack uses automatically the address of the remote connected host as the destination address for the packet.
 *
 * \param socket The socket ID.
 * \param buffer A pointer to data.
 * \param length Data length.
 *
 * \return 0 done
 * \return -1 Invalid socket ID.
 * \return -2 Socket memory allocation fail.
 * \return -3 TCP state not established or address scope not defined .
 * \return -4 Socket TX process busy or unknown interface.
 * \return -5 Socket not connected
 * \return -6 Packet too short (ICMP raw socket error).
 */
int8_t socket_send(int8_t socket, uint8_t *buffer, uint16_t length);

/**
 * \brief A function to read received data buffer from a socket.
 *
 * Used by the application to get data from a socket. This method must be called once
 * from a socket callback when handling event SOCKET_DATA. If the received data does not fit
 * in the buffer provided the excess data bytes are discarded.
 *
 * \param socket The socket ID.
 * \param src_addr A pointer to a structure where the sender's address is stored.
 * \param buffer A pointer to an array where the read data is written to.
 * \param length The maximum length of the allocated buffer.
 *
 * \return greater than 0 indicates the length of the data copied to buffer.
 * \return 0 if no data is available to read.
 * \return -1 invalid input parameters.
 */
int16_t socket_read(int8_t socket, ns_address_t *src_addr, uint8_t *buffer, uint16_t length);

/**
 * \brief A function to read received message with ancillary data from a socket.
 *
 * Used by the application to get data from a socket. This method must be called once
 * from a socket callback when handling event SOCKET_DATA. If the received data does not fit
 * in the buffer provided the excess data bytes are discarded.
 *
 * Ancillary data must request by socket_setsockopt().
 *
 * msg->msg_controllen is updated to indicate actual length of ancillary data output
 *
 * The returned length is normally the length of data actually written to the buffer; if
 * NS_MSG_TRUNC is set in flags, then for non-stream sockets, the actual datagram length is
 * returned instead, which may be larger than the buffer size.
 *
 * \param socket The socket ID.
 * \param msg A pointer to a structure where messages is stored with or without ancillary data
 * \param flags A flags for message read.
 *
 * \return greater than 0 indicates the length of the data.
 * \return 0 if no data is available to read.
 * \return -1 invalid input parameters.
 */
int16_t socket_recvmsg(int8_t socket, ns_msghdr_t *msg, int flags);

/**
 * \brief A function to send UDP, TCP or raw ICMP data via the socket.
 *
 * Used by the application to send data.
 *
 * \param socket The socket ID.
 * \param address A pointer to the destination address information.
 * \param buffer A pointer to data to be sent.
 * \param length Length of the data to be sent.
 *
 * \return 0 on success.
 * \return -1 Invalid socket ID.
 * \return -2 Socket memory allocation fail.
 * \return -3 TCP state not established or address scope not defined .
 * \return -4 Socket TX process busy or unknown interface.
 * \return -5 Socket not connected
 * \return -6 Packet too short (ICMP raw socket error).
 */
int8_t socket_sendto(int8_t socket, ns_address_t *address, uint8_t *buffer, uint16_t length);

/**
 * \brief A function to send UDP, TCP or raw ICMP data via the socket with or without ancillary data or destination address.
 *
 * Used by the application to send data message header support also vector list socket_send() and socket_sendto() use this functionality internally.
 *
 * \param socket The socket ID.
 * \param msg A pointer to the Message header which include address, payload and ancillary data.
 * \param flags A flags for message send for future usage (not supported yet)
 *
 * Messages destination address is defined by msg->msg_name which must be ns_address_t. If msg->msg_nme is NULL socket select connected address
 *
 * Messages payload and length is defined msg->msg_iov and msg->msg_iovlen. API support to send multiple data vector.
 *
 * Supported ancillary data for send defined by msg->msg_control and msg->msg_controllen.
 *
 * msg->flags and flags is ignored
 *
 * \return 0 on success.
 * \return -1 Invalid socket ID or message structure.
 * \return -2 Socket memory allocation fail.
 * \return -3 TCP state not established or address scope not defined .
 * \return -4 Socket TX process busy or unknown interface.
 * \return -5 Socket not connected
 * \return -6 Packet too short (ICMP raw socket error).
 */
int8_t socket_sendmsg(int8_t socket, const ns_msghdr_t *msg, int flags);

/**
 * \brief A function to read session info for TCP event.
 *
 *
 * \param socket The socket ID.
 * \param address A pointer to the address structure where the session address information is read to.
 *
 * \return 0 on success.
 * \return -1 if no socket is found or TCP is not compiled into this project.
 * \return -2 if no session information is found.
 *
 * Note: This function should be called only at socket callback when the socket event is SOCKET_BIND_DONE or SOCKET_TX_DONE.
 * The following sections introduce those functions.
 */
int8_t socket_read_session_address(int8_t socket, ns_address_t *address);


/** \name Flags for SOCKET_IPV6_ADDR_PREFERENCES - opposites 16 bits apart. */
///@{
#define SOCKET_IPV6_PREFER_SRC_TMP              0x00000001 /**< Prefer temporary address (RFC 4941); default. */
#define SOCKET_IPV6_PREFER_SRC_PUBLIC           0x00010000 /**< Prefer public address (RFC 4941). */
#define SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT    0x00000100 /**< Prefer 6LoWPAN short address. */
#define SOCKET_IPV6_PREFER_SRC_6LOWPAN_LONG     0x01000000 /**< Prefer 6LoWPAN long address. */
///@}

/** \name Options for SOCKET_IPV6_ADDRESS_SELECT. */
///@{
#define SOCKET_SRC_ADDRESS_MODE_PRIMARY     0 /**< Default value always. */
#define SOCKET_SRC_ADDRESS_MODE_SECONDARY   1 /**< This mode typically selects the secondary address. */
///@}

/** \name Protocol levels used for socket_setsockopt. */
///@{
#define SOCKET_IPPROTO_IPV6         41	/**< IPv6. */
///@}

/** \name Option names for protocol level SOCKET_IPPROTO_IPV6.
 * \anchor OPTNAMES_IPV6
 */
///@{
/** Specify traffic class for outgoing packets, as int16_t (RFC 3542 S6.5 says int); valid values 0-255, or -1 for system default. */
#define SOCKET_IPV6_TCLASS                  1
/** Specify hop limit for outgoing unicast packets, as int16_t (RFC 3493 S5.1 says int); valid values 0-255, or -1 for system default. */
#define SOCKET_IPV6_UNICAST_HOPS            2
/** Specify hop limit for outgoing multicast packets, as int16_t (RFC 3493 S5.2 says int); valid values 0-255, or -1 for system default. */
#define SOCKET_IPV6_MULTICAST_HOPS          3
/** Specify source address preferences, as uint32_t - see RFC 5014; valid values combinations of SOCKET_IPV6_PREFER_xxx flags. */
#define SOCKET_IPV6_ADDR_PREFERENCES        4
/** Specify PMTU preference, as int8_t (RFC 3542 S11.1 says int); valid values -1 (PMTUD for unicast, default), 0 (PMTUD always), 1 (PMTUD off). */
#define SOCKET_IPV6_USE_MIN_MTU             5
/** Specify not to fragment datagrams, as int8_t (RFC 3542 S11.2 says int); valid values 0 (fragment to path MTU, default), 1 (no fragmentation, TX fails if bigger than outgoing interface MTU). */
#define SOCKET_IPV6_DONTFRAG                6
/** Specify flow label for outgoing packets, as int32_t; valid values 0-0xfffff, or -1 for system default, or -2 to always autogenerate */
#define SOCKET_IPV6_FLOW_LABEL              7
/** Hop limit control for socket_sendmsg() and indicate for hop limit socket_recmsg(), as int16_t; valid values 0-255, -1 for default for outgoing packet */
#define SOCKET_IPV6_HOPLIMIT                8
/** Specify control messages packet info for send and receive as ns_in6_pktinfo_t socket_sendmsg() it define source address and outgoing interface. socket_recvmsg() it define messages destination address and arrival interface. Reference at RFC3542*/
#define SOCKET_IPV6_PKTINFO                 9

/** Specify socket_recvmsg() ancillary data request state for Packet info (destination address and interface id), as bool; valid values true write enabled, false write disabled */
#define SOCKET_IPV6_RECVPKTINFO             10
/** Specify socket_recvmsg() ancillary data request state for receive messages hop-limit, as bool; valid values true  write enabled, false information write disabled */
#define SOCKET_IPV6_RECVHOPLIMIT           11
/** Specify socket_recvmsg() ancillary data request state for receive messages traffic class, as bool; valid values true  write enabled, false information write disabled */
#define SOCKET_IPV6_RECVTCLASS             12

#define SOCKET_BROADCAST_PAN                0xfc /**< Internal use - transmit with IEEE 802.15.4 broadcast PAN ID */
#define SOCKET_LINK_LAYER_SECURITY          0xfd /**< Not standard enable or disable socket security at link layer (For 802.15.4). */
#define SOCKET_INTERFACE_SELECT             0xfe /**< Not standard socket interface ID. */
#define SOCKET_IPV6_ADDRESS_SELECT          0xff /**< Deprecated - use SOCKET_IPV6_ADDR_PREFERENCES instead. */

/** Socket options summary
 *
 * | opt_name / cmsg_type         | Data type        | set/getsockopt  | sendmsg | recvmsg                           |
 * | :--------------------------: | :--------------: | :-------------: | :-----: | :-------------------------------: |
 * | SOCKET_IPV6_TCLASS           | int16_t          |     Yes         |   Yes   | If enabled with RECVTCLASS        |
 * | SOCKET_IPV6_UNICAST_HOPS     | int16_t          |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_MULTICAST_HOPS   | int16_t          |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_ADDR_PREFERENCES | int              |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_USE_MIN_MTU      | int8_t           |     Yes         |   Yes   | No                                |
 * | SOCKET_IPV6_DONTFRAG         | int8_t           |     Yes         |   Yes   | No                                |
 * | SOCKET_IPV6_FLOW_LABEL       | int32_t          |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_HOPLIMIT         | int16_t          |     No          |   Yes   | If enabled with RECVHOPLIMIT      |
 * | SOCKET_IPV6_PKTINFO          | ns_in6_pktinfo_t |     No          |   Yes   | If enabled with RECVPKTINFO       |
 * | SOCKET_IPV6_RECVPKTINFO      | bool             |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_RECVHOPLIMIT     | bool             |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_RECVTCLASS       | bool             |     Yes         |   No    | No                                |
 * | SOCKET_BROADCAST_PAN         | int8_t           |     Yes         |   No    | No                                |
 * | SOCKET_LINK_LAYER_SECURITY   | int8_t           |     Yes         |   No    | No                                |
 * | SOCKET_INTERFACE_SELECT      | int8_t           |     Yes         |   No    | No                                |
 *
 *
 */

///@}

/**
 * \brief Set an option for a socket
 *
 * Used to specify miscellaneous options for a socket. Supported levels and
 * names defined above.
 *
 * \param socket The socket ID.
 * \param level The protocol level.
 * \param opt_name The option name (interpretation depends on level). See \ref OPTNAMES_IPV6.
 * \param opt_value A pointer to value for the specified option.
 * \param opt_len Size of the data pointed to by the value.
 *
 * \return 0 on success.
 * \return -1 invalid socket ID.
 * \return -2 invalid/unsupported option.
 * \return -3 invalid option value.
 */
int8_t socket_setsockopt(int8_t socket, uint8_t level, uint8_t opt_name, const void *opt_value, uint16_t opt_len);

/**
 * \brief Get an option for a socket.
 *
 * Used to read miscellaneous options for a socket. Supported levels and
 * names defined above. If the buffer is smaller than the option, the output
 * is silently truncated; otherwise opt_len is modified to indicate the actual
 * length.
 *
 * \param socket The socket ID.
 * \param level The protocol level.
 * \param opt_name The option name (interpretation depends on level). See \ref OPTNAMES_IPV6.
 * \param opt_value A pointer to output buffer.
 * \param opt_len A pointer to length of output buffer; updated on exit.
 *
 * \return 0 on success.
 * \return -1 invalid socket ID.
 * \return -2 invalid/unsupported option.
 */
int8_t socket_getsockopt(int8_t socket, uint8_t level, uint8_t opt_name, void *opt_value, uint16_t *opt_len);

#ifdef __cplusplus
}
#endif
#endif /*_NS_SOCKET_H*/
