CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Nebula3)

# for makefile based builds default to release builds
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

IF(WIN32)
    SET(N_DIRECTX_LIBS "dxguid.lib d3d9.lib d3dx9.lib dinput8.lib xinput.lib dxerr9.lib")
    SET(N_EXTRA_WIN32_LIBS "dbghelp.lib wsock32.lib rpcrt4.lib wininet.lib")
ENDIF()

#-------------------------------------------------------------------------------
# Settings that can be tweaked by the user prior to building.
#-------------------------------------------------------------------------------
IF(MSVC)
    OPTION(N_USE_PRECOMPILED_HEADERS "Use precompiled headers." ON)
ENDIF()

OPTION(N_BUILD_BENCHMARKS "Build Benchmarks." OFF)
OPTION(N_BUILD_TESTS "Build Tests." OFF)
OPTION(N_BUILD_TOOLKIT "Build Toolkit." OFF)
OPTION(N_BUILD_TOOLS "Build Tools." OFF)

#-------------------------------------------------------------------------------
# Sets up the default compiler options for a Nebula library target or executable.
#-------------------------------------------------------------------------------
MACRO(N_SET_COMPILER_DEFAULTS)
    IF(MSVC)        
        SET(CMAKE_CXX_FLAGS 
            "/D__WIN32__ /DWIN32 /D_HAS_EXCEPTIONS=0 /GF /fp:fast /W3 /WX /nologo /errorReport:prompt" 
            CACHE STRING "Flags used by the compiler during all build types." FORCE)
        SET(CMAKE_CXX_FLAGS_DEBUG
            "/D_DEBUG /Od /Gm /RTC1 /MTd /Gy /Wp64 /ZI"
            CACHE STRING "Flags used by the compiler during debug builds." FORCE)
        SET(CMAKE_CXX_FLAGS_RELEASE
            "/Ob2 /Oi /Os /Oy /GL /FD /MT /GS- /Zi"
            CACHE STRING "Flags used by the compiler during release builds." FORCE)
    ENDIF()
ENDMACRO(N_SET_COMPILER_DEFAULTS)

#-------------------------------------------------------------------------------
# Sets up the default linker options for a Nebula executable.
#-------------------------------------------------------------------------------
MACRO(N_SET_EXE_LINKER_DEFAULTS TargetName)
    IF(MSVC)
        SET(CMAKE_EXE_LINKER_FLAGS
            "/MANIFEST:NO"
            CACHE STRING "Flags used by the linker when building an executable." FORCE
        )
    ENDIF()
ENDMACRO(N_SET_EXE_LINKER_DEFAULTS)

#-------------------------------------------------------------------------------
# Sets the subsystem linker option on WIN32, when the subsystem is set to
# "Windows" the executable is expected to have a WinMain(), if "Console" is
# defined the executable is expected to have a main().
#-------------------------------------------------------------------------------
MACRO(N_SET_EXE_SUBSYSTEM TargetName SubsystemName)
    IF(WIN32)
        IF(${SubsystemName} STREQUAL Windows)
            SET_TARGET_PROPERTIES(${TargetName}
                PROPERTIES
                WIN32_EXECUTABLE ON
            )
        ELSEIF(${SubsystemName} STREQUAL Console)
            SET_TARGET_PROPERTIES(${TargetName}
                PROPERTIES
                WIN32_EXECUTABLE OFF
            )
        ENDIF()
    ENDIF()
ENDMACRO(N_SET_EXE_SUBSYSTEM)

#-------------------------------------------------------------------------------
# Sets up the default Nebula precompiled header for the given target.
# param TargetName is the name of a target that should use precompiled headers.
# See extlibs/tinyxml/CMakeLists.txt for an example.
#-------------------------------------------------------------------------------
MACRO(N_SET_PCH TargetName HeaderName SourceName)
    SET(SourceFilePath "")
    IF(${ARGC} STREQUAL 4)
        SET(SourceFilePath ${ARGV3})
    ENDIF()
    IF(MSVC)
        SET_TARGET_PROPERTIES(${TargetName}
            PROPERTIES
            COMPILE_FLAGS "/Yu${HeaderName} /Fp${CMAKE_CFG_INTDIR}/${TargetName}.pch")
        SET_SOURCE_FILES_PROPERTIES(${SourceFilePath}${SourceName}
            PROPERTIES
            COMPILE_FLAGS "/Yc${HeaderName}")
    ENDIF()
ENDMACRO(N_SET_PCH)

#-------------------------------------------------------------------------------
# The individual targets that make up Nebula3
#-------------------------------------------------------------------------------

ADD_SUBDIRECTORY(foundation)
ADD_SUBDIRECTORY(extlibs)
ADD_SUBDIRECTORY(addons)
IF(WIN32)
    ADD_SUBDIRECTORY(render)
    ADD_SUBDIRECTORY(application)
ENDIF()
#IF(N_BUILD_BENCHMARKS)
#    ADD_SUBDIRECTORY(benchmarks)
#ENDIF()
#IF(N_BUILD_TESTS)
#    ADD_SUBDIRECTORY(tests)
#ENDIF()
#IF(N_BUILD_TOOLKIT)
#    ADD_SUBDIRECTORY(toolkit)
#ENDIF()
#IF(N_BUILD_TOOLS)
#    ADD_SUBDIRECTORY(tools)
#ENDIF()
