file(GLOB_RECURSE objs "*.o")
add_library(mbed-6lowpan ${objs})
SET_TARGET_PROPERTIES(mbed-6lowpan PROPERTIES LINKER_LANGUAGE C)
