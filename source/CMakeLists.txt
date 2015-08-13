file(GLOB_RECURSE objs "*.o")
add_library(sal-stack-nanostack ${objs})
set_target_properties(sal-stack-nanostack PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(sal-stack-nanostack
    mbed-client-c
    nanostack-libservice
    coap-service
    sal-stack-nanostack-eventloop
    nanostack-randlib
)
