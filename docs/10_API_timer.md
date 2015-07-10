Library Timer API
=================

This chapter describes the timer functionality.

If an application requires a timer, it can allocate one with the `eventOS_event_timer_request( )` function. Further to a successful allocation, the system timer event occurs.

**Note**

The library supports dynamic timer count at the application layer. The stack can multiplex a single timer for multiple purposes.

## Allocate timer

To allocate timer events, use the following function call:

```
int8_t eventOS_event_timer_request
(
	uint8_t		snmessage,
	uint8_t		event_type,
	int8_t		tasklet_id,
	uint32_t	time
)
```

where:

<dl>
<dt><code>snmessage</code></dt>
<dd>The timer ID defined by the application.</dd>

<dt><code>event_type</code></dt>
<dd>The event type to be sent to a tasklet when the timer expires. Usually <code>ARM_LIB_SYSTEM_TIMER_EVENT</code>.</dd>

<dt><code>tasklet_id</code></dt>
<dd>The tasklet ID of the event receiver.</dd>

<dt><code>time</code></dt>
<dd>The requested period in milliseconds.</dd>
<dd><b>Note:</b> Resolution is 10ms.</dd>

<dt><code>Return value</code></dt>
<dd>0 If timer allocation was successful.</dd>
<dd>-1 If timer allocation failed.</dd>
</dl>

Further to the allocated time period, the event OS library will transmit an event of which the sender, event type `(event->event_type)` and `event->event_id`, is indicating allocated timer identification.

## Release timer

To cancel allocated timer events, use the following function call:

```
int8_t eventOS_event_timer_cancel
(
	uint8_t		snmessage,
	int8_t		tasklet_id
)
```

where:

<dl>
<dt><code>snmessage</code></dt>
<dd>The timer ID of the event to be cancelled.</dd>

<dt><code>tasklet_id</code></dt>
<dd>The tasklet ID of the event receiver.</dd>

<dt><code>Return value</code></dt>
<dd>0 If timer release was successful.</dd>
<dd>-1 If timer release failed, timer ID is invalid or the timer is already released.</dd>
</dl>

