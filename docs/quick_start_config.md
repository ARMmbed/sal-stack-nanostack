Configuring the network
=======================

The Nanostack configuration has a huge impact on the network behaviour. Due the number of different network types possible, the configuration must always planned to match the topology. Appropriate default values cannot always be provided.

The 6LoWPAN network consist of different protocols. Each of them has a different scope on the network bootsrap.
The following list provides examples of the parameters you need to consider when designing a network:

* IEEE 802.15.4 Beacons
    * Protocol id
 	* PAN ID
 	* Network ID
* Node type: Router/Host/sleepy host.
* Network prefix
* 6LoWPAN Neighbor Discovery (6LoWPAN-ND)
    * Address mode (GP64/GP16)
 	* Neighbor discovery probes
* Mesh Link Establishment (MLE)
    * Lifetimes
* Routing Protocol for Low power and Lossy networks (RPL)
    * Lifetimes
* Security
    * Autheticating nodes
    * Link-layer security
    * Transport layer security

Configuring lifetime values for routing related protocols is important. In general, the faster the network reacts to failures, the more bandwidth it uses for keep alive messages and RPL+MLE signalling.

Most of the configuration values must be set when initializing the stack, before starting the network bootstrap. Refer to section [IPv6/6LoWPAN Stack Initialization](07_API_initialize.md) for more information.

### Beacon information

Nanostack uses 802.15.4 beacons to carry some extra information (Pekka: such as: xxx) that needs to be configured by the user.

The 6LoWPAN nodes use beacons to find the specific network that the device wants to join.
All fields (Pekka: what fields?) must be set on the border router. On the node side, the filtering (Pekka: filtering what?) is optional.

On a border router beacon, the information (Pekka: what information?) is part of the `border_router_setup_s` structure that is used when initializing the stack:

```
int8_t arm_nwk_6lowpan_border_router_init(int8_t interface_id, border_router_setup_s *border_router_setup_ptr);
```

#### Protocol ID

ZigbeeIP defines the protocol ID to be `2` and we recommend to leave this as default.

This is a mandatory setting on the border router, as it is a part of the `border_router_setup_s` structure.

On the 6LoWPAN nodes, you can filter beacons by protocol ID. To enable the filtering, call:

```
int8_t arm_nwk_6lowpan_link_protocol_id_filter_for_nwk_scan(int8_t interface_id, uint8_t protocol_ID);
```

#### PAN ID

PAN ID is a locally unique identifier for the network. On the same range, different 6LoWPAN networks must use
different PAN IDs.

On the border router, `mac_panid` is a part of the `border_router_setup_s` structure and must be set to a value between 1 and 0xfffe.

To enable the PAN ID filtering on the 6LoWPAN nodes, call:

```
int8_t arm_nwk_6lowpan_link_panid_filter_for_nwk_scan(int8_t interface_id, uint16_t pan_id_filter);
```

#### Network ID

Network ID is analogous to WiFi network names. It is a 16 bytes-long binary string.

On the border router, the field `uint8_t network_id[16]` is a part of the `border_router_setup_s` structure.

To enable filtering on the node side, call:

```
int8_t arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan(int8_t interface_id, const uint8_t *nwk_id_filter);
```

### Selecting the node type

When initializing the stack, a node must call:

```
int8_t arm_nwk_interface_configure_6lowpan_bootstrap_set(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode, net_6lowpan_mode_extension_e net_6lowpan_mode_extension);
```

Where the `bootstrap_mode` can be one of the following:

* NET_6LOWPAN_ROUTER
* NET_6LOWPAN_HOST
* NET_6LOWPAN_SLEEPY_HOST


### Tuning routing protocols

See Doxygen documentation for: [net_6lowpan_parameter_api.h](https://docs.mbed.com/docs/arm-ipv66lowpan-stack/en/latest/api/net__6lowpan__parameter__api_8h.html).


## Enabling the security

Nanostack allows you to choose from following link layer security features:

* No security.
* PSK network authentication.
* PANA network authentication.

See the [The Network Layer Control APIs](12_API_network.md) for how to enable the security features.
