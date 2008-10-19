#-------------------------------------------------------------------------------
# TinyXML
#-------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}/foundation/
    ${Nebula3_SOURCE_DIR}/extlibs/
)
SET(extlibs_tinyxml_src
    tinystr.cc
    tinystr.h
    tinyxml.cc
    tinyxml.h
    tinyxmlerror.cc
    tinyxmlparser.cc
)
SET(extlibs_tinyxml_pch_src
    ../stdneb.cc
    ../stdneb.h
)
SET(extlibs_tinyxml_SRCS
    ${extlibs_tinyxml_src}
    ${extlibs_tinyxml_pch_src}
)
SOURCE_GROUP(tinyxml FILES ${extlibs_tinyxml_src})
SOURCE_GROUP(precompiled FILES ${extlibs_tinyxml_pch_src})

ADD_LIBRARY(tinyxml STATIC ${extlibs_tinyxml_SRCS})
N_SET_COMPILER_DEFAULTS()
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(tinyxml stdneb.h stdneb.cc ../)
ENDIF()
ADD_DEPENDENCIES(tinyxml foundation)
