#-------------------------------------------------------------------------------
# addons/nebula2
#-------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}/
    ${Nebula3_SOURCE_DIR}/foundation/
    ${Nebula3_SOURCE_DIR}/extlibs/
    ${Nebula3_SOURCE_DIR}/render/
    ${Nebula3_SOURCE_DIR}/addons/
)
SET(addons_nebula2_anim_src
    anim/animtable.cc
    anim/animtable.h
    anim/managedanimation.cc
    anim/managedanimation.h
    anim/nanimation.h
    anim/nanimation_main.cc
    anim/nanimationserver.h
    anim/nanimationserver_main.cc
    anim/nanimclip.h
    anim/nanimclipscheduler.cc
    anim/nanimclipscheduler.h
    anim/nmemoryanimation.h
    anim/nmemoryanimation_main.cc
    anim/nmetadata.h
)
SET(addons_nebula2_character_src
    character/characterrenderer.cc
    character/characterrenderer.h
    character/characterserver.cc
    character/characterserver.h
    character/charfragment.h
    character/charvariation.h
    character/ncharacter2.cc
    character/ncharacter2.h
    character/ncharacter3set.cc
    character/ncharacter3set.h
    character/ncharjoint.h
    character/ncharjointpalette.h
    character/ncharskeleton.h
    character/skininfo.h
)
SET(addons_nebula2_character_base_src
    character/base/characterrendererbase.cc
    character/base/characterrendererbase.h
)
SET(addons_nebula2_character_win360_src
    character/win360/d3d9characterrenderer.cc
    character/win360/d3d9characterrenderer.h
)
SET(addons_nebula2_pch_src
    ../stdneb.cc
    ../stdneb.h
)
SET(addons_nebula2_SRCS
    ${addons_nebula2_anim_src}
    ${addons_nebula2_character_src}
    ${addons_nebula2_character_base_src}
    ${addons_nebula2_character_win360_src}
    ${addons_nebula2_pch_src}
    nebula2wrapper.h
)

IF(WIN32)
    SET(addons_nebula2_SRCS
        ${addons_nebula2_SRCS}
        ${addons_nebula2_character_win360_src}
    )
ENDIF()

SOURCE_GROUP(nebula2 FILES nebula2wrapper.h})
SOURCE_GROUP(nebula2\\anim FILES ${addons_nebula2_anim_src})
SOURCE_GROUP(nebula2\\character FILES ${addons_nebula2_character_src})
SOURCE_GROUP(nebula2\\character\\base FILES ${addons_nebula2_character_base_src})
SOURCE_GROUP(precompiled FILES ${addons_nebula2_pch_src})
IF(WIN32)
    SOURCE_GROUP(nebula2\\character\\win360 FILES ${addons_nebula2_character_win360_src})
ENDIF()

ADD_LIBRARY(nebula2 STATIC ${addons_nebula2_SRCS})
N_SET_COMPILER_DEFAULTS()
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(nebula2 stdneb.h stdneb.cc ../)
ENDIF()
ADD_DEPENDENCIES(nebula2 foundation)
