Debug Statistics Interface API
==============================

This chapter describes the debug statistics interface API.

## Debug statistics API

Using the debug statistics API it is possible to collect statistics from the stack in real-time for debugging purposes. You can use this API by including the following line to your application:

```
#include nwk_stats_api.h
```

An overview of the function definitions and the Network statistics structure (`nwk_stats_t`) itself are presented hereinafter.

### protocol_stats_init

To initialize collection of network statistics in the Network (NWK) statistics structure, use the following function call:

```
void protocol_stats_init
(
	void
)
```

### protocol_stats_start

To start collecting network statistics in the Network (NWK) statistics structure, use the following function call:

```
void protocol_stats_start
(
	nwk_stats_t	*stats_ptr
)
```

where:

<dl>
<dt><code>stats_ptr</code></dt>
<dd>A pointer to the statistics structure on application.</dd>
</dl>


### protocol_stats_stop

To stop collecting network statistics in NWK statistics structure, use the following function call:

```
void protocol_stats_stop
(
	void
)
```

### protocol_stats_reset

To reset network statistics to clean slate state, that is, all the statistics counters are set to zero, use se the following function call:

```
void protocol_stats_reset
(
	void
)
```

### Network statistics structure (nwk_stats_t)

Following statistics can be collected:

#### General MAC related stats
<dl>
<dt><code>mac_tx_buffer_overflow</code></dt>
<dd>Provides a count MAC TX queue overflow.</dd>
</dl>

<dl>
<dt><code>mac_rx_count</code></dt>
<dd>A count of received MAC packets.</dd>
</dl>

<dl>
<dt><code>mac_tx_count</code></dt>
<dd>A count of transmitted MAC packets.</dd>
</dl>


<dl>
<dt><code>mac_rx_drop</code></dt>
<dd>A count of dropped MAC packets.</dd>
</dl>

#### MAC payload flow
<dl>
<dt><code>mac_tx_bytes</code></dt>
<dd>A count of no. of bytes transmitted.</dd>
</dl>

<dl>
<dt><code>mac_rx_bytes</code></dt>
<dd>A count of no. of bytes received.</dd>
</dl>

<dl>
<dt><code>mac_tx_failed</code></dt>
<dd>No. of times a transmission failed.</dd>
</dl>

<dl>
<dt><code>mac_tx_retry</code></dt>
<dd>No. of times retries were made.</dd>
</dl>

<dl>
<dt><code>mac_tx_cca_cnt</code></dt>
<dd>No. of times clear channel assessment was made.</dd>
</dl>

<dl>
<dt><code>mac_tx_failed_cca</code></dt>
<dd>A count of failed CCA attempts.</dd>
</dl>


<dl>
<dt><code>mac_security_drop</code></dt>
<dd>A count of security packets dropped.</dd>
</dl>

#### 6LoWPAN related statistics

<dl>
<dt><code>ip_rx_count</code></dt>
<dd>No. of IP packets received.</dd>
</dl>

<dl>
<dt><code>ip_tx_count</code></dt>
<dd>No. of IP packets transmitted.</dd>
</dl>

<dl>
<dt><code>ip_rx_drop</code></dt>
<dd>No. of IP packets dropped.</dd>
</dl>

<dl>
<dt><code>ip_cksum_error</code></dt>
<dd>IP checksum error count.</dd>
</dl>
#### IP payload flow

<dl>
<dt><code>ip_tx_bytes</code></dt>
<dd>No. of bytes transmitted as IP packets.</dd>
</dl>

<dl>
<dt><code>ip_rx_bytes</code></dt>
<dd>No. of bytes received as IP packets.</dd>
</dl>

<dl>
<dt><code>ip_routed_up</code></dt>
<dd>No. of bytes routed as IP packets.</dd>
</dl>


#### Fragmentation statistics

<dl>
<dt><code>frag_rx_errors</code></dt>
<dd>Fragmentation errors in reception.</dd>
</dl>

<dl>
<dt><code>frag_tx_errors</code></dt>
<dd>Fragmentation errors in transmission.</dd>
</dl>
#### RPL statistics
<dl>
<dt><code>rpl_route_routecost_better_change</code></dt>
<dd>No. of times an RPL parent was changed.</dd>
</dl>

<dl>
<dt><code>ip_routeloop_detect</code></dt>
<dd>No. of times an RPL IP loop creation was detected.</dd>
</dl>

<dl>
<dt><code>ip_routeloop_detect</code></dt>
<dd>No. of times an RPL route loop creation was detected.</dd>
</dl>

<dl>
<dt><code>ip_no_route</code></dt>
<dd>No. of times an RPL route was not found.</dd>
</dl>
#### Various buffers

<dl>
<dt><code>buf_alloc</code></dt>
<dd>A count of buffer allocation.</dd>
</dl>

<dl>
<dt><code>buf_headroom_realloc</code></dt>
<dd>A buffer headroom allocation count.</dd>
</dl>

<dl>
<dt><code>buf_headroom_shuffle</code></dt>
<dd>A buffer headroom shuffling  count.</dd>
</dl>

<dl>
<dt><code>buf_headroom_fail</code></dt>
<dd>A buffer headroom failure  count.</dd>
</dl>


<!-- ### dev_stat_internal_init

To start collecting device statistics within the device statistics structure, use the following function call:

```
int8_t dev_stat_internal_init
(
	dev_stat_t	*info_ptr
)
```

where:

<dl>
<dt><code>info_ptr</code></dt>
<dd>A pointer to the device statistics structure on application.</dd>

<dt><code>return value</code></dt>
<dd>0 Success.</dd>

<dd>-1 Fail.</dd>
</dl>


### dev_stat_get_longest_heap_sector

The following function call returns the longest available free sector size in the 6LoWPAN stack dynamic memory:

```
int16_t dev_stat_get_longest_heap_sector
(
	void
)
```

where:

<dl>
<dt><code>return value</code></dt>
<dd>The size of the longest free sector.</dd>
</dl>


### dev_stat_get_runtime_seconds

The following function call returns the runtime since last reset:

```
uint32_t dev_stat_get_runtime_seconds
(
	void
)
```

where:

<dl>
<dt><code>return value</code></dt>
<dd>The runtime since last reset.</dd>
</dl> -->

