INCLUDE_DIRECTORIES(${Nebula3_SOURCE_DIR}/foundation
                    ${Nebula3_SOURCE_DIR}/extlibs
                    ${Nebula3_SOURCE_DIR}/tests
)

ADD_LIBRARY(Nebula3TestBase
    testcase.cc
    testcase.h
    testrunner.cc
    testrunner.h
)

