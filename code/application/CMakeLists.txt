#-------------------------------------------------------------------------------
# application
#-------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}/
    ${Nebula3_SOURCE_DIR}/foundation/
    ${Nebula3_SOURCE_DIR}/extlibs/
    ${Nebula3_SOURCE_DIR}/extlibs/gimpact/
    ${Nebula3_SOURCE_DIR}/extlibs/opcode/
    ${Nebula3_SOURCE_DIR}/extlibs/ode/
    ${Nebula3_SOURCE_DIR}/addons/
    ${Nebula3_SOURCE_DIR}/render/
    ${Nebula3_SOURCE_DIR}/application/
    ${Nebula3_SOURCE_DIR}/application/basegamefeature/
    ${Nebula3_SOURCE_DIR}/application/graphicsfeature/
    ${Nebula3_SOURCE_DIR}/application/physicsfeature/
)
SET(application_appgame_src
    appgame/appconfig.h
    appgame/gameapplication.cc
    appgame/gameapplication.h
    appgame/gamestatehandler.cc
    appgame/gamestatehandler.h
    appgame/statehandler.cc
    appgame/statehandler.h
)
SET(application_basegamefeature_src
    basegamefeature/basegamefeatureunit.cc
    basegamefeature/basegamefeatureunit.h
    basegamefeature/basegameprotocol.cc
    basegamefeature/basegameprotocol.h
)
SET(application_basegamefeature_basegameattr_src
    basegamefeature/basegameattr/basegameattributes.cc
    basegamefeature/basegameattr/basegameattributes.h
)
SET(application_basegamefeature_basegametiming_src
    basegamefeature/basegametiming/gametimesource.cc
    basegamefeature/basegametiming/gametimesource.h
    basegamefeature/basegametiming/inputtimesource.cc
    basegamefeature/basegametiming/inputtimesource.h
    basegamefeature/basegametiming/systemtimesource.cc
    basegamefeature/basegametiming/systemtimesource.h
    basegamefeature/basegametiming/timemanager.cc
    basegamefeature/basegametiming/timemanager.h
    basegamefeature/basegametiming/timesource.cc
    basegamefeature/basegametiming/timesource.h
)
SET(application_basegamefeature_debug_src
    basegamefeature/debug/objectinspectorhandler.cc
    basegamefeature/debug/objectinspectorhandler.h
)
SET(application_basegamefeature_loader_src
    basegamefeature/loader/entityloader.cc
    basegamefeature/loader/entityloader.h
    basegamefeature/loader/entityloaderbase.cc
    basegamefeature/loader/entityloaderbase.h
    basegamefeature/loader/environmentloader.cc
    basegamefeature/loader/environmentloader.h
    basegamefeature/loader/levelloader.cc
    basegamefeature/loader/levelloader.h
    basegamefeature/loader/loaderserver.cc
    basegamefeature/loader/loaderserver.h
    basegamefeature/loader/userprofile.cc
    basegamefeature/loader/userprofile.h
)
SET(application_basegamefeature_managers_src
    basegamefeature/managers/categorymanager.cc
    basegamefeature/managers/categorymanager.h
    basegamefeature/managers/entitymanager.cc
    basegamefeature/managers/entitymanager.h
    basegamefeature/managers/enventitymanager.cc
    basegamefeature/managers/enventitymanager.h
    basegamefeature/managers/envquerymanager.cc
    basegamefeature/managers/envquerymanager.h
    basegamefeature/managers/factorymanager.cc
    basegamefeature/managers/factorymanager.h
    basegamefeature/managers/focusmanager.cc
    basegamefeature/managers/focusmanager.h
    basegamefeature/managers/globalattrsmanager.cc
    basegamefeature/managers/globalattrsmanager.h
)
SET(application_basegamefeature_properties_src
    basegamefeature/properties/timeproperty.cc
    basegamefeature/properties/timeproperty.h
    basegamefeature/properties/transformableproperty.cc
    basegamefeature/properties/transformableproperty.h
)
SET(application_game_src
    game/entity.cc
    game/entity.h
    game/featureunit.cc
    game/featureunit.h
    game/gameserver.cc
    game/gameserver.h
    game/manager.cc
    game/manager.h
    game/property.cc
    game/property.h
)
SET(application_graphicsfeature_src
    graphicsfeature/graphicsfeatureproperties.h
    graphicsfeature/graphicsfeatureunit.cc
    graphicsfeature/graphicsfeatureunit.h
    graphicsfeature/graphicsprotocol.cc
    graphicsfeature/graphicsprotocol.h
)
SET(application_graphicsfeature_graphicsattr_src
    graphicsfeature/graphicsattr/graphicsattributes.cc
    graphicsfeature/graphicsattr/graphicsattributes.h
)
SET(application_graphicsfeature_graphicsutil_src
    graphicsfeature/graphicsutil/lightflickerutil.cc
    graphicsfeature/graphicsutil/lightflickerutil.h
    graphicsfeature/graphicsutil/segmentedgfxutil.cc
    graphicsfeature/graphicsutil/segmentedgfxutil.h
)
SET(application_graphicsfeature_properties_src
    graphicsfeature/properties/cameraproperty.cc
    graphicsfeature/properties/cameraproperty.h
    graphicsfeature/properties/chasecameraproperty.cc
    graphicsfeature/properties/chasecameraproperty.h
    graphicsfeature/properties/environmentgraphicsproperty.cc
    graphicsfeature/properties/environmentgraphicsproperty.h
    graphicsfeature/properties/graphicsproperty.cc
    graphicsfeature/properties/graphicsproperty.h
    graphicsfeature/properties/inputproperty.cc
    graphicsfeature/properties/inputproperty.h
    graphicsfeature/properties/lightproperty.cc
    graphicsfeature/properties/lightproperty.h
    graphicsfeature/properties/mayacameraproperty.cc
    graphicsfeature/properties/mayacameraproperty.h
)
SET(application_physicsfeature_src
    physicsfeature/physicsfeatureunit.cc
    physicsfeature/physicsfeatureunit.h
    physicsfeature/physicsprotocol.cc
    physicsfeature/physicsprotocol.h
)
SET(application_physicsfeature_physicsattr_src
    physicsfeature/physicsattr/physicsattributes.cc
    physicsfeature/physicsattr/physicsattributes.h
)
SET(application_physicsfeature_properties_src
    physicsfeature/properties/actorphysicsproperty.cc
    physicsfeature/properties/actorphysicsproperty.h
    physicsfeature/properties/environmentcollideproperty.cc
    physicsfeature/properties/environmentcollideproperty.h
    physicsfeature/properties/mousegripperproperty.cc
    physicsfeature/properties/mousegripperproperty.h
    physicsfeature/properties/physicsproperty.cc
    physicsfeature/properties/physicsproperty.h
    physicsfeature/properties/triggerproperty.cc
    physicsfeature/properties/triggerproperty.h
)
SET(application_uifeature_src
    uifeature/uievent.cc
    uifeature/uievent.h
    uifeature/uieventhandler.cc
    uifeature/uieventhandler.h
    uifeature/uifactory.cc
    uifeature/uifactory.h
    uifeature/uifeatureunit.cc
    uifeature/uifeatureunit.h
    uifeature/uiserver.cc
    uifeature/uiserver.h
    uifeature/uixmlparser.cc
    uifeature/uixmlparser.h
)
SET(application_uifeature_elements_src
    uifeature/elements/button.cc
    uifeature/elements/button.h
    uifeature/elements/canvas.cc
    uifeature/elements/canvas.h
    uifeature/elements/control.cc
    uifeature/elements/control.h
    uifeature/elements/element.cc
    uifeature/elements/element.h
    uifeature/elements/label.cc
    uifeature/elements/label.h
    uifeature/elements/window.cc
    uifeature/elements/window.h
)
SET(application_pch_src
    stdneb.cc
    stdneb.h
)
SET(application_SRCS
    ${application_appgame_src}
    ${application_basegamefeature_basegameattr_src}
    ${application_basegamefeature_basegametiming_src}
    ${application_basegamefeature_debug_src}
    ${application_basegamefeature_loader_src}
    ${application_basegamefeature_managers_src}
    ${application_basegamefeature_properties_src}
    ${application_basegamefeature_src}
    ${application_game_src}
    ${application_graphicsfeature_graphicsattr_src}
    ${application_graphicsfeature_graphicsutil_src}
    ${application_graphicsfeature_properties_src}
    ${application_graphicsfeature_src}
    ${application_physicsfeature_src}
    ${application_physicsfeature_physicsattr_src}
    ${application_physicsfeature_properties_src}
    ${application_uifeature_src}
    ${application_uifeature_elements_src}
    ${application_pch_src}
)
SOURCE_GROUP(appgame FILES ${application_appgame_src})
SOURCE_GROUP(basegamefeature FILES ${application_basegamefeature_src})
SOURCE_GROUP(basegamefeature\\basegameattr FILES ${application_basegamefeature_basegameattr_src})
SOURCE_GROUP(basegamefeature\\basegametiming FILES ${application_basegamefeature_basegametiming_src})
SOURCE_GROUP(basegamefeature\\debug FILES ${application_basegamefeature_debug_src})
SOURCE_GROUP(basegamefeature\\loader FILES ${application_basegamefeature_loader_src})
SOURCE_GROUP(basegamefeature\\managers FILES ${application_basegamefeature_managers_src})
SOURCE_GROUP(basegamefeature\\properties FILES ${application_basegamefeature_properties_src})
SOURCE_GROUP(game FILES ${application_game_src})
SOURCE_GROUP(graphicsfeature FILES ${application_graphicsfeature_src})
SOURCE_GROUP(graphicsfeature\\graphicsattr FILES ${application_graphicsfeature_graphicsattr_src})
SOURCE_GROUP(graphicsfeature\\graphicsutil FILES ${application_graphicsfeature_graphicsutil_src})
SOURCE_GROUP(graphicsfeature\\properties FILES ${${application_graphicsfeature_properties_src}})
SOURCE_GROUP(physicsfeature FILES ${application_physicsfeature_src})
SOURCE_GROUP(physicsfeature\\physicsattr FILES ${application_physicsfeature_physicsattr_src})
SOURCE_GROUP(physicsfeature\\properties FILES ${application_physicsfeature_properties_src})
SOURCE_GROUP(uifeature FILES ${application_uifeature_src})
SOURCE_GROUP(uifeature\\elements FILES ${application_uifeature_elements_src})
SOURCE_GROUP(precompiled FILES ${application_pch_src})


ADD_LIBRARY(application STATIC ${application_SRCS})
N_SET_COMPILER_DEFAULTS()
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(application stdneb.h stdneb.cc)
ENDIF()
ADD_DEPENDENCIES(application render)
