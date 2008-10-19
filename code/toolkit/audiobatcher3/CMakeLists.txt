#-------------------------------------------------------------------------------
# toolkit/audiobatcher3
#-------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}/foundation/
    ${Nebula3_SOURCE_DIR}/toolkit/
)
SET(toolkit_audiobatcher3_src
    audiobatcher3.cc
    audiobatcherapp.cc
    audiobatcherapp.h
)
SET(toolkit_audiobatcher3_pch_src
    ../stdneb.cc
)
SET(tools_audiobatcher3_SRCS
    ${toolkit_audiobatcher3_src}
    ${toolkit_audiobatcher3_pch_src}
)

SOURCE_GROUP(audiobatcher3 FILES ${toolkit_audiobatcher3_src})
SOURCE_GROUP(precompiled FILES ${toolkit_audiobatcher3_pch_src})

ADD_EXECUTABLE(audiobatcher3 ${tools_audiobatcher3_SRCS})
N_SET_COMPILER_DEFAULTS()
N_SET_EXE_LINKER_DEFAULTS(audiobatcher3)
N_SET_EXE_SUBSYSTEM(audiobatcher3 Console)
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(audiobatcher3 stdneb.h stdneb.cc ../)
ENDIF()
ADD_DEPENDENCIES(audiobatcher3 toolkitutil)
TARGET_LINK_LIBRARIES(audiobatcher3 
    foundation
    ${N_EXTRA_WIN32_LIBS}
    ${N_DIRECTX_LIBS}
)
