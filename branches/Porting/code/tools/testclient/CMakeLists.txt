INCLUDE_DIRECTORIES(${Nebula3_SOURCE_DIR} ${Nebula3_SOURCE_DIR}/foundation)

IF(APPLE)
    FIND_LIBRARY(CORE_FOUNDATION_LIBRARY CoreFoundation)
    MARK_AS_ADVANCED(CORE_FOUNDATION_LIBRARY)
    SET(EXTRA_LIBS ${CORE_FOUNDATION_LIBRARY})
ENDIF(APPLE)

LINK_LIBRARIES(Nebula3Foundation Nebula3ZLib Nebula3Lua ${EXTRA_LIBS})
ADD_EXECUTABLE(testclient
    testclient.cc
    testclientapplication.cc
    testclientapplication.h
)
