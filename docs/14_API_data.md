Data Structures, Types and Variables
====================================
This chapter describes data structure, types and variables used in the socket and RF layers. It contains the following sections:

- [_Basic data types_](#basic-data-types)
- [_Socket address type definition_](#socket-address-type-definition)
- [_Address type_](#address-type)

## API Headers

```
#include ns_types.h
#include ns_addres.h
```

## Basic data types

The basic data types used are:

Data type|Description
---------|-----------
`uint8_t`|An unsigned 8-bit integer.
`int8_t`|A signed 8-bit integer.
`uint16_t`|An unsigned 16-bit integer.
`int16_t`|A signed 16-bit integer.
`uint32_t`|An unsigned 32-bit integer.
`int32_t`|A signed 32-bit integer.

## Socket address type definition

This structure defines the socket address type and has the following variables:

```
typedef struct ns_address_t
{
    address_type_t	type;
    uint8_t			address[16];
    uint16_t		identifier;
}ns_address_t;
```

where:

<dl>
<dt><code>type</code></dt>
<dd>The address type.</dd>

<dt><code>address</code></dt>
<dd>The address data in the format defined by type.</dd>

<dt><code>identifier</code></dt>
<dd>The port for TCP/UDP, message ID for ICMP.</dd>
</dl>


## Address type

This enumeration defines the address types:

```
typedef enum address_type_t
{
	ADDRESS_IPV6,
	ADDRESS_IPV4,
	ADDRESS_TUN_DRIVER_ID
}address_type_t;
```

where:

<dl>
<dt><code>ADDRESS_IPV6</code></dt>
<dd>IPv6 address type. The address is in 128-bit binary form.</dd>

<dt><code>ADDRESS_IPV4</code></dt>
<dd>IPv4 address type. The address is in 32-bit binary form.</dd>

<dt><code>ADDRESS_TUN_DRIVER_ID</code></dt>
<dd>Local socket address type. The address is an 8-bit driver ID for packet source.</dd>
</dl>

## Address type

This constant defines the IPv6 address INADDR_ANY:

```
extern const uint8_t ns_in6addr_any[16];
```

