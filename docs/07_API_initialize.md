6LoWPAN Stack Initialization
===============================

This chapter describes the 6LoWPAN stack initialization sequence. It contains the following sections:

- [_About initalization_](#about-initalization)
- [_Stack initialize sequence_](#stack-initialize-sequence)
- [_How to configure a network interface_](#how-to-configure-a-network-interface)

## About initalization

This section is an introduction to the 6LoWPAN stack initialization.

To take the 6LoWPAN stack into use, follow these instructions:

1. Initialize the event OS by calling `eventOS_scheduler_init()`.
2. Initialize the 6LoWPAN stack and platform by calling `net_init_core()`.
3. Allocate a dynamic tasklet for an application by calling `eventOS_event_handler_create()`.
4. Configure an interface and network.

## Stack initialize sequence

Perform stack initialization in the `main( )` function, as follows:

1. Initialize the event OS by calling `eventOS_scheduler_init()`.
2. Initialize the 6LoWPAN stack by calling `net_init_core()`.
3. Start the event dispatch loop by calling `eventOS_scheduler_run();`. The 6LoWPAN stack scheduler will now run in an infinite loop.

The `eventOS_scheduler_run()` function call starts the processing of the 6LoWPAN stack events. This function is an infinite loop where the processor cannot execute any commands below the `eventOS_scheduler_run()` call.

An example 6LoWPAN stack initialization sequence:

```
void main(void)
{
	// Run any platform specific
	// initialization here
	board_init();
	// Initialize 6LoWPAN stack
	eventOS_scheduler_init();
	net_init_core();
	// Register a tasklet
	tasklet_id = eventOS_event_handler_create(&tasklet_main,
	ARM_LIB_TASKLET_INIT_EVENT);
	// Enter the event loop
	eventOS_scheduler_run();
}
```

## How to configure a network interface

To effectively use the 6LoWPAN stack, you must run at least one tasklet. Some API interfaces require tasklets for handling incoming events. The following resources must be configured from a tasklet:

- Interface:
	- When you generate an interface it is tied to the registering tasklet. If the application removes the tasklet, the interface events do not work anymore.

- Sockets:
	- When the sockets are generated, they are also tied to the tasklet used in registration.

To configure a network interface, follow these instructions:

1. Create an interface by calling the function `arm_nwk_interface_init()`.
2. Define the 6LoWPAN bootstrap by calling the function `arm_nwk_interface_configure_6lowpan_bootstrap_set()`.
3. Configure the 6LoWPAN link layer security by calling the function `arm_nwk_6lowpan_link_layer_security_mode()`.
4. (Optional) Configure the 6LoWPAN address mode by calling the function  `arm_nwk_6lowpan_gp_address_mode()`.
5. (Optional) Configure the 6LoWPAN node NWK scan parameters by calling the function  `arm_nwk_6lowpan_link_scan_paramameter_set()`.
6. (Optional) Configure the 6LoWPAN node NWK scan filters by calling the functions:
	* PAN ID:  `arm_nwk_6lowpan_link_panid_filter_for_nwk_scan()`.
	* Network ID: `arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan().`
	* Protocol ID: `arm_nwk_6lowpan_link_protocol_id_filter_for_nwk_scan()`.
7. (Optional) Configure the PANA, if it is activated for link layer security, by calling the functions:
	* Server: `arm_pana_server_library_init()`.
	* Client: `arm_pana_client_library_init()`.
8. Start the interface by calling the function `arm_nwk_interface_up()`.
