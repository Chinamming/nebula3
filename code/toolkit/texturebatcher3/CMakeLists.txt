#-------------------------------------------------------------------------------
# toolkit/texturebatcher3
#-------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}/foundation/
    ${Nebula3_SOURCE_DIR}/toolkit/
)
SET(toolkit_texturebatcher3_src
    texturebatcher3.cc
    texturebatcherapp.cc
    texturebatcherapp.h
)
SET(toolkit_texturebatcher3_pch_src
    ../stdneb.cc
)
SET(tools_texturebatcher3_SRCS
    ${toolkit_texturebatcher3_src}
    ${toolkit_texturebatcher3_pch_src}
)

SOURCE_GROUP(texturebatcher3 FILES ${toolkit_texturebatcher3_src})
SOURCE_GROUP(precompiled FILES ${toolkit_texturebatcher3_pch_src})

ADD_EXECUTABLE(texturebatcher3 ${tools_texturebatcher3_SRCS})
N_SET_COMPILER_DEFAULTS()
IF(WIN32)
    ADD_DEFINITIONS(/D_WIN32)
ENDIF()
N_SET_EXE_LINKER_DEFAULTS(texturebatcher3)
N_SET_EXE_SUBSYSTEM(texturebatcher3 Console)
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(texturebatcher3 stdneb.h stdneb.cc ../)
ENDIF()
ADD_DEPENDENCIES(texturebatcher3 toolkitutil)
TARGET_LINK_LIBRARIES(texturebatcher3 
    foundation
    devil
    ${N_EXTRA_WIN32_LIBS}
    ${N_DIRECTX_LIBS}
)
