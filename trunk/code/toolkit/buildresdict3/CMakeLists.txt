#-------------------------------------------------------------------------------
# toolkit/buildresdict3
#-------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}/foundation/
    ${Nebula3_SOURCE_DIR}/toolkit/
)
SET(toolkit_buildresdict3_src
    buildresdict3.cc
    resourcedictionaryapp.cc
    resourcedictionaryapp.h
)
SET(toolkit_buildresdict3_pch_src
    ../stdneb.cc
)
SET(tools_buildresdict3_SRCS
    ${toolkit_buildresdict3_src}
    ${toolkit_buildresdict3_pch_src}
)

SOURCE_GROUP(buildresdict3 FILES ${toolkit_buildresdict3_src})
SOURCE_GROUP(precompiled FILES ${toolkit_buildresdict3_pch_src})

ADD_EXECUTABLE(buildresdict3 ${tools_buildresdict3_SRCS})
N_SET_COMPILER_DEFAULTS()
N_SET_EXE_LINKER_DEFAULTS(buildresdict3)
N_SET_EXE_SUBSYSTEM(buildresdict3 Console)
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(buildresdict3 stdneb.h stdneb.cc ../)
ENDIF()
ADD_DEPENDENCIES(buildresdict3 toolkitutil)
TARGET_LINK_LIBRARIES(buildresdict3 
    foundation
    ${N_EXTRA_WIN32_LIBS}
    ${N_DIRECTX_LIBS}
)
