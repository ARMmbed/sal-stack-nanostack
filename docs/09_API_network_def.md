Network API Definition
======================

This chapter describes the network API definitions. It contains the following sections:

- [_PANA network authentication API_](#pana-network-authentication-api)
- [_General security type definitions_](#general-security-type-definitions)
- [_Ethernet interface bootstrap definition_](#ethernet-interface-bootstrap-definition)
- [_RF 6LoWPAN interface configure definition_](#rf-6lowpan-interface-configure-definition)

## PANA network authentication API

This section introduces the PANA network authentication API for server and client mode.

PANA network authentication is an authentication layer for network access. It supports configurable authentication methods and security parameters. The server controls that only devices that passed the network authentication get access
to the network and receive security material for network layer encryption. The server also handles the key updates to nodes in coordinated way.

Supported TLS 1.2 cipher modes for client and server are listed in _Table 4-0_:

**Table 4-0 Supported TLS 1.2 cipher modes**

Cipher mode|Description
-----------|-----------
`NET_TLS_PSK_CIPHER`|PSK authentication.
`NET_TLS_ECC_CIPHER`|ECC authentication (ECC must be enabled).
`NET_TLS_PSK_AND_ECC_CIPHER`|PSK and ECC authentication (if ECC is not enabled, only PSK is used).

In PSK mode, the client and server must use the same PSK key. The PSK key must be loaded to the stack by `arm_tls_add_psk_key()` function. Unused keys can be removed by `arm_tls_remove_psk_key()` function.

In ECC mode, the Network authentication certificate must be loaded to the stack by `arm_network_certificate_chain_set()`.

### Client API

_Table 4-1_ introduces the client functions used.

**Table 4-1 Client functions**

Function|Description
--------|-----------
`arm_pana_client_library_init()`|Initializes the PANA Protocol for client mode with a given setup.
`arm_pana_client_key_pull()`|Manually pulls the key from the server. Only for testing purposes.

To initialize and configure the PANA network client interface, use the following function:

```
int8_t arm_pana_client_library_init
(
	int8_t nwk_interface_id,
	net_tls_cipher_e cipher_mode,
	uint32_t psk_key_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>cipher_mode</code></dt>
<dd>Defines the TLS 1.2 cipher mode PSK, ECC or both.</dd>

<dt><code>psk_key_id</code></dt>
<dd>PSK key ID for the PSK setup.</dd>

<dt><code>Return value:</code></dt>
<dd>0 Success.</dd>
<dd>-1 Unknown network ID or PANA is not supported in this stack.</dd>
<dd>-2 Interface active.</dd>
</dl>

To pull the PANA client key manually for testing purposes, use the following function:

```
int8_t arm_pana_client_key_pull
(
	int8_t nwk_interface_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>Return value:</code></dt>
<dd>0 Success.</dd>
<dd>-1 Unknown network ID.</dd>
</dl>

### Server API

_Table 4-2_ introduces the server functions used.

**Table 4-2 Server functions**

Function|Description
--------|-----------
`arm_pana_server_library_init()`|Initializes the PANA server mode and security material.
`arm_pana_server_key_update()`|Starts a key update process and key delivery to all authenticated nodes.
`arm_pana_activate_new_key()`|Triggers new key material quicker after key delivery phase. Only for testing purposes.
`arm_network_key_get()`|Reads PANA server key material. Only for testing purposes.

To initialize and configure the PANA network server interface, use the following function:

```
int8_t arm_pana_server_library_init
(
	int8_t nwk_interface_id,
	net_tls_cipher_e cipher_mode,
	uint8_t *key_material,
	uint32_t time_period_before_activate_key
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>cipher_mode</code></dt>
<dd>Defines the TLS 1.2 cipher mode PSK, ECC or both.</dd>

<dt><code>key_material</code></dt>
<dd>A pointer to 128-bit key material or <code>NULL</code> when the PANA server generates the random key.</dd>

<dt><code>time_period_before_activate_key</code></dt>
<dd>Guard period in seconds after a succesful key delivery phase before the key is activated by the server.</dd>

<dt><code>Return value:</code></dt>
<dd>0 Success.</dd>
<dd>-1 Unknown network ID.</dd>
<dd>-2 Interface active.</dd>
</dl>

To trigger a network key update process, use the following function. This function will deliver a new network key to all routers that have registered a GP address with the server. The function call always triggers a new key ID.
The key delivery is started in 300ms intervals between the nodes. This function does not cause any traffic if the server does not have any router device sessions.

```
int8_t arm_pana_server_key_update
(
	int8_t nwk_interface_id,
	uint8_t * network_key_material
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>network_key_material</code></dt>
<dd>A pointer to new 128-bit key material or a NULL pointer to a randomly generated key.</dd>

<dt><code>Return value:</code></dt>
<dd>0 Key update process is OK.</dd>
<dd>-1 PANA server is not yet initialized.</dd>
<dd>-2 Old key update is still active.</dd>
<dd>-3 Memory allocation fail.</dd>
</dl>

To activate the new key material quicker before a standard timeout, use the following function. This function is only used for testing purposes.

```
int8_t arm_pana_activate_new_key
(
	int8_t nwk_interface_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>Return value:</code></dt>
<dd>0 Key activate process is OK.</dd>
<dd>-1 No pending key update.</dd>
<dd>-2 PANA server is not initialized or the PANA server API is disabled by this stack.</dd>
</dl>

To read security key material of the PANA server, use the following function:

`previous_active_network_key` is information and only valid when `current_active_key_index` is bigger than 1.

```
int8_t arm_network_key_get
(
	int8_t interface_id, 	ns_keys_t *key
)
```

where:

<dl>
<dt><code>key</code></dt>
<dd>A pointer for storing key material information.</dd>

<dt><code>Return value:</code></dt>
<dd>0 Key read is OK.</dd>
<dd>-1 PANA server key material is not available.</dd>
</dl>

This structure defines the network keys used by the `net_network_key_get` function and comprises the following variables:

```
typedef struct ns_keys_t
{
	uint8_t previous_active_network_key[16];
	uint8_t previous_active_key_index;
	uint8_t current_active_network_key[16];
	uint8_t current_active_key_index;
} ns_keys_t;
```

where:

<dl>
<dt><code>previous_active_network_key</code></dt>
<dd>contains the previous key. The key is still valid for reception.</dd>

<dt><code>previous_active_key_index</code></dt>
<dd>contains the key index for the previous key.</dd>

<dt><code>current_active_network_key</code></dt>
<dd>contains the current key.</dd>

<dt><code>current_active_key_index</code></dt>
<dd>contains the index for the current key.</dd>
</dl>

### Resume API

The PANA Resume API allows the previously established PANA sessions to be resumed without going through a full authentication phase. This allows smaller low-power devices to join the network faster after the host or router is booted.

The resume functionality depends on the platform side to provide non-volatile storage space that is used for storing PANA session data. The storage is used for keeping the data when the device is powered off or rebooted.
The data contains authentication keys so it must be kept secure. If no such secure storage can be provided, ARM recommends not to use the resume functionality because it may leak the authentication keys.

The resume functionality is divided into two parts, the client side API and the server side API. Both sides require the application to set a callback that receives new material when there are updates in the network. The client side callback receives key material immediately after a successful authentication and periodically after each network key update. On the server side, the callback receives an update when a new client joins the network or when network
keys are updated.

Security material is basically divided into two parts, session keys and network keys. Each client session has unique authentication keys that are assigned to only one session. After a successful authentication, the client
receives network keys that are shared with the whole network. The network keys are used for encryption and decryption for all clients. Therefore, a session resume requires you to store both keys.

#### Client side PANA resume API

To set a callback for storing PANA key material, use the following function. Before setting this callback, the application should allocate a buffer for transferring keys to the callback.

```
int8_t pana_client_nvm_callback_set
(
	void (*passed_fptr)(pana_client_nvm_update_process_t),
	uint8_t * nvm_static_buffer
);
```

where:

<dl>
<dt><code>passed_fptr</code></dt>
<dd>A pointer to the callback function.</dd>

<dt><code>nvm_static_buffer</code></dt>
<dd>A pointer to the allocated buffer. The required size is 86 bytes.</dd>

<dt><code>Return value:</code></dt>
<dd>0 Initialization is OK.</dd>
<dd>-1 Failure.</dd>
</dl>

The callback function is called whenever the stack acquires new keys. The parameter passed to the callback contains information about which parts of the keys are updated. This enumeration defines the parameter types:

```
typedef enum pana_client_nvm_update_process_t
{
       PANA_CLIENT_SESSION_UPDATE,
       PANA_CLIENT_SESSION_SEQ_UPDATE,
}pana_client_nvm_update_process_t;
```

where:

<dl>
<dt><code>PANA_CLIENT_SESSION_UPDATE</code></dt>
<dd>is a full update of PANA session information.</dd>

<dt><code>PANA_CLIENT_SESSION_SEQ_UPDATE</code></dt>
<dd>is an update of the REQ and RES sequence number of a PANA key pull or push operation.</dd>
</dl>

When a callback takes place with the parameter `PANA_CLIENT_SESSION_UPDATE`, the first 16 bytes in the data buffer are PANA session address and the following 70 bytes PANA session keys. The session address is associated with
the node's parent address, so that is most probably required only on star topology networks. On the mesh network, the parent address can change, so storing the session address is unnecessary.

When multiple sessions are supported, the session address identifies each session. All sessions are then stored separately.

An example of a session saving functionality when the session address is not stored:

```
#define PANA_SESSION_ADDRESS_SIZE 16
#define PANA_SESSION_KEY_SIZE 70
void pana_resume_callback(pana_client_nvm_update_process_t event)
{
       if (PANA_CLIENT_SESSION_UPDATE == event) {
             // store now the Session keys, skip the session address
             write_to_storage(nvm_static_buffer+PANA_SESSION_ADDRESS_SIZE, PANA_SESSION_KEY_SIZE);
       }
}
```

To resume the PANA session, use the following function:

```
int8_t net_pana_client_session_nvm_data_load
(
	uint8_t *data_buffer,
	uint8_t *session_address,
	int8_t interface_id
);
```

where:

<dl>
<dt><code>data_buffer</code></dt>
<dd>A pointer to the PANA session keys.</dd>

<dt><code>session_address</code></dt>
<dd>A pointer to the PANA session address, or NULL if no session address information is stored.</dd>

<dt><code>interface_id</code></dt>
<dd>The interface ID for a 6LoWPAN interface.</dd>
</dl>

When you give a `NULL` pointer to `session_address`, the function only restores the PANA keys and can then associate with any parent from the same network. This functionality is required on the mesh topology.

#### Server side API

The server side API differs from the client side so that it stores multiple PANA sessions. This adds some complexity to storing of keys.

The first step is to allocate a buffer that is used to transfer keys to the callback function. The buffer size must be 116 bytes and is symbolically defined as `PANA_SERVER_CLIENT_NVM_SESSION_BUF_SIZE`.

The next step is to set a PANA key update callback that receives the key material:

```
int8_t pana_server_nvm_callback_set
(
	uint16_t (*passed_fptr)(pana_nvm_update_process_t),
	uint8_t * nvm_static_buffer
);
```

where:

<dl>
<dt><code>passed_fptr</code></dt>
<dd>A pointer to the callback function.</dd>

<dt><code>nvm_static_buffer</code></dt>
<dd>A pointer to the buffer used to store the PANA keys.</dd>

<dt><code>Return value:</code></dt>
<dd>0 Success.</dd>
<dd>-1 Error.</dd>
</dl>

The following enumeration defines the parameters to the callback:

```
typedef enum pana_nvm_update_process_t
{
       PANA_SERVER_MATERIAL_UPDATE,
       PANA_SERVER_CLIENT_SESSION_UPDATE,
       PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE,
       PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE,
}pana_nvm_update_process_t;
```

where:

<dl>
<dt><code>PANA_SERVER_MATERIAL_UPDATE</code></dt>
<dd>is an update of the PANA server security material.</dd>

<dt><code>PANA_SERVER_CLIENT_SESSION_UPDATE</code></dt>
<dd>is an update of the PANA client session.</dd>

<dt><code>PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE</code></dt>
<dd>is a sequence number update of the PANA client session.</dd>

<dt><code>PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE</code></dt>
<dd><code>Pana Client Session remove</code></dd>
<dd>removes the PANA client session.</dd>
</dl>

The buffer that is used to transfer data from the PANA process to the storage can contain server keys or client session data. When the server keys are stored, the buffer size is 90 bytes. On the client sessions, it depends
on the event that the buffer contains.

When client session data is stored, the buffer is divided into the following parts:

1. Session addresses, 20 bytes, containing the following parts:
	1. 16 bytes offset, 2 bytes.
	2. Client session IPv6 address, 16 bytes.
	3. Client port number, 2 bytes.
2. PANA client session data, 33 bytes.
3. PANA client session private keys, 63 bytes.

Not all segments are valid by each call. The parameter that is passed to the callback function defines the parts that are valid. _Table 4-3_ defines the segments.

**Table 4-3 Callback parameters**

<table>
   <tr>
      <td><b>Callback parameter</b></td>
      <td><b>Buffer content</b></td>
   </tr>
   <tr>
      <td><code>PANA_SERVER_MATERIAL_UPDATE</code></td>
      <td>Buffer contains server key material. The size is defined by the symbol <code>PANA_SERVER_MATERIAL_BUF_SIZE.</code></td>
   </tr>
   <tr>
	<td><code>PANA_SERVER_CLIENT_SESSION_UPDATE</code></td>
	<td>Add a new PANA session or update the previously stored one. All three segments are valid.<br> An offset number is used to determine which session this is. The offset number for new sessions is zero.<br>
The callback should return the offset number for the sessions; this information is not used by the stack but is provided back on the following update.</td>
   </tr>
   <tr>
	<td><code>PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE</code></td>
	<td>Update only the client session data. The first two segments are stored in the buffer.<br>
The callback should use the offset field to determine which session data this is.</td>
   </tr>
   <tr>
	<td><code>PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE</code></td>
	<td>Remove the previously stored session. Only the first segment is stored in the buffer.<br>
The callback should use the offset field to determine which session data this is.</td>
   </tr>
</table>

The following example shows the basic functionality of the PANA server callback:

```
uint16_t app_nvm_pana_update(pana_nvm_update_process_t update_type )
{
       uint16_t ret_val = 0;
       uint16_t offset;

       switch(update_type) {
       case PANA_SERVER_MATERIAL_UPDATE:
             store_server_key(nvm_static_buffer, PANA_SERVER_MATERIAL_BUF_SIZE);
             return 0;
       case PANA_SERVER_CLIENT_SESSION_UPDATE:
             // Store a new session, or update full session record.
             offset = *((uint16_t*)nvm_static_buffer);
             if (0 == offset)
                    return store_new_session(nvm_static_buffer);
             else
                    return update_session(offset, nvm_static_buffer);
       case PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE:
             // Update session data, but does not touch keys
             offset = *((uint16_t*)nvm_static_buffer);
             return update_session_data(offset, nvm_static_buffer);
       case PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE:
              // Remove previously stored session
             offset = *((uint16_t*)nvm_static_buffer);
             return remove_session(offset);
       }
}
```
**Note**

The previous example assumes that the user provides the functions `store_server_key()`, `store_new_session()`, `update_session()`, `update_session_data()` and `remove_session()`.

When the server starts, it uses the following API to restore the previous server keys:

```
int8_t pana_server_restore_from_nvm
(
	uint8_t * nvm_data,
	int8_t interface_id
);
```

where:

<dl>
<dt><code>nvm_data</code></dt>
<dd>Full buffer content given to the callback previously.</dd>

<dt><code>interface_id</code></dt>
<dd>The 6LoWPAN interface ID.</dd>

<dt><code>Return value:</code></dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

After restoring the server data, each session previously stored must be restored:

```
int8_t pana_server_nvm_client_session_load
(
	uint8_t *nvm_pointer
);
```

where:

<dl>
<dt><code>nvm_pointer</code></dt>
<dd>A pointer to the full PANA session record containing all three defined fields.</dd>

<dt><code>Return value:</code></dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

## General security type definitions

This section introduces general security type definitions.

### Certificate structure

The certificate structure comprises the following variables:

```
typedef struct arm_certificate_chain_entry_s
{
	uint8_t chain_length;
	const uint8_t *cert_chain[4];
	uint16_t cert_len[4];
	const uint8_t *key_chain[4];
} arm_certificate_chain_entry_t;
```

where:

<dl>
<dt><code>chain_length</code></dt>
<dd>defines the length of the certificate chain.</dd>

<dt><code>cert_chain</code></dt>
<dd>defines the pointers to the certificates in chain.</dd>

<dt><code>cert_len</code></dt>
<dd>defines the certificate lengths.</dd>

<dt><code>key_chain</code></dt>
<dd>defines the private keys.</dd>
</dl>


### TLS cipher mode structure

This enumeration defines the TLS cipher modes:

```
typedef enum net_tls_cipher_e
{
	NET_TLS_PSK_CIPHER,
	NET_TLS_ECC_CIPHER,
	NET_TLS_PSK_AND_ECC_CIPHER,
} net_tls_cipher_e;
```

where:

<dl>
<dt><code>NET_TLS_PSK_CIPHER</code></dt>
<dd>means that network authentication only supports PSK.</dd>

<dt><code>NET_TLS_ECC_CIPHER</code></dt>
<dd>means that network authentication only supports ECC.</dd>

<dt><code>NET_TLS_PSK_AND_ECC_CIPHER</code></dt>
<dd>means that network authentication supports both PSK and ECC.</dd>
</dl>

### TLS PSK info structure

The TLS PSK info structure comprises the following variables:

```
typedef struct net_tls_psk_info_s
{
	uint32_t key_id;
	uint8_t key[16];
} net_tls_psk_info_s;
```

where:

<dl>
<dt><code>key_id</code></dt>
<dd>means that a PSK key ID can be 0x01-0xFFFF. The storage size is intentionally 32 bits.</dd>

<dt><code>key</code></dt>
<dd>defines a 128-bit PSK key.</dd>
</dl>

The 6LoWPAN stack supports two different chain certificate users:

- Transport Layer Security (TLS).
- Network authentication (PANA, EAP or TLS).


## Ethernet interface bootstrap definition

This section defines the Ethernet interface bootstrap.

### IPv6

This enumeration defines the IPv6 bootstrap:

```
typedef enum net_ipv6_mode_e
{
	NET_IPV6_BOOTSTRAP_STATIC
} net_ipv6_mode_e;
```

where:

<dl>
<dt><code>NET_IPV6_BOOTSTRAP_STATIC</code></dt>
<dd>means that the application defines the IPv6 prefix.</dd>
</dl>


## RF 6LoWPAN interface configure definition

This section defines the RF 6LoWPAN interface configuration.

### typedef enum net_6lowpan_mode_e

This enumeration defines the different 6LoWPAN bootstrap or device modes:

```
typedef enum net_6lowpan_mode_e
{
	NET_6LOWPAN_BORDER_ROUTER,
	NET_6LOWPAN_ROUTER,
	NET_6LOWPAN_HOST,
	NET_6LOWPAN_SLEEPY_HOST
} net_6lowpan_mode_e;
```

where:

<dl>
<dt><code>NET_6LOWPAN_BORDER_ROUTER</code></dt>
<dd>is a root device for 6LoWPAN ND.</dd>

<dt><code>NET_6LOWPAN_ROUTER</code></dt>
<dd>is a router device.</dd>

<dt><code>NET_6LOWPAN_HOST</code></dt>
<dd>is a host device. This is the default setting.</dd>

<dt><code>NET_6LOWPAN_SLEEPY_HOST</code></dt>
<dd>is a sleepy host device.</dd>
</dl>

The `NET_6LOWPAN_SLEEPY_HOST` mode support requires MLE protocol support.

### typedef enum net_host_mode_e

This enumeration defines the sleepy host poll modes:

```
typedef enum net_host_mode_e
{
	NET_HOST_FAST_POLL_MODE,
	NET_HOST_SLOW_POLL_MODE,
	NET_HOST_RX_ON_IDLE_MODE,
	NET_HOST_MODE_NOT_ACTIVE,
} net_host_mode_e;
```

where:

<dl>
<dt><code>NET_HOST_FAST_POLL_MODE</code></dt>
<dd>means that the sleepy host is polling fast. The maximum polling period is 400ms.</dd>

<dt><code>NET_HOST_SLOW_POLL_MODE</code></dt>
<dd>means that the sleepy host is polling slowly. The user-defined maximum polling period is in seconds (up to 1 day).</dd>

<dt><code>NET_HOST_RX_ON_IDLE_MODE</code></dt>
<dd>means that the sleepy host has changed to host. The parent can use Direct Data Send and the host does not poll.</dd>

<dt><code>NET_HOST_MODE_NOT_ACTIVE</code></dt>
<dd>means that the host mode is not active (it responds to read).</dd>
</dl>

### typedef enum net_6lowpan_gp_address_mode_e

This enumeration defines the different addressing modes for a network interface. This setting is specific to each interface and is only applicable to a 6LoWPAN interface.

```
typedef enum net_6lowpan_gp_address_mode_e
{
	NET_6LOWPAN_GP64_ADDRESS,
	NET_6LOWPAN_GP16_ADDRESS,
	NET_6LOWPAN_MULTI_GP_ADDRESS,
} net_6lowpan_gp_address_mode_e;
```

where:

<dl>
<dt><code>NET_6LOWPAN_GP64_ADDRESS</code></dt>
<dd>means that the interface will only register a GP64 address. This is the default setting.</dd>

<dt><code>NET_6LOWPAN_GP16_ADDRESS</code></dt>
<dd>means that the interface will only register a GP16 address.</dd>

<dt><code>NET_6LOWPAN_MULTI_GP_ADDRESS</code></dt>
<dd>means that the interface will register both GP16 and GP64 addresses.</dd>
</dl>

The default address mode is `NET_6LOWPAN_GP64_ADDRESS`.

### typedef enum net_6lowpan_link_layer_sec_mode_e

This enumeration defines the security mode for an interface. This setting is specific to each interface and is only applicable to 6LoWPAN interfaces.

```
typedef enum net_6lowpan_link_layer_sec_mode_e
{
	NET_SEC_MODE_NO_LINK_SECURITY,
	NET_SEC_MODE_PSK_LINK_SECURITY,
	NET_SEC_MODE_PANA_LINK_SECURITY,
} net_6lowpan_link_layer_sec_mode_e;
```

where:

<dl>
<dt><code>NET_SEC_MODE_NO_LINK_SECURITY</code></dt>
<dd>means that the security is disabled at a link layer. This is the default setting.</dd>

<dt><code>NET_SEC_MODE_PSK_LINK_SECURITY</code></dt>
<dd>means that a PSK key defines the link security.</dd>

<dt><code>NET_SEC_MODE_PANA_LINK_SECURITY</code></dt>
<dd>means that PANA network authentication defines the link key. The client must call <code>arm_pana_client_library_init()</code> and the border router application <code>arm_pana_server_library_init()</code> to initialize the PANA Protocol. The PANA Network API could be disabled by some of the stack packets and it is also possible that it only supports client mode.</dd>
</dl>

The default setting is `NET_SEC_MODE_NO_SECURITY`.

### typedef struct net_link_layer_psk_security_info_s

This structure defines the PSK security information and comprises the following variables:

```
typedef struct net_link_layer_psk_security_info_s
{
	uint8_t key_id;
	uint8_t security_key[16];
} net_link_layer_psk_security_info_s;
```

where:

<dl>
<dt><code>key_id</code></dt>
<dd>PSK key ID of a link layer. Can be <code>0x01-0xFF</code>.</dd>

<dt><code>security_key</code></dt>
<dd>defines the 128-bit PSK key of a link layer.</dd>
</dl>

### typedef struct border_router_setup_t
This structure defines the information required to set up a 6LoWPAN border router and comprises the following variables:

```
typedef struct border_router_setup_t
{
	uint8_t channel;
	uint16_t mac_panid;
	uint8_t beacon_protocol_id;
	uint8_t network_id[16];
	uint8_t lowpan_nd_prefix[8];
	uint16_t ra_life_time;
	uint16_t abro_version_num;
} border_router_setup_t;
```

where:

<dl>
<dt><code>channel</code></dt>
<dd>defines the channel used in 802.15.4 radio. Supported values are from 1 to 26.</dd>

<dt><code>mac_panid</code></dt>
<dd>PAN ID of a link layer. Accepted values are from <code>0</code> to <code>0xFFFD</code>.</dd>

<dt><code>beacon_protocol_id</code></dt>
<dd>Beacon protocol ID. ZigBee reserves the values from `0` to `2`. 6LoWPAN does not define any value for this, you can use any non-reserved value.</dd>

<dt><code>network_id</code></dt>
<dd>A 16-byte long network ID. Used in the beacon payload.</dd>

<dt><code>lowpan_nd_prefix</code></dt>
<dd>defines the ND default prefix, ABRO, DODAG ID, and GP address.</dd>

<dt><code>ra_life_time</code></dt>
<dd>defines the ND router lifetime in seconds. ARM recommends value 180+.</dd>

<dt><code>abro_version_num</code></dt>
<dd>defines the ND ABRO version number (0 when starting a new ND setup).</dd>
</dl>


