INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}
    ${Nebula3_SOURCE_DIR}/foundation
    ${Nebula3_SOURCE_DIR}/extlibs
)

IF(APPLE)
    FIND_LIBRARY(CORE_FOUNDATION_LIBRARY CoreFoundation)
    MARK_AS_ADVANCED(CORE_FOUNDATION_LIBRARY)
    SET(EXTRA_LIBS ${CORE_FOUNDATION_LIBRARY})
ENDIF(APPLE)

LINK_LIBRARIES(Nebula3Foundation Nebula3ZLib Nebula3Lua Nebula3TinyXML ${EXTRA_LIBS})
ADD_EXECUTABLE(idlc
    idlarg.cc
    idlarg.h
    idlc.cc
    idlcodegenerator.cc
    idlcodegenerator.h
    idlcommand.cc
    idlcommand.h
    idlcompiler.cc
    idlcompiler.h
    idldependency.cc
    idldependency.h
    idldocument.cc
    idldocument.h
    idllibrary.cc
    idllibrary.h
    idlmessage.cc
    idlmessage.h
    idlprotocol.cc
    idlprotocol.h
    stdneb.cc
)
