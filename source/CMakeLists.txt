file(GLOB_RECURSE objs "*.o")
add_library(mbed-6lowpan ${objs})
set_target_properties(mbed-6lowpan PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(mbed-6lowpan mbed-client-libservice coap-service mbed-6lowpan-eventloop)
