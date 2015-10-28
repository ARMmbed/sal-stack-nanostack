# Device Driver API

The 6LoWPAN Stack uses radio drivers through a physical layer driver interface called PHY driver API. These functions allow the developers to easily port new drivers for the 6LoWPAN Stack allowing 6LoWPAN to be implemented on a wide variety of networking devices.

## API Headers

```
#include arm_hal_phy.h
```
The following code example shows a subset of imaginary drivers that send packets to a serial port:

```
/* Fill out the driver structure */
static phy_device_driver_s my_driver =
{
	.link_type = PHY_LINK_TUN,
	.tx = send,
	.state_control = state_control,
}

/* Driver implementation */
int8_t send(const uint8_t *data_ptr, uint16_t data_len, uint8_t tx_handle)
{
	// Send packet to serial line
	uart_send(data_ptr, data_len);
	return 0;
}

int8_t state_control(phy_interface_state_e new_state, uint8_t channel)
{
	switch(new_state) {
	case PHY_INTERFACE_UP:
		uart_enable();
		break;
	case PHY_INTERFACE_DOWN:
		uart_disable();
		break;
	}
	Return 0;
}

void uart_reader()
{
	static char *buffer[BUFFER_SIZE];
	uint16_t size;
	while(0 != (size=uart_read(buffer))) {
			// Incoming packets are pushed to Stack
			// using arm_net_phy_rx()
			arm_net_phy_rx(buffer, size, 0, interface_id);
	}
}

void register_my_driver()
{
	interface_id = arm_net_phy_register(&my_driver);
}
```

This driver sends raw IP packets to a serial interface. Incoming packets are pushed to the 6LoWPAN stack for parsing. Initialization of an actual hardware driver is left out from the example code.

For more information and a comprehensive documentation on Device Driver API, see chapter [**Porting 6LoWPAN Stack**](16_API_porting.md). 
