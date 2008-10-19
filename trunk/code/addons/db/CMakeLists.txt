#-------------------------------------------------------------------------------
# DB
#-------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}/
    ${Nebula3_SOURCE_DIR}/foundation/
    ${Nebula3_SOURCE_DIR}/extlibs/
    ${Nebula3_SOURCE_DIR}/addons/
)
SET(addons_db_sqlite3_src
    sqlite3/sqlite3.h
    sqlite3/sqlite3command.cc
    sqlite3/sqlite3command.h
    sqlite3/sqlite3database.cc
    sqlite3/sqlite3database.h
    sqlite3/sqlite3dataset.cc
    sqlite3/sqlite3dataset.h
    sqlite3/sqlite3factory.cc
    sqlite3/sqlite3factory.h
    sqlite3/sqlite3filterset.cc
    sqlite3/sqlite3filterset.h
    sqlite3/sqlite3table.cc
    sqlite3/sqlite3table.h
)
SET(addons_db_src
    column.cc
    column.h
    command.cc
    command.h
    database.cc
    database.h
    dataset.cc
    dataset.h
    dbfactory.cc
    dbfactory.h
    dbserver.cc
    dbserver.h
    filterset.cc
    filterset.h
    reader.cc
    reader.h
    relation.cc
    relation.h
    table.cc
    table.h
    valuetable.cc
    valuetable.h
    writer.cc
    writer.h
)
SET(addons_db_pch_src
    ../stdneb.cc
    ../stdneb.h
)
SET(addons_db_SRCS
    ${addons_db_sqlite3_src}
    ${addons_db_src}
    ${addons_db_pch_src}
)
SOURCE_GROUP(db FILES ${addons_db_src})
SOURCE_GROUP(db\\sqlite3 FILES ${addons_db_sqlite3_src})
SOURCE_GROUP(precompiled FILES ${addons_db_pch_src})

ADD_LIBRARY(db STATIC ${addons_db_SRCS})
N_SET_COMPILER_DEFAULTS()
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(db stdneb.h stdneb.cc ../)
ENDIF()
ADD_DEPENDENCIES(db foundation sqlite)
