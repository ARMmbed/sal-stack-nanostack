Network debug statistics API
==============================

This chapter describes the debug statistics interface API.

## API Header

Using the network debug statistics API, it is possible to collect statistics from the stack in real-time for debugging purposes. You can use this API by including the following header to your application:

```
#include nwk_stats_api.h
```

An overview of the function definitions and the Network statistics structure (`nwk_stats_t`) itself are presented further in this document.

##Initializing protocol stats collection

To initialize collection of network statistics in the network (NWK) statistics structure, use the following function call:

```
void protocol_stats_init
(
	void
)
```

## Start stats collection

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


## Stop stats collection

To stop collecting network statistics in NWK statistics structure, use the following function call:

```
void protocol_stats_stop
(
	void
)
```

## Reset stats

To reset network statistics to clean slate state, that is, all the statistics counters are set to zero, use se the following function call:

```
void protocol_stats_reset
(
	void
)
```

## Types of collectable stats

Various types of network stats can be collected using the structure `typedef struct nwk_stats_t `. The following statistics can be collected:

#### General MAC related stats

<dl>
<dt><code>mac_tx_buffer_overflow</code></dt>
<dd>Provides a count MAC TX queue overflow.</dd>

<dt><code>mac_rx_count</code></dt>
<dd>Number of received MAC packets.</dd>

<dt><code>mac_tx_count</code></dt>
<dd>Number of transmitted MAC packets.</dd>

<dt><code>mac_rx_drop</code></dt>
<dd>Number of dropped MAC packets.</dd>
</dl>

#### MAC payload flow

<dl>
<dt><code>mac_tx_bytes</code></dt>
<dd>Number of bytes transmitted.</dd>

<dt><code>mac_rx_bytes</code></dt>
<dd>Number of bytes received.</dd>

<dt><code>mac_tx_failed</code></dt>
<dd>Number of times a transmission failed.</dd>

<dt><code>mac_tx_retry</code></dt>
<dd>Number of times retries were made.</dd>

<dt><code>mac_tx_cca_cnt</code></dt>
<dd>Number of times clear channel assessment was made.</dd>

<dt><code>mac_tx_failed_cca</code></dt>
<dd>Number of failed CCA attempts.</dd>

<dt><code>mac_security_drop</code></dt>
<dd>Number of security packets dropped.</dd>
</dl>

#### 6LoWPAN related statistics

<dl>
<dt><code>ip_rx_count</code></dt>
<dd>Number of IP packets received.</dd>

<dt><code>ip_tx_count</code></dt>
<dd>Number of IP packets transmitted.</dd>

<dt><code>ip_rx_drop</code></dt>
<dd>Number of IP packets dropped.</dd>

<dt><code>ip_cksum_error</code></dt>
<dd>IP checksum error count.</dd>
</dl>

#### IP payload flow

<dl>
<dt><code>ip_tx_bytes</code></dt>
<dd>Number of bytes transmitted as IP packets.</dd>

<dt><code>ip_rx_bytes</code></dt>
<dd>Number of bytes received as IP packets.</dd>

<dt><code>ip_routed_up</code></dt>
<dd>Number of bytes routed as IP packets.</dd>
</dl>

#### Fragmentation statistics

<dl>
<dt><code>frag_rx_errors</code></dt>
<dd>Fragmentation errors in reception.</dd>

<dt><code>frag_tx_errors</code></dt>
<dd>Fragmentation errors in transmission.</dd>
</dl>

#### RPL statistics

<dl>
<dt><code>rpl_route_routecost_better_change</code></dt>
<dd>Number of times an RPL parent was changed.</dd>

<dt><code>ip_routeloop_detect</code></dt>
<dd>Number of times an RPL IP loop creation was detected.</dd>

<dt><code>ip_routeloop_detect</code></dt>
<dd>Number of times an RPL route loop creation was detected.</dd>

<dt><code>ip_no_route</code></dt>
<dd>Number of times an RPL route was not found.</dd>
</dl>

#### Various buffers

<dl>
<dt><code>buf_alloc</code></dt>
<dd>A count of buffer allocation.</dd>

<dt><code>buf_headroom_realloc</code></dt>
<dd>A buffer headroom allocation count.</dd>

<dt><code>buf_headroom_shuffle</code></dt>
<dd>A buffer headroom shuffling count.</dd>

<dt><code>buf_headroom_fail</code></dt>
<dd>A buffer headroom failure count.</dd>
</dl>




