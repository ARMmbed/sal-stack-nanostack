if(NOT DEFINED YOTTA_CFG_NANOSTACK_CONFIGURATION)
	set(YOTTA_CFG_NANOSTACK_CONFIGURATION mbedOS)
endif()

if(DEFINED TARGET_LIKE_CR_GCC)
  file(GLOB_RECURSE objs "cr-gcc/${YOTTA_CFG_NANOSTACK_CONFIGURATION}/*.o")
elseif(DEFINED TARGET_LIKE_GCC)
  file(GLOB_RECURSE objs "gcc/${YOTTA_CFG_NANOSTACK_CONFIGURATION}/*.o")
elseif(DEFINED TARGET_LIKE_ARMCC)
  file(GLOB_RECURSE objs "armcc/${YOTTA_CFG_NANOSTACK_CONFIGURATION}/*.o")
endif()

add_library(sal-stack-nanostack ${objs})
set_target_properties(sal-stack-nanostack PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(sal-stack-nanostack
    mbed-client-c
    nanostack-libservice
    sal-stack-nanostack-eventloop
    nanostack-randlib
)
