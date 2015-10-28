# dev_stat_internal_init

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
</dl> 