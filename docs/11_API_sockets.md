Socket API
==============================
This chapter describes the socket use and the available APIs.

## API Headers

Socket API can be fully harnessed by including the following header files:

```
#include net_interface.h
#include socket_api.h
#include socket_security.h
```

## Socket types available

Sockets are a common abstraction model for network communication and are used in most Operating Systems (OS). The 6LoWPAN stack API follows the _Berkeley Software Distribution_ (BSD) socket API conventions closely with some extensions necessitated by the event-based scheduling model. The stack supports the socket types shown in _Table 3-18_.

**Table 3-18 Socket types**

Socket name|Socket description
-----------|------------------
`SOCKET_UDP`|UDP socket type.
`SOCKET_TCP`|TCP socket type.
`SOCKET_ICMP`|ICMP raw socket type; see section _ICMP socket instruction_.
`SOCKET_LOCAL`|Local application space socket.

### ICMP socket instruction

When using _Internet Control Message Protocol_ (ICMP) sockets, the minimum packet length is eight bytes where the first four bytes comprise the ICMP header, as described in _Table 3-19_. The stack will calculate the checksum
automatically before transmitting the packet.

**Table 3-19 General ICMP packet structure**

Type|Code|Checksum|Payload|Notes
----|----|--------|-------|-----
1|1|2|n (min. 4)|Length in bytes
`0xXX`|`0xXX`|`0x00 0x00`|n bytes|Transmit
`0xXX`|`0xXX`|`0xXX 0xXX`|n bytes|Receive


ICMP echo request with four bytes of payload (ping6), as shown in _Table 3-20_.

**Table 3-20 ICMP echo request**

Type|Code|Checksum|Payload
----|----|--------|-------
`0x80`|`0x00`|`0x00 0x00`|`0x00 0x01 0x02 0x03`

ICMP echo response for the message, as shown in _Table 6-4_.

**Table 3-21 ICMP echo response**

Type|Code|Checksum|Payload
----|----|--------|-------
`0x81`|`0x00`|`0xXX 0xXX`|`0x00 0x01 0x02 0x03`

## Receive callback structure

When there is data to read from the socket, a receive callback function is called from the stack with the socket event parameter. A socket event can be,for example, a change in the state of the _Transmission Control Protocol_ (TCP) socket and the socket _Transmit_ (TX) process is ready or the process failed `(SOCKET_TX_FAIL` or `SOCKET_TX_DONE)`. All supported socket event types are listed in _Table 3-22_. The receive callback function must be defined when a socket is opened using the `socket_open( )` API.

The socket call back structure, `socket_callback_t` is defined as below:

```
typedef struct socket_callback_t {
    uint8_t 	event_type;
    int8_t 		socket_id;
    int8_t 		interface_id;
    uint16_t  	d_len;
    uint8_t 	LINK_LQI;
} socket_callback_t;
```
where:

<dl>
<dt><code>event_type</code></dt>
<dd>Socket event type as provided in Table 3-22.</dd>

<dt><code>socket_id</code></dt>
<dd>ID of the socket that caused the event.</dd>

<dt><code>interface_id</code></dt>
<dd>Network interface ID from where the packet came.</dd>

<dt><code>d_len</code></dt>
<dd>Length of data readable from socket.

<dt><code>LINK_LQI</code></dt>
<dd>Link quality indicator value if the interface can provide any.</dd>
</dl>

**Table 3-22 Supported socket event types**

Event type|Description
----------|-----------
`SOCKET_DATA`|Data received.
`SOCKET_BIND_DONE`|TCP connection ready.
`SOCKET_BIND_FAIL`|TCP connection failed.
`SOCKET_BIND_AUTH_FAIL`|TCP connection authentication failed.
`SOCKET_SERVER_CONNECT_TO_CLIENT`|TCP connection state change from listen to establishment.
`SOCKET_TX_FAIL`|Socket data send failed.
`SOCKET_CONNECT_CLOSED`|TCP connection closed.
`SOCKET_CONNECT_FAIL_CLOSED`|TCP connection closed - no ACK received.
`SOCKET_NO_ROUTER`|No route available to destination.
`SOCKET_TX_DONE`|Last socket TX process done, in TCP case whole TCP process is ready.
`SOCKET_NO_RAM `|If no RAM is present.

An example parsing socket event:

```
#define APP_SOCK_RX_SIZE	250

/*Application socket payload buffer used for RX and TX*/
static uint8_t rx_buffer[APP_SOCK_RX_SIZE];

void main_receive
(
	void *cb
)
{
	socket_callback_t *cb_res = cb;
    int16_t length;

    if( cb_res->event_type == SOCKET_DATA )
    {
        sn_nsdl_addr_s sn_addr_s;

        //Read data from the RX buffer
        length = socket_read( cb_res->socket_id,
				&app_src,
				rx_buffer,
				APP_SOCK_RX_SIZE );

        if( length )
        {
            if( cb_res->socket_id == app_udp_socket )
            {
                // Handles data received in UDP socket

                sn_nsdl_process_coap(rx_buffer, length, &sn_addr_s);
            }

        }
    }
}
```

## Using TCP sockets

When a TCP socket is opened, it is in an unusable state and must be set to either a _listen_ or _connect_ state before it can be used to receive or transmit data.

The socket can be set to a _listen_ state by calling the `socket_listen( )` function. After the call, the socket can accept an incoming connection from a remote host. The TCP implementation of the 6LoWPAN stack supports only one connection from a remote host. The _listen_ state closes the connection automatically after a server timeout or when the client or application closes the connection manually by using the `socket_close( )` function.

The TCP socket can be connected to a remote host by calling `socket_connect( )` with the correct arguments. After the function call, an application (non-blocking) must await the socket event to confirm the successful state change of the socket.

After receiving a successful state event, data can be sent using the `socket_send( )` call. The connection can be closed by calling `socket_close( )` or with a server timeout.

## Using UDP and ICMP sockets

A _User Datagram Protocol_ (UDP) socket is ready to receive and send data immediately after a successful call to `socket_open( )` when the `ARM_NWK_BOOTSTRAP_READY` event is received. Data can then be transmitted using the
`socket_sendto( )` function call. The same function call can also be used for an ICMP socket.

## Detailed Socket API usage

This section describes the socket layer functions in more detail. Each function is presented with example parameters and possible return values.

### How to open a socket

To initialize a socket ready for communication, use the following function:

```
int8_t socket_open
(
	uint8_t		protocol,
	uint16_t	identifier,
	void		(*passed_fptr)(void *)
)
```

**Note**

A maximum of seven concurrent sockets is supported.

where:

<dl>
<dt><code>protocol</code></dt>
<dd>The protocol to be used over this socket and valid values for the argument are:</dd>
<dd><code>SOCKET_UDP</code> UDP: standard communication.</dd>
<dd><code>SOCKET_TCP</code> TCP: standard communication.</dd>
<dd><code>SOCKET_ICMP</code> ICMP: used for ping functionality.</dd>

<dt><code>identifier</code></dt>
<dd>The port identifier for UDP and TCP, or message ID for ICMP.</dd>

<dt><code>passed_fptr</code></dt>
<dd>A function pointer to the desired socket receive callback function.</dd>

<dt><code>Return value</code></dt>
<dd>0 Socket ID, used as a reference to the specific socket in subsequent calls.</dd>
<dd>-1 No free sockets or invalid protocol.</dd>
</dl>

### How to release a socket

To release a socket, use the following function:

`int8_t socket_free( int8_t socket )`

where:

<dl>
<dt><code>socket</code></dt>
<dd>The socket ID of the socket to be released.</dd>

<dt><code>Return value</code></dt>
<dd>0 Socket release successful.</dd>
<dd>-1 Socket release failed. Socket ID invalid or already released.</dd>
</dl>

### How to bind a socket

To bind socket to a port, use the following function:

**Note**

Binding to address is not supported, therefore `address` in the structure `ns_address_t` must be set to `ns_in6addr_any`.

```
int8_t socket_bind
(
	int8_t 				socket,
	const ns_address_t 	*address
)
```

where:

<dl>
<dt><code>socket</code></dt>
<dd>The socket ID returned by <code>socket_open</code>.</dd>

<dt><code>Return value</code></dt>
<dd>0 on success.</dd>
<dd>-1 if given address is NULL.</dd>
<dd>-2 if port is already bound to another socket.</dd>
<dd>-3 if trying to bind to port 0.</dd>
<dd>-4 if socket is already bound.</dd>
<dd>-5 if given address is not equal to <code>ns_in6addr_any</code>.</dd>
</dl>

### How to read data from a socket

To read received data from a socket, use the following function:

```
int16_t socket_read
(
	int8_t			socket,
	ns_address_t	*address,
	uint8_t			*buffer,
	uint16_t		length
)
```

where:
<dl>
<dt><code>socket</code></dt>
<dd>The socket ID of the socket to be read.</dd>

<dt><code>address</code></dt>
<dd>A pointer to an address structure containing the source address of the packet (populated by the stack).</dd>

<dt><code>buffer</code></dt>
<dd>A pointer to a byte array containing the payload of the packet.</dd>

<dt><code>length</code></dt>
<dd>The length of the payload data to be stored in the buffer.</dd>

<dt><code>Return value</code></dt>
<dd>>0 Length of the received data.</dd>
<dd>-1 Fail.</dd>
</dl>

### How to send data to a socket

To transmit data using a socket, the 6LoWPAN Stack offers two different functions depending on the transport layer protocol that is used, as shown in _Table 3-23_.

After successfully calling the function, the application must await the TX process to complete.

**Table 3-23 The two transmit function calls**

Function|Socket types
--------|------------
`socket_sendto( )`|UDP and ICMP
`socket_send( )`|TCP


_Table 3-24_ describes the possible response events when the outcome of the function call is successful.

**Table 3-24 The possible response events following a successful function call**

Response Event|Socket Type|Description
--------------|-----------|-----------
`SOCKET_TX_DONE`|TCP/UDP|UDP link layer TX ready/TCP TX process ready by TCP _Acknowledgement_ (ACK).
`SOCKET_TX_FAIL`|UDP|UDP link layer TX fails.
`SOCKET_CONNECT_FAIL_CLOSED`|TCP|TX process fails and connection closed.


To transmit UDP or raw ICMP data, use the following function:

```
int8_t socket_sendto
(
	int8_t			socket,
	ns_address_t	address,
	uint8_t			*buffer,
	uint16_t		length
)
```

where:

<dl>
<dt><code>socket</code></dt>
<dd>The socket ID to use for transmission.</dd>

<dt><code>address</code></dt>
<dd>A pointer to an address structure containing the destination address of the packet (populated by the application).</dd>

<dt><code>buffer</code></dt>
<dd>A pointer to a byte array containing the payload of the packet.</dd>

<dt><code>length</code></dt>
<dd>The length of the payload data in the buffer.</dd>

<dt><code>Return value</code></dt>
<dd>>0 Data packet accepted by the stack.</dd>
<dd>-1 Fail.</dd>
</dl>

To send data via a connected TCP socket, use the following function:

**Note**

A socket connection must be ready before using this function. The stack will automatically use the address of the remote connected host address as the destination for the packet.

```
int8_t socket_send
(
	int8_t		socket,
	uint8_t		*buffer,
	uint16_t	length
)
```

where:

<dl>
<dt><code>socket</code></dt>
<dd>The socket ID to use for transmission.</dd>

<dt><code>buffer</code></dt>
<dd>A pointer to a byte array containing the payload of the packet.</dd>

<dt><code>length</code></dt>
<dd>The length of the payload data in buffer.</dd>

<dt><code>Return value</code></dt>
<dd>>0 Data packet accepted by the stack.</dd>
<dd>-1 Fail.</dd>
</dl>

### TCP socket configuration

The TCP socket configuration API offers three function calls, as shown in _Table 3-25_ and are further described.

**Table 3-25 The TCP socket configuration functions**

Function|Description
--------|-----------
`socket_listen()`|Set socket to the listen state.
`socket_connect()`|Connect socket to a host.
`socket_close()`|Close socket connection.

To set a TCP socket into the listen state, use the following function:

```
int8_t socket_listen
(
	int8_t	socket

```
where:

<dl>
<dt><code>socket</code></dt>
<dd>The socket ID that is to be set to the listen state.</dd>

<dt><code>Return value</code></dt>
<dd>0 Valid request.</dd>
<dd><b>Note:</b> This does not imply that the state of the socket has been successfully changed.</dd>
<dd>-1 Fail.</dd>
</dl>

To connect a socket to a remote host, use the following function:

```
int8_t socket_connect
(
	int8_t			socket,
	ns_address_t	*address,
	uint8_t			randomly_take_src_numbers
)
```

where:

<dl>
<dt><code>socket</code></dt>
<dd>The socket ID, which is used to connect to the remote host.</dd>

<dt><code>address</code></dt>
<dd>A pointer to an <code>address_t</code> structure that contains the address of the remote host.</dd>

<dt><code>randomly_take_src_numbers</code></dt>
<dd>Value 1 indicates that a randomly selected source port number is used.</dd>

<dt><code>Return value</code></dt>
<dd>0 Valid request.</dd>
<dd><b>Note:</b>This does not imply that the state of the socket has been successfully changed.</dd>
<dd>-1 Fail.</dd>
</dl>

There are two possible responses from the stack for `socket_connect( )`:

<dl>
<dt><code>SOCKET_BIND_DONE</code></dt>
<dd>TCP handshake ready.</dd>

<dt><code>SOCKET_CONNECT_FAIL_CLOSED</code></dt>
<dd>TCP handshake fail.</dd>
</dl>

To close a TCP connection, use the following function:

```
int8_t socket_close

	int8_t			socket,
	ns_address_t	*address
)
```

where:

<dl>
<dt><code>socket</code></dt>
<dd>The socket ID of the socket to be disconnected from the remote host.</dd>

<dt><code>address</code></dt>
<dd>The destination client address; a client should use a null pointer for this parameter.</dd>

<dt><code>Return value</code></dt>
<dd>0 Valid request.</dd>
<dd><b>Note:</b>This does not imply that the state of the socket has been successfully changed.</dd>
<dd>-1 Fail.</dd>
</dl>

### Modifying socket options

To specify miscellaneous options for a socket, use the following function:

```
int8_t socket_setsockopt
(
	int8_t		socket,
	uint8_t		level,
	uint8_t		opt_name,
	const void	*opt_value,
	uint16_t	opt_len
)
```

where:

<dl>
<dt><code>socket</code></dt>
<dd>Socket ID.</dd>

<dt><code>level</code></dt>
<dd>Option level.</dd>

<dt><code>opt_name</code></dt>
<dd>Option name.</dd>

<dt><code>opt_value</code></dt>
<dd>A pointer to the value of the specified option.</dd>

<dt><code>opt_len</code></dt>
<dd>Size of the data pointed to by a value.</dd>

<dt><code>Return value</code></dt>
<dd>0 Done.</dd>
<dd>-1 Invalid socket ID.</dd>
<dd>-2 Invalid or unsupported option.</dd>
<dd>-3 Invalid option value.</dd>
</dl>

Each socket has unique control of the following items:

- IPv6 traffic class, option name: `SOCKET_IPV6_TCLASS`
- Socket source address mode, option name: `SOCKET_IPV6_ADDRESS_SELECT`

A socket uses a configured setup until the user changes it with a new function call.

**Note**

`SOCKET_IPV6_ADDRESS_SELECT` is only supported when the interface bootstrap address mode is `NET_6LOWPAN_MULTI_GP_ADDRESS`.

#### How to set address mode for a socket

A socket can be configured to use a primary or secondary address as the source address when transmitting packets using `socket_sendto( )`. The primary (default) address is an IPv6 address that is created using a short address type, whereas the secondary address, also IPv6 based, is created using a MAC address type. The source address is currently the only configurable parameter using `socket_configuration` and when multimode is not used, the source address configuration has no effect. The primary address is the default address when no configuration is needed.

```
int8_t set_coap_socket_src_address_mode(int16_t address_mode)
{

 return socket_setsockopt(app_udp_socket,SOCKET_IPPROTO_IPV6,
SOCKET_IPV6_ADDRESS_SELECT, &address_mode),sizeof(address_mode)));
}
```

### How to set traffic class for a socket

You can use `socket_setsockopt()` to set the socket traffic class. When this option is set, it will stay until modified. Therefore, if you want to set the class for one specific packet, you must call `socket_setsockopt()` again with a default traffic class after the packet has been sent.

Parameters for Traffic class:

<dl>
<dt><code>socket</code></dt>
<dd>The socket identified.</dd>

<dt><code>level</code></dt>
<dd><code>SOCKET_IPPROTO_IPV6</code></dd>

<dt><code>opt_name</code></dt>
<dd><code>SOCKET_IPV6_TCLASS</code></dd>

<dt><code>opt_value</code></dt>
<dd>A pointer to <code>int16_t</code> value. Valid values are from 0 to 255. -1 is for system default.</dd>

<dt><code>opt_len</code></dt>
<dd>Is the size of <code>int16_t</code>, 2 bytes.</dd>
</dl>

[RFC 4594](https://tools.ietf.org/html/rfc4594) specifies the appropriate traffic class values. The 6LoWPAN Stack does not interpret the specified traffic class. It is just passed through.

