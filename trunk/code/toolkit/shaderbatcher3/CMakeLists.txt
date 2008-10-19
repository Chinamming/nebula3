#-------------------------------------------------------------------------------
# toolkit/shaderbatcher3
#-------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}/foundation/
    ${Nebula3_SOURCE_DIR}/toolkit/
)
SET(toolkit_shaderbatcher3_src
    shaderbatcher3.cc
    shaderbatcherapp.cc
    shaderbatcherapp.h
)
SET(toolkit_shaderbatcher3_pch_src
    ../stdneb.cc
)
SET(tools_shaderbatcher3_SRCS
    ${toolkit_shaderbatcher3_src}
    ${toolkit_shaderbatcher3_pch_src}
)

SOURCE_GROUP(shaderbatcher3 FILES ${toolkit_shaderbatcher3_src})
SOURCE_GROUP(precompiled FILES ${toolkit_shaderbatcher3_pch_src})

ADD_EXECUTABLE(shaderbatcher3 ${tools_shaderbatcher3_SRCS})
N_SET_COMPILER_DEFAULTS()
N_SET_EXE_LINKER_DEFAULTS(shaderbatcher3)
N_SET_EXE_SUBSYSTEM(shaderbatcher3 Console)
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(shaderbatcher3 stdneb.h stdneb.cc ../)
ENDIF()
ADD_DEPENDENCIES(shaderbatcher3 toolkitutil)
TARGET_LINK_LIBRARIES(shaderbatcher3 
    foundation
    ${N_EXTRA_WIN32_LIBS}
    ${N_DIRECTX_LIBS}
)
