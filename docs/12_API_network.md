The Network Layer Control APIs
=========================

This chapter describes the functions of the network control layer. It contains the following sections:

- [_Network control API_](#network-control-api)
- [_6LoWPAN MAC layer configuration API_](#6lowpan-mac-layer-configuration-api)
- [_Network start and stop control API_](#network-start-and-stop-control-api)
- [_Interface address mode API_](#interface-address-mode-api)
- [_Border router interface API_](#border-router-interface-api)
- [_Network sleep control API_](#network-sleep-control-api)
- [_RPL structures and definitions API_](#rpl-structures-and-definitions-api)
- [_RPL root configuration API_](#rpl-root-configuration-api)
- [_NET address retrieval API_](#net-address-retrieval-api)
- [_MLE router and host lifetime API_](#mle-router-and-host-lifetime-configuration)
- [_6LoWPAN ND configuration API_](#6lowpan-nd-configuration-api)
- [_PANA configuration API_](#pana-configuration-api)
- [_Network Information retrieval API_](#network-information-retrieval-api)
- [_Multicast API_](#multicast-api)

## API Headers

To use the Network Layer Control APIs, include the following headers:

```
#include net_interface.h
#include net_pana_parameters_api.h
#include net_6lowpan_parameter_api.h
#include net_lowpan_opt.h
#include net_nwk_scan.h
#include net_rpl.h
#include net_sleep.h
#include net_polling.h
#include multicast_api.h
```
## Network control API

This section describes the functions of the network control layer where each function is presented with its valid parameters.

The 6LoWPAN Stack contains the features and related functions shown in _Table 3-13_.

**Table 3-13 The network control API features and related functions**

Feature|API function
-------|------------
Network interface creation|`arm_nwk_interface_init()`
Network interface configuration|`arm_nwk_interface_configure()`
Network interface start, operation mode setting, and interface-specific bootstrap start|`arm_nwk_interface_up()`
Network interface stop|`arm_nwk_interface_down()`

### Interface enable

To set up the configured network interface and enable the interface-specific bootstrap, use the following function:

```
int8_t arm_nwk_interface_up
(
	int8_t nwk_interface_id
)
```

where:
<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The interface is set up OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 Already in up state.</dd>
<dd>-3 No configured IP stack at the interface (Ethernet and WiFi can return).</dd>
</dl>

### Interface disable

To stop and set the interface to idle, use the following function:

```
int8_t arm_nwk_interface_down
(
	int8_t nwk_interface_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The process is OK.</dt>
<dd>-1 An unknown network ID.</dt>
<dd>-2 Not active.</dt>
</dl>


## 6LoWPAN MAC layer configuration API

Use the 6LoWPAN MAC layer API to configure:

- The network security mode for an interface:
	* No security.
	* PANA network authentication.
	* PSK network authentication.
- The channel mask and scan time for an active scan.
- Node interface filters:
	* Beacon protocol ID.
	* PAN ID.
	* Network ID.
- Node interface address mode:
	* Use only GP64.
	* Use only GP16.
	* Support GP16 and GP64.
- Host poll control (requires MLE support):
	* Deep sleep enable.
	* Set the poll mode.
	* Read mode.

You must configure these before calling the `arm_nwk_interface_up()` function.

Starting the network with security enabled, but without having set the security level and key, will result in an error. The stack will store the 6LoWPAN MAC configuration after the first call and will use the existing configuration until it is explicitly changed by the application, or if the device is power-cycled.

### Scan channel list (2.4GHz only)

The configuration API for MAC scan channel list uses the `arm_nwk_6lowpan_link_scan_paramameter_set` function. The channel list is set up using a 32-bit variable where 27 bits are used to set the channels to be scanned, as described in the [IEEE 802.15.4-2011 standard](https://standards.ieee.org/findstds/standard/802.15.4-2011.html).

```
int8_t arm_nwk_6lowpan_link_scan_paramameter_set
(
	int8_t 		nwk_interface_id,
	uint32_t 	channel_mask,
	uint8_t 	scan_time
)
```
where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>channel_mask</code></dt>
<dd>Value 0-0xFFFFFFFF to determine which channels to scan.</dd>

<dt><code>scan_time</code></dt>
<dd>Time in seconds to find a network</dd>

<dt><code>Return value</code></dt>
<dd>0 Success.</dd>
<dd>-1 Unknown interface ID.</dd>
<dd>-3 Too long scan time.</dd>
<dd>-4 Interface is not active.</dd>
</dl>

The default value is `0x07FFF800` which scans channels 11 to 26. For example, to scan only channels 11 and 26, the value would be `0x04000800`. The exact function call would then be as follows:

`int8_t arm_nwk_6lowpan_link_scan_paramameter_set(12, 0x04000800, 3)`

**Note**

If the channel list has not been set by the `arm_nwk_6lowpan_link_scan_paramameter_set` function, the default list shall contain all channels.


### Beacon protocol ID filter

During an active 802.15.4 MAC scan, the 6LoWPAN Stack transmits MAC beacon requests to the pre-defined channels. If it receives a beacon, it will filter the beacon by using the protocol ID (this filter is disabled by default).

To set the protocol ID filter, use the following function:

```
int8_t arm_nwk_6lowpan_link_protocol_id_filter_for_nwk_scan
(
	int8_t 	nwk_interface_id,
	uin8_t 	protocol_ID
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>protocol_ID</code></dt>
<dd>The value 0-0xFE enables the filter for a specific ID.</dd>
<dd>The value 0xFF (default) disables the filter.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The link layer security of the interface is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
</dl>

The stack will respond to the application with `ARM_NWK_SCAN_FAIL`, if no valid networks are found.

### PAN ID filter

To set a PAN ID filter of a configured network interface for a network scan, use the following function:

```
int8_t arm_nwk_6lowpan_link_panid_filter_for_nwk_scan
(
	int8_t 	nwk_interface_id,
	uin16_t pan_id_filter
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>pan_id_filter</code></dt>

<dd>Values <code>0x0000-0xFFFD</code> enable the filter for a specific ID.</dd>
<dd>The value <code>0xFFFE</code> or <code>0xFFFF</code> (default) disables the  filter.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The link layer security of the interface is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
</dl>

### Network ID filter

To set a network ID filter of a configured network interface for a network scan, use the following function:

```
int8_t arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan
(
	int8_t nwk_interface_id,
	uin8_t *nwk_id_filter
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>nwk_id_filter</code></dt>

<dd>A pointer to a 16-byte array used to filter network.</dd>
<dd>The value NULL (default) disables the filter.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The link layer security of the interface is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
</dl>

### Security API

This section describes the functions of the security API that you can use to:

- Cipher PANA and set PSK (for a server and a client).
- Set a TLS PSK key and certificate.
- Initialize a certificate chain:
	* Network.
	* Transport layer.
- Update the PANA key.
- Set a timer period for the key trigger after delivery.
- Activate a new key before timer triggering.


#### Set TLS PSK key

To set a PSK key to the TLS module, use the following function:

```
int8_t arm_tls_add_psk_key
(
	uint8_t *key_ptr,
	uint16_t key_id
)
```

where:

<dl>
<dt><code>key_ptr</code></dt>
<dd>A pointer to a 16-byte long PSK key.</dd>

<dt><code>key_id</code></dt>
<dd>ID for the PSK key.</dd>

<dt><code>Return value</code></dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

#### Remove TLS PSK key

To remove the PSK key from the TLS module, use the following function:

```
int8_t arm_tls_remove_psk_key
(
	uint16_t key_id
)
```

where:
<dl>
<dt><code>key_id</code></dt>
<dd>ID for the PSK key.
<dt><code>Return value</code></dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

#### Set certificate chain for network

To set a certificate chain for network authentication, use the following function:

```
int8_t arm_network_certificate_chain_set
(
	arm_certificate_chain_entry_t *chain_info
)
```

where:

<dl>
<dt><code>chain_info</code></dt>
<dd>A pointer to a certificate chain.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The certificate chain register is OK.</dd>
<dd>-1 TLS is not supported in this stack.</dd>
<dd>-2 A NULL pointer parameter.</dd>
</dl>


## Network start and stop control API

This section specifies how to start and stop the network state of the stack.

After configuring the network interface, start the network using the `arm_nwk_interface_up()` function that starts the 6LoWPAN bootstrap process:

- An Edge router in use is required.
- Mesh network in use.
- Full RPL mesh routing is available.
- Node derives GP64 or GP16.
- Router or host mode is available.
- Link layer security is supported.

### 6LoWPAN bootstrap configure

To configure the 6LoWPAN bootstrap, use the following function:

```
int8_t arm_nwk_interface_configure_6lowpan_bootstrap_set
(
	int8_t				nwk_interface_id,
	net_6lowpan_mode_e	bootstrap_mode,
	uint8_t				enable_mle_protocol
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>bootstrap_mode</code></dt>
<dd>Values to start the bootstrap mode:</dd>
<dd><code>NET_6LOWPAN_BORDER_ROUTER</code> initializes a basic setup for the border router.</dd>
<dd><code>NET_6LOWPAN_ROUTER</code> enables normal 6LoWPAN ND and RPL to bootstrap.</dd>
<dd><code>NET_6LOWPAN_HOST</code> enables normal 6LoWPAN ND only to bootstrap.</dd>
<dd><code>NET_6LOWPAN_SLEEPY_HOST</code> enables normal 6LoWPAN ND only to bootstrap.</dd>

<dt><code>enable_mle_protocol</code></dt>
<dd>The value <code>0x01</code> enables the MLE protocol.</dd>
<dd>The value <code>0x00</code> disables the MLE for bootstrap.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The bootstrap mode is set OK.</dd>
<dd>-1 An unknown network ID.</dd>
<dd>-2 An unsupported bootstrap type in this stack.</dd>
<dd>-3 No memory for a 6LoWPAN Stack.</dd>
<dd>-4 A <code>NULL</code> pointer parameter.</dd>
</dl>


The `arm_nwk_interface_up()` function call starts the stack bootstrap process. The bootstrap process performs 802.15.4 MAC beacon active scanning on the selected channel list, 6LoWPAN ND, and RPL router discovery.

If the application wants to save power and enter sleep, the network mode should use `NET_6LOWPAN_HOST` or `NET_6LOWPAN_SLEEPY_HOST`; sleepy host mode uses indirect transmission. When the device awakens, it uses polling data from its parent. A host device does not route packets so it can continue to sleep. Router devices can, likewise, enter sleep; however, the network topology will become unstable for the duration while the device remains in sleep.

All configuration calls (such as channel selection) must be performed before calling `arm_nwk_interface_up()`.

**Note**

After successfully calling the `arm_nwk_interface_up()` function, the application must wait for the `ARM_LIB_NWK_INTERFACE_EVENT` status event from the stack before proceeding to perform another communications stack related function.

### IPv6 bootstrap configure

To configure the IPv6 bootstrap, use the following function:

```
int8_t arm_nwk_interface_configure_ipv6_bootstrap_set
(
	int8_t 			nwk_interface_id,
	net_ipv6_mode_e bootstrap_mode,
	uint8_t 		*ipv6_prefix_pointer
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>bootstrap_mode</code></dt>
<dd>Values to start the bootstrap mode:</dd>
<dd><code>NET_IPV6_BOOTSTRAP_STATIC</code> application defines the IPv6 prefix.</dd>

<dt><code>ipv6_prefix_pointer</code></dt>
<dd>A pointer to a 64-bit IPv6 prefix.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The bootstrap mode is set OK.</dd>
<dd>-1 An unknown network ID.</dd>
</dl>


The `arm_nwk_interface_up()` function call starts the stack bootstrap process and performs neighbour discovery.

### Security mode for RF link layer

To set a security mode for the link layer of a configured network interface, use the following function:

```
int8_t arm_nwk_link_layer_security_mode
(
	int8_t 					nwk_interface_id,
	net_6lowpan_link_layer_sec_mode_e  mode,
	uint8_t 				sec_level,
	net_link_layer_psk_security_info_s *psk_key_info
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>mode</code></dt>
<dd>Defines the security mode for the link layer:</dd>
<dd><code>NET_SEC_MODE_NO_SECURITY</code>, security is disabled.</dd>
<dd><code>NET_SEC_MODE_PSK_LINK_SECURITY</code>, security is enabled with selected mode and PSK key info.</dd>
<dd><code>NET_SEC_MODE_PANA_LINK_SECURITY</code>, link layer keys are defined by the PANA authentication server.</dd>

<dt><code>sec_level</code></dt>
<dd>Supported values are 1 to 7. This parameter is only checked when the mode is <code>NET_SEC_MODE_NO_SECURITY</code>.</dd>

<dt><code>psk_key_info</code></dt>
<dd>A pointer to PSK key material: key and key ID. This parameter is only checked when the mode is <code>NET_SEC_MODE_PSK_LINK_SECURITY</code>.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The link layer security of the interface is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 Unsupported PANA authentication in this stack.</dd>
<dd>-3 Unsupported security level.</dd>
<dd>-4 An unsupported key ID from the PSK info.</dd>
<dd>-5 A <code>NULL</code> pointer psk_key_info parameter.</dd>
</dl>

## Interface address mode API

To set the global address mode for a configured network interface, use the following function:

```
int8_t arm_nwk_6lowpan_gp_address_mode
(
	int8_t 					nwk_interface_id,
	net_6lowpan_gp_address_mode_e 	mode,
	uint16_t 				short_address_base,
	uint8_t 				define_new_short_address_at_DAD
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>mode</code></dt>
<dd>Defines the register mode of the 6LoWPAN global address:</dd>
<dd><code>NET_6LOWPAN_GP64_ADDRESS</code>, the interface only registers GP64.</dd>
<dd><code>NET_6LOWPAN_GP16_ADDRESS</code>, the interface only registers GP16.</dd>
<dd><code>NET_6LOWPAN_MULTI_GP_ADDRESS</code>, the interface registers both GP16 (primary) and GP64 (secondary) addresses.</dd>

<dt><code>short_address_base</code></dt>
<dd>Short address base. If an application defines the value <code>0-0xFFD</code>, 6LoWPAN tries to register the GP16 address using that address. <code>0xFFFE</code> and <code>0xFFFF</code> generate a random 16-bit short address. If the border router wants to use a short address, it defines it here.</dd>

<dt><code>define_new_short-address_at_DAD</code></dt>
<dd>This parameter is only checked when the mode is not <code>NET_6LOWPAN_GP64_ADDRESS</code> and the <code>short_address_base</code> is <code>0-0xFFD</code>. The recommended value 1 enables an automatic new address definition at Duplicate Address Detection (DAD). The value 0 generates a DAD error for an interface bootstrap, which the border router does not check.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The link layer security of the interface is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 Bootstrap mode is not selected.</dd>
<dd>-3 An unsupported short address at this platform.</dd>
</dl>

## Border router interface API

The border router is configured in three phases:

1. MAC and 6LoWPAN ND:
	1. MAC:
		* PAN ID.
		* NWK ID.
		* Beacon protocol ID.
		* Channel.
	2. Interface address mode:
		* Use only GP64.
		* Use only GP16.
		* Support for both GP16 and GP64.
	3. ND setup:
		* RA liftime.
		* Contexts.
		* ABRO version number.
		* Global Prefix address prefix/64.
2. Optional PANA server configuration.
3. Optional RPL DODAG configuration:
	1. Generate a DODAG root base.
	2. Prefixes.
	3. Routes.

#### Define border router setup

To define the border router MAC and 6LoWPAN ND setup for a selected interface, use the following function:

```
int8_t arm_nwk_6lowpan_border_router_init
(
	int8_t 					nwk_interface_id,
	border_router_setup_t	*border_router_setup_ptr
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>border_router_setup_ptr</code></dt>
<dd>A pointer to the MAC and 6LoWPAN ND setup.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The basic setup of the border router is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 A NULL pointer parameter.</dd>
<dd>-3 Already in up state.</dd>
<dd>-4 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code>.</dd>
<dd>-5 Not a proper interface type.</dd>
</dl>

#### Update border router context list

To update the context list of the interface border router at the configure proxy, use the following function. The update will go to the proxy setup where it must be pushed by `arm_nwk_6lowpan_border_router_configure_push()` if the interface is up.

```
int8_t arm_nwk_6lowpan_border_router_context_update
(
int8_t nwk_interface_id,
	uint8_t c_id_flags,
	uint8_t context_len,
	uint16_t ttl,
	uint8_t *context_ptr
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>c_id_flags</code></dt>
<dd>Bit [5] indicates compression support and bit [0:3] a context ID.</dd>

<dt><code>context_len</code></dt>
<dd>The context length in bits must be [64:128].</dd>

<dt><code>ttl</code></dt>
<dd>The time to live value of the context in minutes.</dd>

<dt><code>context_ptr</code></dt>
<dd>A pointer to a full 128-bit memory area.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 Context is added to the proxy OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 No memory for a new context.</dd>
<dd>-3 Failure of a given parameter (<code>c_id_flags</code> > <code>0x1F</code> or <code>contex_len</code> < 64).</dd>
<dd>-4 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
<dd>-5 Not a proper interface type.</dd>
</dl>


#### Context parameter update
To change the compression state or time-to-live value of the context, use the following function. The update will go to the proxy setup where it must be pushed by `arm_nwk_6lowpan_border_router_configure_push()` if the interface is up.

```
int8_t arm_nwk_6lowpan_border_router_context_parameter_update
(
	int8_t nwk_interface_id,
	uint8_t c_id,
	uint8_t compress_mode,
	uint16_t ttl
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>c_id</code></dt>
<dd>A context ID for detection of a proper context.</dd>

<dt><code>compress_mode</code></dt>
<dd>1 Enables 6LoWPAN compression for TX and RX.</dd>
<dd>0 Disables context compression for TX.</dd>

<dt><code>ttl</code></dt>
<dd>The time-to-live value of the context in minutes.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The basic setup of the border router is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 An unknown context ID.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
</dl>


#### Delete context

To delete the selected context, use the following function. The update will go to the proxy setup where it must be pushed by `arm_nwk_6lowpan_border_router_configure_push()` if the interface is up.

```
int8_t arm_nwk_6lowpan_border_router_context_remove_by_id
(
	int8_t nwk_interface_id,
	uint8_t c_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>c_id</code></dt>
<dd>A context ID for detection of a proper context.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The basic setup of the border router is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 An unknown context ID.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
</dl>


#### Reload 6LoWPAN contexts

To reload all 6LoWPAN contexts from the proxy and update the ABRO version number, use the following function. The RA advertisement of the border router starts advertising a new 6LoWPAN context setup.

```
int8_t arm_nwk_6lowpan_border_router_configure_push
(
	int8_t nwk_interface_id
	uint8_t c_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>c_id</code></dt>
<dd>A context ID for detection of a proper context.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The basic setup of the border router is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 An unknown context ID.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
</dl>

## Network sleep control API

If the application wants to save power, the 6LoWPAN Stack can enter sleep state by calling the `arm_net_enter_sleep()` function. The 6LoWPAN Stack will respond with an `EV_READY_TO_SLEEP` event in the core idle function that was defined during initialization, when it is possible to change the power mode. The application must return the time spent in sleep mode to the 6LoWPAN Stack when the processor wakes up.

Sleep is only supported when the 6LoWPAN bootstrap mode is `NET_6LOWPAN_SLEEPY_HOST`. The stack will automatically call `platform_event_os_sleep()` when it sets the CPU to sleep or sets the current task to sleep for a given time.

An example of how to place the processor in sleep mode:

```
uint32_t app_ns_core_idle
(
	uint8_t		event,
	uint32_t	sleep_time_ms
)
{
	uint32_t returned_slept_time = 0;

	if( event == EV_READY_TO_SLEEP )
	{
		/*Return time spent in sleep mode (milliseconds)*/
		returned_slept_time = hal_sleep( SLEEPMODE_POWER_SAVE, 198000, 1 );
	}
	else /* EV_CORE_IDLE */
	{
		hal_idle_sleep( );
	}

	return returned_slept_time;
}
```

where `hal_sleep( )` and `hal_idle_sleep( )` are functions in the application, which put the processor in sleep or in an idle state.

### Checking if sleep mode possible

Use the function `arm_net_check_enter_deep_sleep_possibility()` to check whether there is a possibility of putting the stack in deep sleep or not.

```
uint32_t time_to_sleep=0;
time_to_sleep = arm_net_check_enter_deep_sleep_possibility()
```
<dl>
<dt><code>Return value</code></dt>
<dd>0, not possible to sleep.</dd>
<dd>Time in milliseconds, possible to sleep for that amount of time.</dd>
</dl>

### Enter sleep mode

Use `arm_net_enter_sleep()` function to put the stack to sleep.

```
int arm_net_enter_sleep(void);
```
<dl>
<dt><code>Return value</code></dt>
<dd>0, stack stopped.</dd>
<dd>-1, action not supported at the moment.</dd>
</dl>

### Restart stack and synch timers

Use `arm_net_wakeup_and_timer_synch()` function to restart the stack and synchronize timers.

```
int arm_net_wakeup_and_timer_synch(uint32_t time_to_sleep);
```
<dl>
<dt><code>Return value</code></dt>
<dd>0, stack restarted.</dd>
<dd>1, stack will restart after sleeping for a time defined as time_to_sleep.</dd>
<dd>-1, stack already active.</dd>
</dl>

## RPL structures and definitions API

The 6LoWPAN Border Router defines an RPL MOP and DODAG preference. The ZigBeeIP must use `(BR_DODAG_MOP_NON_STORING | BR_DODAG_PREF_7)` only one RPL DODAG root. The RPL module defines a DODAG ID for an interface DAG from the default GP address of the interface. Therefore, before the interface can use the RPL module, the interface bootstrap and address must first be configured.

The DODAG instance ID is dynamically allocated. It generates the first free number starting from `0x01`, and ends to `0xFE`.

The DODAG configuration structure has the following variables:

```
typedef struct dodag_config_s
{
	uint8_t DAG_SEC_PCS;
	uint8_t DAG_DIO_INT_DOUB;
	uint8_t DAG_DIO_INT_MIN;
	uint8_t DAG_DIO_REDU;
	uint16_t DAG_MAX_RANK_INC;
	uint16_t DAG_MIN_HOP_RANK_INC;
	uint16_t DAG_OCP;
	uint8_t LIFE_IN_SECONDS;
	uint16_t LIFETIME_UNIT;
} dodag_config_s;
```

where:

<dl>
<dt><code>DAG_SEC_PCS</code></dt>
<dd>Defines the possible parents for node. The recommended and maximum value 1 means two parents.</dd>

<dt><code>DAG_DIO_INT_DOUB</code></dt>
<dd>RPL Trickle DIOIntervalDoublings. Should use 12.</dd>

<dt><code>DAG_DIO_INT_MIN</code></dt>
<dd>RPL Trickle DIOIntervalMin. Should use 9.</dd>

<dt><code>DAG_DIO_REDU</code></dt>
<dd>RPL Trickle DIORedundancyConstant. Should use 3.</dd>

<dt><code>DAG_MAX_RANK_INC</code></dt>
<dd>RPL MaxRankIncrease. Should use 2048.</dd>

<dt><code>DAG_MIN_HOP_RANK_INC</code></dt>
<dd>RPL MinHopRankIncrease. Should use 128.</dd>

<dt><code>DAG_OCP</code></dt>
<dd>Objective Code Point must use 1.</dd>

<dt><code>LIFE_IN_SECONDS</code></dt>
<dd>Lifetime that is used as default for all routes (expressed in units of lifetime unit). Default lifetime in seconds is <code>LIFE_IN_SECONDS * LIFETIME_UNIT</code>.</dd>

<dt><code>LIFETIME_UNIT</code></dt>
<dd>Defines the unit in seconds that is used to express route lifetimes. 60 means 1 minute.</dd>

</dl>

### DODAG root setup

The DODAG root setup is as follows:

**_DODAGPreference_ (Prf)**

A 3-bit unsigned integer that defines how preferable the root of this DODAG is compared to other DODAG roots within the instance. The DAGPreference ranges from `0x00` (least preferred) to `0x07` (most preferred).
The default is `0` least preferred).

```
#define BR_DODAG_PREF_0 0
#define BR_DODAG_PREF_1 1
#define BR_DODAG_PREF_2 2
#define BR_DODAG_PREF_3 3
#define BR_DODAG_PREF_4 4
#define BR_DODAG_PREF_5 5
#define BR_DODAG_PREF_6 6
#define BR_DODAG_PREF_7 7
```

**_Mode of Operation_ (MOP)**

The MOP field identifies the mode of operation of the RPL instance as administratively provisioned at and distributed by the DODAG root. All nodes that join the DODAG must be able to honor the MOP to fully participate as a router, or else they must only join as a leaf.

```
/**Non-Storing Mode of Operation  */
#define BR_DODAG_MOP_NON_STRORING 8
/** Storing Mode of Operation with no multicast support */
#define BR_DODAG_MOP_STRORING 16
```

**_Grounded_ (G)**

The Grounded `G` flag indicates that the DODAG advertised can satisfy the application-defined goal:

- If the flag is set, the DODAG is grounded.
- If the flag is cleared, the DODAG is floating.

```
#define BR_DODAG_FLOATIN 0<<7
#define BR_DODAG_GROUNDED 1<<7
/** RPL Prefix update Flags for A-flag AUTONOMOUS address generation*/
#define RPL_PREFIX_AUTONOMOUS_ADDRESS_FLAG 0x40
/** RPL Prefix update Flags for R-Flag */
#define RPL_PREFIX_ROUTER_ADDRESS_FLAG 0x20
```

## RPL root configuration API

This section introduces the functionality of the RPL root API.

### Define RPL DODAG root proxy

To define the RPL DODAG root proxy for a selected interface, use the following function:

```
int8_t arm_nwk_6lowpan_rpl_dodag_init
(
	int8_t nwk_interface_id,
	dodag_config_s *config,
	uint8_t flags
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>config</code></dt>
<dd>A pointer to the DODAG configure structure.</dd>

<dt><code>flags</code></dt>
<dd>Defines the RPL MOP,  DODAG Pref and Grounded. The ZigBeeIP must use the non stroring mode of operation and the most preferred preference <code>(BR_DODAG_MOP_NON_STRORING | BR_DODAG_PREF_7)</code>.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The RPL DODAG proxy allocation is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has already defined the DODAG root.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
<dd>-4 No memory for DODAG root base.</dd>
</dl>

### Update RPL prefix

To update the RPL prefix to the DODAG proxy, use the following function:

```
int8_t arm_nwk_6lowpan_rpl_dodag_prefix_update
(
	int8_t 		nwk_interface_id,
	uint8_t 	*prefix_ptr,
	uint8_t 	prefix_len,
	uint8_t 	flags,
	uint32_t 	lifetime
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID that defines the root proxy where the prefix update will come.</dd>

<dt><code>prefix_ptr</code></dt>
<dd>A pointer to the IPv6 prefix (16 bytes).</dd>

<dt><code>prefix_len</code></dt>
<dd>The prefix length must be 64.</dd>

<dt><code>flags</code></dt>
<dd>Defines: </dd>
<dd>R-flag <code>(RPL_PREFIX_ROUTER_ADDRESS_FLAG)</code>.</dd>
<dd>A-flag <code>(RPL_PREFIX_AUTONOMOUS_ADDRESS_FLAG)</code>.</dd>

<dt><code>lifetime</code></dt>
<dd>The prefix lifetime.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The RPL DODAG proxy allocation is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 No root defined for a specific interface.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
<dd>-4 No memory for the prefix.</dd>
</dl>

### Update RPL route information

To update the RPL route information to the DODAG proxy, use the following function:

```
int8_t arm_nwk_6lowpan_rpl_dodag_route_update
(
	int8_t 		nwk_interface_id,
	uint8_t 	*prefix_ptr,
	uint8_t 	prefix_len,
	uint8_t 	flags,
	uint32_t 	lifetime
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID that defines the root proxy where the prefix update will come.</dd>

<dt><code>prefix_ptr</code></dt>
<dd>A pointer to the IPv6 prefix (16 bytes).</dd>

<dt><code>prefix_len</code></dt>
<dd>The prefix length must be 64.</dd>

<dt><code>flags</code></dt>
<dd>Defines the R-flag <code>(RPL_PREFIX_ROUTER_ADDRESS_FLAG)</code>.</dd>

<dt><code>lifetime</code></dt>
<dd>The route lifetime.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The RPL DODAG proxy allocation is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 No root defined for a specific interface.</dd>
<dd>-3 The interface bootstrap is not <code>NET_6LOWPAN_BORDER_ROUTER</code> or the border router init was not called.</dd>
<dd>-4 No memory for the route.</dd>
</dl>

### Activate RPL DODAG

To activate the defined and configured RPL DODAG, if the configuration was done when the interface was in _up_ state, use the following function:

```
int8_t arm_nwk_6lowpan_rpl_dodag_start
(
	int8_t nwk_interface_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The RPL DODAG is started OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

### DODAG DAO trig

To trigger an RPL DODAG DAO by DTSN increment, use the following function. DODAG nodes send DAO routing information to the root.

```
int8_t arm_nwk_6lowpan_rpl_dodag_dao_trig
(
	int8_t nwk_interface_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The RPL DODAG DAO trig is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

### DODAG version increment

Network devices reset the current RPL instance. To do a unicast DIS/DIO and DAO/DAO ACK handshake, use the following function:

```
int8_t arm_nwk_6lowpan_rpl_dodag_version_increment
(
	int8_t nwk_interface_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The RPL DODAG DAO trig is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

### Run down RPL DODAG

To run down the RPL DODAG interface by flooding the poison rank, use the following function. Wait for an event from the stack when the flooding is completed. After this call, DODAG removal is recommended.

```
int8_t arm_nwk_6lowpan_rpl_dodag_poison
(
	int8_t nwk_interface_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The RPL DODAG flooding started OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

### Remove DODAG root setup

To remove the DODAG root setup from the selected interface, use the following function:

```
int8_t arm_nwk_6lowpan_rpl_dodag_remove
(
	int8_t nwk_interface_id
)
```

where:

<dl>
<dt><code>nwk_interface_id</code></dt>
<dd>The network interface ID.</dd>

<dt><code>Return value</code></dt>
<dd>>=0 The RPL DODAG removal is OK.</dd>
<dd>-1 An unknown interface ID.</dd>
<dd>-2 The interface has not defined any RPL DODAG root.</dd>
</dl>

## MLE router and host lifetime configuration

6LoWPAN and Thread use global router lifetime between neighbours. Routers refresh router neighbours entry when they receive MLE Advertisment message. Router lifetime is global so whole network should use same value. Host lifetime is node spesific lifetime and host will change that information between MLE link handshake. Host will refresh neighbour connection when about 70% lifetime have been running.
Thread define 100 seconds router lifetime which should not touch when running Thread stack.

6LoWPAN default router lifetime is 128 seconds which define 32 seconds MLE advetisment interval with [0.9-1.1] times random. Router need to miss 4 advertisment from router neighbour device before it silencely remove entry.

How network affect when setting longer router lifetime:
* MLE advertisment period is longer (Router lifetime in sconds / 4)
* Router's react slower when neighbour router is not reachable
* New Neighbour are learned slower after bootstrap
* Good setup with bigger network setup

How network affct when setting shorter router lifetime:
* Shorter MLE advertisment period
* Detect faster new neighbours and dissappier routers
* Good solution for small network which need to react fast for dynamic enviroment

### How to control lifetime parameters
Lifetime parameters can be change from default values when interface is created but before start bootstrap.

```
int lowpan_opt_set(
int8_t interface_id, 
enum lowpan_opt option, 
const void *opt_value, 
uint16_t opt_len
)
```
where:
<dl>
<dt><code>interface_id</code></dt>
<dd>Interface Id for setting new lifetime values.</dd>

<dt><code>option</code></dt>
<dd>A option type which update: LOWPAN_ROUTER_DEFAULT_LIFETIME or LOWPAN_HOST_DEFAULT_LIFETIME.</dd>
<dd> Supported lifetime values are between 64-2560 seconds.</dd>

<dt><code>opt_value</code></dt>
<dd>Pointer which include lifetime in seconds.</dd>

<dt><code>opt_len</code></dt>
<dd> Define value length behind opt_value. For lifetime this should be 2 ( sizeof(int16_t) )

<dt><code>Return value</code></dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

Example for setting router lifetime to 256 seconds.

`int16_t lifetime=256;`
`lowpan_opt_set(lowpan_interface_id, LOWPAN_ROUTER_DEFAULT_LIFETIME, &lifetime, sizeof(int16_t));`

## NET address retrieval API

An application can read the interface NET address (see _Table 3-14_) from the selected interface.

**Table 3-14 NET address**

Address|Description
-------|-----------
`ADDR_IPV6_GP`|Primary GP IPv6 address. Every IPv6 and 6LoWPAN interface supports this by default.
`ADDR_IPV6_GP_SEC`|Secondary IPv6 address. The 6LoWPAN interface supports this only when the address mode for bootstrap is `NET_6LOWPAN_MULTI_GP_ADDRESS`.

To read the network address information, use the following function:

```
extern int8_t arm_net_address_get
(
	int8_t 		nwk_interface_id
	net_address_t	addr_id,
	uint8_t		*address
)
```

where:
<dl>
<dt><code>addr_id</code></dt>
<dd>Identifies the address information type to be read.</dd>

<dt><code>address</code></dt>
<dd>A pointer to a structure where the address information is written to.</dd>

<dt><code>Return value</code></dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

## 6LoWPAN ND configuration API

This section introduces the functions to configure 6LoWPAN ND (Neighbour Discovery) parameters. _Table 3-15_ shows the configuration functions available.

**Table 3-15 Functions to configure 6LoWPAN ND parameters**

Function|Description
--------|-----------
`net_6lowpan_nd_parameter_read()`|Read a 6LoWPAN ND parameter.
`net_6lowpan_nd_parameter_set()`|Set a 6LoWPAN ND parameter.


### Parameter structure

This structure defines the 6LoWPAN ND parameters and has the following variables:

```
typedef struct nd_parameters_s
{
	uint8_t 	rs_retry_max;
	uint8_t 	ns_retry_max;
	uint16_t 	timer_random_max;
	uint16_t 	rs_retry_interval_min;
	uint16_t 	ns_retry_interval_min;
	uint16_t 	ns_retry_linear_backoff;
	uint16_t 	ra_interval_min;
	uint8_t 	ra_transmits;
	uint16_t 	ns_forward_timeout;
} nd_parameters_s;
```

where:

<dl>
<dt><code>rs_retry_max</code></dt>
<dd>Defines the maximum retry count of the bootstrap RS.</dd>

<dt><code>ns_retry_max</code></dt>
<dd>Defines the maximum retry count of the bootstrap NS.</dd>

<dt><code>timer_random_max</code></dt>
<dd>Defines the interval random in 6LoWPAN bootstrap timer ticks for RS, NS and starting NS-NA process.</dd>

<dt><code>rs_retry_interval_min</code></dt>
<dd>Defines the retry interval in 6LoWPAN bootstrap timer ticks waiting for RA.</dd>

<dt><code>ns_retry_interval_min</code></dt>
<dd>Defines the retry interval in 6LoWPAN bootstrap timer ticks waiting for NA.</dd>

<dt><code>ns_retry_linear_backoff</code></dt>
<dd>Defines the linear backoff of the retry interval in bootstrap timer ticks.</dd>

<dt><code>ra_interval_min</code></dt>
<dd>Defines the initial transmission interval for Router Advertisements in standard timer ticks.</dd>

<dt><code>ra_transmits</code></dt>
<dd>Defines the number of RA transmissions.</dd>

<dt><code>ns_forward_timeout</code></dt>
<dd>Defines the timeout when forwarding NS messages. If reached, our own address discovery process is restarted.</dd>
</dl>

### 6LoWPAN ND parameter set

Use this API to change the 6LoWPAN ND bootstrap parameters.

**Note**

This function should be called after `net_init_core()` and definitely before creating any 6LoWPAN interface.

For future compatibility, to support extensions to this structure:

1. Read the current parameters using `net_6lowpan_timer_parameter_read()`.
2. Modify the fields known.
3. Set the parameters.

```
int8_t net_6lowpan_nd_parameter_set
(
	const nd_parameters_s *parameter_ptr
)
```

where:

<dl>
<dt><code>parameter_ptr</code></dt>
<dd>A pointer for ND parameters.</dd>

<dt><code>Return value</code></dt>
<dd>0 Change is OK.</dd>
<dd>-1 Invalid values.</dd>
<dd>-2 6LoWPAN interface is already active.</dd>
</dl>

### 6LoWPAN ND parameter read

Use this API to read the 6LoWPAN ND bootstrap parameters:

```
void net_6lowpan_nd_parameter_read
(
	nd_parameters_s *parameter_ptr
)
```

where:

<dl>
<dt><code>parameter_ptr</code></dt>
<dd>An output pointer for ND parameters.</dd>
</dl>


## PANA configuration API

This section introduces the functions to configure PANA protocol parameters. _Table 3-16_ shows the configuration functions available.

**Table 3-16 Functions to configure PANA protocol parameters**

Function|Description
--------|-----------
`net_pana_parameter_read()`|Read the currently used parameters.
`net_pana_parameter_set()`|Write new parameters.

### Parameter structure

This structure defines the PANA module dynamic parameters and has the following variables:

```
typedef struct pana_lib_parameters_s
{
	uint16_t PCI_IRT;
	uint16_t PCI_MRT;
	uint8_t PCI_MRC;
	uint16_t REQ_IRT;
	uint16_t REQ_MRT;
	uint16_t REQ_MRC;
	uint16_t AUTHENTICATION_TIMEOUT;
	uint16_t KEY_UPDATE_THRESHOLD;
	uint8_t KEY_ID_MAX_VALUE;
	uint16_t EAP_FRAGMENT_SIZE;
	uint8_t  AUTH_COUNTER_MAX;
} pana_lib_parameters_s;
```

where:

<dl>
<dt><code>PCI_IRT</code></dt>
<dd>Initial PCI timeout in seconds. The default is 10.</dd>

<dt><code>PCI_MRT</code></dt>
<dd>Maximum PCI timeout value in seconds. The default is 60.</dd>

<dt><code>PCI_MRC</code></dt>
<dd><code>PCI_MRC</code> Maximum PCI retransmission attempts. The default is 5.</dd>

<dt><code>REQ_IRT</code></dt>
<dd><code>PCI_MRC</code> Initial request timeout in seconds. The default is 20.</dd>

<dt><code>REQ_MRT</code></dt>
<dd>Maximum request timeout value. The default is 60.</dd>

<dt><code>REQ_MRC</code></dt>
<dd>Maximum request retransmission attempts. The default is 4.</dd>

<dt><code>AUTHENTICATION_TIMEOUT</code></dt>
<dd>Maximum timeout for authentication timeout. The default is 100 seconds.</dd>

<dt><code>KEY_UPDATE_THRESHOLD</code></dt>
<dd>A gap in seconds when the server starts sending a new network key. The default is 10.</dd>

<dt><code>KEY_ID_MAX_VALUE</code></dt>
<dd>Defines the resolution for key ID <code>[1-KEY_ID_MAX_VALUE]</code>. The default is 255. The minimum accepted value is 3.</dd>

<dt><code>EAP_FRAGMENT_SIZE</code></dt>
<dd>Defines the EAP fragment slot size. Fragmentation is activated when the EAP payload is more than 920. The default is 296.</dd>

<dt><code>AUTH_COUNTER_MAX</code></dt>
<dd>Defines the limit for the PANA session re-authentication. When the maximum value is reached, the server does not respond to PANA notify request. The default is <code>0xFF</code>.</dd>
</dl>

### PANA parameter set

Use this API to change the PANA module parameters.

**Note**

This function should be called after `net_init_core()` and definitely before creating any 6LoWPAN interface.

For future compatibility, to support extensions to this structure:

1. Read the current parameters using `net_pana_parameter_read()`.
2. Modify the fields known.
3. Set the parameters.

```
int8_t net_pana_parameter_set
(
	const pana_lib_parameters_s *parameter_ptr
)
```

where:

<dl>
<dt><code>parameter_ptr</code></dt>
<dd>A pointer for PANA parameters.</dd>

<dt><code>Return value</code></dt>
<dd>0 Change is OK.</dd>
<dd>-1 Invalid values.</dd>
<dd>-2 PANA is not supported.</dd>
</dl>

### PANA parameter read

Use this API to read the PANA module parameters:

```
int8_t net_pana_parameter_read
(
	pana_lib_parameters_s *parameter_ptr
)
```

where:
<dl>
<dt><code>parameter_ptr</code></dt>
<dd>An output pointer for PANA parameters.</dd>

<dt><code>Return value</code></dt>
<dd>0 Read OK.</dd>
<dd>-1 PANA is not supported.</dd>
</dl>

## Network Information retrieval API

This section introduces functions to read network parameters. _Table 3-17_ shows the configuration functions available.

**Table 3-17 Functions to read network parameters**

Function|Description
--------|-----------
`arm_nwk_param_read()`|Read network layer configurations.
`arm_nwk_mac_address_read()`|Read MAC PAN ID, short address and EUI-64.
`arm_nwk_nd_address_read()`|Read the 6LoWPAN ND border router address and NWK prefix.

### Network layer configurations

To read network layer configurations, use the following function:

```
int8_t arm_nwk_param_read
(
	int8_t 			interface_id,
	link_layer_setups_s 	*network_params
)
```

where:

<dl>
<dt><code>network_params</code></dt>
<dd>A pointer to the structure where the network layer configurations are written to.</dd>

<dt><code>Return value</code></dt>
<dd>0 Success.</dd>
<dd>-1 Unknown PAN coordinator.</dd>
</dl>

This structure defines a parameter list of a network coordinator and it is used to read the network parameter for a warm start:

```
typedef struct link_layer_setups_s
{
	 uint16_t 		PANId;
	 uint8_t 		LogicalChannel;
	 net_mac_address_t 	addr_mode;
	 uint8_t 		address[8];
	 uint8_t 		sf;
}link_layer_setups_s;
```

where:

<dl>
<dt><code>PANId</code></dt>
<dd>Network PAN ID.</dd>

<dt><code>LogicalChannel</code></dt>
<dd>Network logical channel.</dd>

<dt><code>addr_mode</code></dt>
<dd>Coordinator address mode.</dd>

<dt><code>address</code></dt>
<dd>Coordinator address.</dd>

<dt><code>sf</code></dt>
<dd>Network super frame setup.</dd>
</dl>

### Link layer address read

To read the MAC PAN ID, short address and EUI-64, use the following function:

```
int8_t arm_nwk_mac_address_read
(
	int8_t 			interface_id,
	link_layer_address_s 	*mac_params
)
```

where:

<dl>
<dt><code>mac_params</code></dt>
<dd>A pointer to the structure where the MAC addresses are written to.</dd>

<dt><code>Return value</code></dt>
<dd>0 Success.</dd>
<dd>-1 .</dd>
</dl>

This structure defines the network MAC address information and it is used to read the link layer address:

```
typedef struct link_layer_address_s
{
	 uint16_t 	PANId;
	 uint16_t 	mac_short;
	 uint8_t 	mac_long[8];
	 uint8_t 	iid_eui64[8];
}link_layer_address_s;
```

where:

<dl>
<dt><code>PANId</code></dt>
<dd>Network PAN ID.</dd>

<dt><code>mac_short</code></dt>
<dd>MAC short address. Valid, if the value is <code><0xFFFE</code>.</dd>

<dt><code>mac_long</code></dt>
<dd>MAC long address (EUI-64 for IEEE 802.15.4; EUI-48 for Ethernet).</dd>

<dt><code>iid_eui64</code></dt>
<dd>IPv6 Interface Identifier based on EUI-64.</dd>
</dl>

### 6LoWPAN parameter read

To read the 6LoWPAN ND border router address and NWK prefix, use the following function:

```
int8_t arm_nwk_nd_address_read
(
	int8_t 			interface_id,
	network_layer_address_s *nd_params
)
```

where:

<dl>
<dt><code>nd_params</code></dt>
<dd>A pointer to the structure where the 6LoWPAN ND border router address is written to.</dd>

<dt><code>Return value</code></dt>
<dd>0 Success.</dd>
<dd>-1 .</dd>
</dl>

This structure defines the parent address information of the network layer and comprises the following variables:

```
typedef struct network_layer_address_s
{
	 uint8_t border_router[16];
	 uint8_t prefix[8];
}network_layer_address_s;
```

where:

<dl>
<dt><code>border_router</code></dt>
<dd>ND border router address.</dd>

<dt><code>prefix</code></dt>
<dd>A long, 64-bit network ID.</dd>
</dl>

## Multicast API

This section introduces functions for multicasting where data can be forwarded to several devices within the network and what devices are included is subject to the multicast scope. For example, a link local multicast is sent to neighbors and cannot be forwarded. However, a site local multicast can be forwarded with a trickle throughout the network and can travel through to the border router. See more on the [Trickle Algorithm](https://tools.ietf.org/html/rfc6206).

**Note**: The site local multicast is the only multicast scope that can be routed through the border router.

The multicast API can be used to subscribe and unsubscribe different multicast groups and can change the trickle multicast parameters. The multicast parameters are set and changed using the function `multicast_set_parameters()`where multicast groups are managed using the function calls `multicast_add_address()` and `multicast_free_address()`.

**Note**

- Only multicast addresses are accepted.
- Trickle forwarding cannot be used with link local addresses.
- The maximum number of multicast groups is 100 where including multicast groups consumes memory allocated by the 6LoWPAN stack.


### Set new parameters for trickle multicast

To set new parameters for a trickle multicast, use the following function:

```
void multicast_set_parameters
(
	uint8_t		i_min,
	uint8_t		i_max,
	uint8_t		k,
	uint8_t		timer_expirations,
	uint8_t		window_expiration
)
```

where:

<dl>
<dt><code>i_min</code></dt>
<dd>The minimum trickle timer interval (50ms resolution), so: <code>Imin = i_min * 50ms</code>.</dd>

<dt><code>i_max</code></dt>
<dd>The maximum trickle timer interval as number if doubling of minimum interval.</dd>

<dt><code>k</code></dt>
<dd>A redundancy constant.</dd>

<dt><code>timer_expirations</code></dt>
<dd>Number if trickle timer expires before terminating the trickle process.</dd>

<dt><code>window_expiration</code></dt>
<dd>Time window state is kept after the trickle process has ended in 50ms resolution.</dd>
</dl>

**Note**

If the `window_expiration` value is set too small, an infinite retransmission loop can occur when using the trickle multicast.

### Add a new address to a multicast group

To add a new address to a multicast group, use the following function:

```
uint8_t multicast_add_address
(
	unit8_t		*address_ptr,
	uint8_t		use_trickle
)
```

where:

<dl>
<dt><code>address_ptr</code></dt>
<dd>A pointer to an array containing the address to be added.</dd>

<dt><code>use_trickle</code></dt>
<dd>Possible trickle values:</dd>
<dd>0 - No trickle multicast forwarding.</dd>
<dd>>0 - Trickle multicast forward is used with this address.</dd>

<dt><code>Return value</code></dt>
<dd>0 General error.</dd>
<dd>1 Address updated.</dd>
<dd>2 Address added.</dd>
<dd>255 Link local not allowed when using multicast.</dd>
</dl>

### Free an address from a multicast group

To free an address from a multicast group, use the following function:

```
uint8_t multicast_free_address
(
	uint8_t		*address_ptr
)
```

where:

<dl>
<dt><code>address_ptr</code></dt>
<dd>A pointer to a 16 byte array that contains the address to be removed.</dd>

<dt><code>Return value</code></dt>
<dd>0 Success.</dd>
<dd>>0 Failure, an error occurred trying to remove the address.</dd>
</dl>

The following source code example shows you how to use the multicast API:

```
/*Set up 16 byte address*/
static const uint8_t multicast_address[16] =
{
	0xFF, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F
};

uint8_t add_multicast_address
(
	void
)
{

	uint8_t return_value = 0;


	/*Set new trickle multicast parameters:

		- 400 ms minimum interval (Imin)

		- 1.6 s maximum interval (Imax)

		- redundancy constant k = 8, if node receives same message
		 at least 8 time it will suppress retransmissions
		- 4 timer expirations before stopping trickle TX
		- 75 s window expiration */

	multicast_set_parameters( 8, 2, 8, 4, 150 );

	/*Try to subscribe to new site local address using trickle multicast*/
	return_value = multicast_add_address( multicast_address, 1 );

	if( return_value == 1 )
	{
		/*Address already added, update successful */
	}
	else if( return_value == 2 )
	{
		/*New address successfully added.*/
	}
	else
	{
		/*Error occurred*/
	}
	return return_value;
}
```



