ARM IPV6/6LoWPAN stack.
=======================

This repository contains ARM IPv6/6LoWPAN stack for mbed OS.

Documentation is hosted in http://docs.mbed.com/projects/arm-ipv66lowpan-stack/

On mbed OS the preferred usage is through [mbed Mesh API](https://github.com/ARMmbed/mbed-mesh-api) that provides required wrappers between Socket abstraction layer and this stack.

See example application [mbed-client-example-6lowpan](https://github.com/ARMmbed/mbed-client-example-6lowpan) for example usage.

See module [sal-iface-6lowpan](https://github.com/ARMmbed/sal-iface-6lowpan) for known limitations.

For Nanostack 4.0 there is number of API changes:

1. Added MLE router and host lifetime configuration API
2. Added MLE neighbor limits configuration API
3. MLE token bucket configuration API
4. Rename the `arm_nwk_6lowpan_link_scan_paramameter_set()` function to `arm_nwk_6lowpan_link_scan_parameter_set()`.
5. Channel mask settings API changed.
7. Added API for adding/deleting routes.
8. The parameters of function `cca_start()` have changed.
9. FHSS API added.

See https://docs.mbed.com/docs/arm-ipv66lowpan-stack/en/latest/api_changes_to_v4_0_0/index.html
for how to update your modules.
