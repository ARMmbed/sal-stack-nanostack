file(GLOB_RECURSE objs "*.o")
add_library(sal-stack-nanostack ${objs})
set_target_properties(mbed-6lowpan PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(mbed-6lowpan
    mbed-client-c
    nanostack-libservice
    coap-service
    sal-stack-nanostack-eventloop
    nanostack-randlib
)
