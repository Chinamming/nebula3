#-------------------------------------------------------------------------------
# toolkit/toolkitutil
#-------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}/foundation/
    ${Nebula3_SOURCE_DIR}/extlibs/
    ${Nebula3_SOURCE_DIR}/extlibs/DevIL/
    ${Nebula3_SOURCE_DIR}/toolkit/
)
SET(toolkit_toolkitutil_src
    applauncher.cc
    applauncher.h
    audioexporter.cc
    audioexporter.h
    imageconverter.cc
    imageconverter.h
    platform.cc
    platform.h
    projectinfo.cc
    projectinfo.h
    resourcedictionarybuilder.cc
    resourcedictionarybuilder.h
    shadercompiler.cc
    shadercompiler.h
    textureattrs.cc
    textureattrs.h
    textureattrtable.cc
    textureattrtable.h
    textureconverter.cc
    textureconverter.h
    toolkitapp.cc
    toolkitapp.h
)
SET(toolkit_toolkitutil_pch_src
    ../stdneb.cc
)
SET(toolkit_toolkitutil_SRCS
    ${toolkit_toolkitutil_src}
    ${toolkit_toolkitutil_pch_src}
)

SOURCE_GROUP(toolkitutil FILES ${toolkit_toolkitutil_src})
SOURCE_GROUP(precompiled FILES ${toolkit_toolkitutil_pch_src})

ADD_LIBRARY(toolkitutil STATIC ${toolkit_toolkitutil_SRCS})
N_SET_COMPILER_DEFAULTS()
ADD_DEFINITIONS(/DIL_STATIC_LIB)
IF(WIN32)
    ADD_DEFINITIONS(/D_WIN32)
ENDIF()
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(toolkitutil stdneb.h stdneb.cc ../)
ENDIF()
ADD_DEPENDENCIES(toolkitutil
    foundation
    tinyxml
    lua
    zlib
    devil
)
