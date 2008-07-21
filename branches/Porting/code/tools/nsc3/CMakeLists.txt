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
ADD_EXECUTABLE(nsc3
    nsc3.cc
    shader.cc
    shader.h
    shaderbinding.cc
    shaderbinding.h
    shadercodegenerator.cc
    shadercodegenerator.h
    shadercompiler.cc
    shadercompiler.h
    shadercompilerapplication.cc
    shadercompilerapplication.h
    shaderfragment.cc
    shaderfragment.h
    shaderfragmentgroup.cc
    shaderfragmentgroup.h
    shaderfragmentmanager.cc
    shaderfragmentmanager.h
    shadernode.cc
    shadernode.h
    shaderparam.cc
    shaderparam.h
    shadersampler.cc
    shadersampler.h
    shadersamplermanager.cc
    shadersamplermanager.h
    shaderslot.cc
    shaderslot.h
)
