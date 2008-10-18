//------------------------------------------------------------------------------
//  sqlite3database.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "addons/db/dataset.h"
#include "addons/db/sqlite3/sqlite3database.h"
#include "addons/db/sqlite3/sqlite3command.h"
#include "addons/db/sqlite3/sqlite3table.h"
#include "addons/db/sqlite3/sqlite3factory.h"
#include "io/ioserver.h"
#include "attr/attributedefinitionbase.h"

namespace Attr
{
    // some SQLite-internal names
    DefineString(name, 'name', ReadOnly);
    DefineString(type, 'type', ReadOnly);
    DefineInt(cid, 'cid_', ReadOnly);
    DefineInt(notnull, 'ntnl', ReadOnly);
    DefineBlob(dflt_value, 'dflv', ReadOnly);
    DefineBool(pk, 'pk__', ReadOnly);
    DefineInt(seq, 'seq_', ReadOnly);
    DefineBool(unique, 'uniq', ReadOnly);
    DefineInt(seqno, 'sqno', ReadOnly);
};

namespace Db
{
__ImplementClass(Db::Sqlite3Database, 'S3DB', Db::Database);

using namespace IO;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
Sqlite3Database::Sqlite3Database() :
    cacheNumPages(2000),
    tempStore(Memory),
    syncMode(false),
    busyTimeout(100),
    sqliteHandle(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Sqlite3Database::~Sqlite3Database()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
    Opens an Sqlite3 database (either opens an existing, or creates a 
    new database). Configures the database via pragmas, reads in existing
    tables.
*/
bool
Sqlite3Database::Open()
{
    n_assert(!this->IsOpen());
    n_assert(0 == this->sqliteHandle);
    n_assert(!this->uri.IsEmpty());
    if (Database::Open())
    {
        IoServer* ioServer = IoServer::Instance();

        // check if database file exists
        bool wasCreated = false;
        if (!ioServer->FileExists(this->uri))
        {
            // if we are in read only mode, fail
            if ((ReadOnly == this->accessMode) || (ReadWriteExisting == this->accessMode))
            {
                String err;
                err.Format("Sqlite3Database::Open(): DB file '%s' doesn't exist!\n", this->uri.AsString().AsCharPtr());
                this->SetError(err);
                return false;
            }
            else
            {
                // if database is created, make sure directory path is valid
                wasCreated = true;
                bool dirSuccess = ioServer->CreateDirectory(this->uri.AsString().ExtractDirName());
                if (!dirSuccess)
                {
                    String err;
                    err.Format("Sqlite3Database::Open(): Failed to create path for DB file '%s'!\n", this->uri.AsString().AsCharPtr());
                    this->SetError(err);
                    return false;
                }
            }
        }
                
        String nativePath = this->uri.AsString();
        nativePath.ANSItoUTF8();
        
        int err = SQLITE_OK;
        if (!this->memoryDatabase)
        {            
            err = sqlite3_open(nativePath.AsCharPtr(), &this->sqliteHandle);
        }
        else
        {
            err = sqlite3_open(":memory:", &this->sqliteHandle);
        }
        if (err != SQLITE_OK)
        {
            this->SetError(sqlite3_errmsg(this->sqliteHandle));
            this->sqliteHandle = 0;
            return false;
        }

        // configure the database
        err = sqlite3_busy_timeout(this->sqliteHandle, this->busyTimeout);
        if (err != SQLITE_OK)
        {
            this->SetError(sqlite3_errmsg(this->sqliteHandle));
            sqlite3_close(this->sqliteHandle);
            this->sqliteHandle = 0;
            return false;
        }
        
        String sql;
        sql.Format("PRAGMA cache_size=%d", this->cacheNumPages);
        Ptr<Command> cmd = DbFactory::Instance()->CreateCommand();
        cmd->CompileAndExecute(this, sql);

        if (this->syncMode) sql = "PRAGMA synchronous=ON";
        else sql = "PRAGMA synchronous=OFF";
        cmd->CompileAndExecute(this, sql);

        if (Memory == this->tempStore) sql = "PRAGMA temp_store=MEMORY";
        else sql = "PRAGMA temp_store=FILE";
        cmd->CompileAndExecute(this, sql);

        // compile begin and end transaction commands
        this->beginTransactionCmd = DbFactory::Instance()->CreateCommand();
        this->beginTransactionCmd->Compile(this, "BEGIN DEFERRED");
        this->endTransactionCmd = DbFactory::Instance()->CreateCommand();
        this->endTransactionCmd->Compile(this, "COMMIT");
        
        // for in-memory database we have to copy all tables from db on disk
        if (this->memoryDatabase)
        {
            // attach disk database file and copy all tables to memory db               
            this->AttachDatabase(nativePath, "fileDB");

            // query existing tables
            cmd = DbFactory::Instance()->CreateCommand();
            Ptr<ValueTable> result = Sqlite3Factory::Instance()->CreateValueTable();
            cmd->CompileAndExecute(this, "SELECT name FROM fileDB.sqlite_master WHERE type='table'", result);
            
            char* errMsg;
            IndexT i;
            for (i = 0; i < result->GetNumRows(); i++)
            {
                String sqlStatement("CREATE TABLE ");
                String newName(result->GetString(Attr::name, i));                
                sqlStatement.Append(newName);
                sqlStatement.Append(" AS SELECT * FROM fileDB.");
                sqlStatement.Append(result->GetString(Attr::name, i));
                sqlite3_exec(this->sqliteHandle, sqlStatement.AsCharPtr(), NULL, NULL, &errMsg);        
            }
            this->DetachDatabase("fileDB");
        }
        
        // read existing database tables        
        this->ReadTableLayouts();

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Close the database.
*/
void
Sqlite3Database::Close()
{
    n_assert(this->IsOpen());
    n_assert(0 != this->sqliteHandle);

    // first, call parent class this will disconnect all tables
    Database::Close();

    // release the transaction commands
    this->beginTransactionCmd = 0;
    this->endTransactionCmd = 0;

    // then close Sqlite database
    int err = sqlite3_close(this->sqliteHandle);
    if (err != SQLITE_OK)
    {
        this->SetError(sqlite3_errmsg(this->sqliteHandle));
        n_error("Sqlite3Database::Close(): Failed to close db '%s' with error '%s'\n", 
            this->uri.AsString().AsCharPtr(), this->error.AsCharPtr());
    }
    this->sqliteHandle = 0;
}

//------------------------------------------------------------------------------
/**
    Begin a database transaction.
*/
void
Sqlite3Database::BeginTransaction()
{
    if (!this->beginTransactionCmd->IsValid())
    {
        this->beginTransactionCmd->Compile(this, "BEGIN");
    }
    bool beginTransactionExecuted = this->beginTransactionCmd->Execute();
    n_assert(beginTransactionExecuted);
}

//------------------------------------------------------------------------------
/**
    End the current database transaction.
*/
void
Sqlite3Database::EndTransaction()
{
    if (!this->endTransactionCmd->IsValid())
    {
        this->endTransactionCmd->Compile(this, "COMMIT");
    }
    bool endTransactionExecuted = this->endTransactionCmd->Execute();
    n_assert(endTransactionExecuted);
}

//------------------------------------------------------------------------------
/**
    Registers additional attributes defined in the database's special
    _Attributes table. This enables an application to function even
    without knowing all attributes beforehand.

    FIXME: we could also check here if the attributes in the database
    are type-compatibel with the already existing attributes(?).
*/
void
Sqlite3Database::RegisterAttributes(Table* attrTable)
{
    n_assert(0 != attrTable);

    // create a dataset and read table contents
    Ptr<Dataset> dataset = attrTable->CreateDataset();
    dataset->AddAllTableColumns();
    dataset->PerformQuery();
    Ptr<ValueTable> values = dataset->Values();
    IndexT rowIndex;
    SizeT numRows = values->GetNumRows();
    for (rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        const Util::String& attrName = values->GetString(Attr::AttrName, rowIndex);        
        if (!Attr::AttrId::IsValidName(attrName))
        {
            Util::String valueString = values->GetString(Attr::AttrType, rowIndex);
        #if __NEBULA2_EXPORT__
            // nebula2 export support
            if (valueString == "vector3" || valueString == "vector4")
            {
                valueString = "float4";
            }
        #endif
            Attr::ValueType valueType = Attr::Attribute::StringToValueType(valueString);
            bool readWrite = values->GetBool(Attr::AttrReadWrite, rowIndex);
            Attr::AccessMode accessMode = readWrite ? Attr::ReadWrite : Attr::ReadOnly;
            Attr::AttributeDefinitionBase::RegisterDynamicAttribute(attrName, Util::FourCC(), valueType, accessMode);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Reads in the existing database table structure, and attaches them as
    Table objects. 
*/
void
Sqlite3Database::ReadTableLayouts()
{
    n_assert(0 == this->GetNumTables());

    // query existing tables
    Ptr<Command> cmd = DbFactory::Instance()->CreateCommand();
    Ptr<ValueTable> result = Sqlite3Factory::Instance()->CreateValueTable();
    cmd->CompileAndExecute(this, "SELECT name FROM sqlite_master WHERE type='table'", result);

    // first check if there is an _Attributes table, if it exists,
    // register all attributes in that table
    Util::Array<IndexT> attrTableRowIndices = result->FindRowIndicesByAttr(Attr::Attribute(Attr::name, "_Attributes"), true);
    if (!attrTableRowIndices.IsEmpty())
    {
        // connect the attributes table as usual...
        Ptr<Sqlite3Table> attrsTable = (Sqlite3Table*) Sqlite3Factory::Instance()->CreateTable().get();
        attrsTable->SetName(result->GetString(Attr::name, attrTableRowIndices[0]));
        this->tables.Append((Table*)attrsTable.get());
        attrsTable->Connect(this, Table::AssumeExists, this->ignoreUnknownColumns);

        // register all attributes in the table...
        this->RegisterAttributes(attrsTable);
    }

    // connect all other tables
    IndexT rowIndex;
    SizeT numRows = result->GetNumRows();
    for (rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        // create a table object and connect it to the database,
        // the table will read the table layout when it is connected
        if (attrTableRowIndices.IsEmpty() || rowIndex != attrTableRowIndices[0])
        {
            Ptr<Sqlite3Table> table = (Sqlite3Table*) Sqlite3Factory::Instance()->CreateTable().get();
        	table->SetName(result->GetString(Attr::name, rowIndex));
            this->tables.Append((Table*)table.get());
            table->Connect(this, Table::AssumeExists, this->ignoreUnknownColumns);
        }
    }
}

//------------------------------------------------------------------------------
/**
    This attaches an external database file to this database under the given
    name. See the SQLite docs for details!
*/
bool
Sqlite3Database::AttachDatabase(const URI& uri, const String& dbName)
{
    n_assert(this->IsOpen());
    n_assert(dbName.IsValid());

    // first check if the file actually exists
    IoServer* ioServer = IoServer::Instance();
    if (ioServer->FileExists(uri))
    {
        // SQLite wants a native path in UTF-8 format
        String nativePath = URI(ioServer->ResolveAssigns(this->uri.AsString())).GetTail();
        nativePath.ANSItoUTF8();
        
        // build and execute SQL command
        String sql;
        sql.Format("ATTACH DATABASE '%s' AS '%s'", nativePath.AsCharPtr(), dbName.AsCharPtr());
        Ptr<Command> cmd = DbFactory::Instance()->CreateCommand();
        bool dbAttached = cmd->CompileAndExecute(this, sql);
        if (!dbAttached)
        {
            n_error("Sqlite3Database::AttachDatabase(%s, %s) failed with sqlite error: %s",
                uri.AsString().AsCharPtr(), dbName.AsCharPtr(), cmd->GetError().AsCharPtr());
            return false;
        }
        return true;
    }
    else
    {
        n_error("Sqlite3Database::AttachDatabase: file '%s' doesn't exist!", uri.AsString().AsCharPtr());
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    This detaches an external database from this database. See the SQLite 
    docs for details!
*/
void
Sqlite3Database::DetachDatabase(const String& dbName)
{
    n_assert(this->IsOpen());
    n_assert(dbName.IsValid());
    String sql;
    sql.Format("DETACH DATABASE '%s'", dbName.AsCharPtr());
    Ptr<Command> cmd = DbFactory::Instance()->CreateCommand();
    bool dbDetached = cmd->CompileAndExecute(this, sql);
    if (!dbDetached)
    {
        n_error("Sqlite3Database::AttachDatabase(%s, %s) failed with sqlite error: %s",
            uri.AsString().AsCharPtr(), dbName.AsCharPtr(), cmd->GetError().AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Sqlite3Database::CopyInMemoryDatabaseToFile(const IO::URI& fileUri)
{
    String nativePath = fileUri.AsString();
    nativePath.ANSItoUTF8();
    // attach disk database file and copy all tables from memory db to file db
    this->AttachDatabase(nativePath, "fileDB");

    // query existing tables
    Ptr<Command> cmd = DbFactory::Instance()->CreateCommand();
    Ptr<ValueTable> result = Sqlite3Factory::Instance()->CreateValueTable();
    cmd->CompileAndExecute(this, "SELECT name FROM sqlite_master WHERE type='table'", result);
    
    char* errMsg;
    IndexT i;
    for (i = 0; i < result->GetNumRows(); i++)
    {
        String sqlStatement("CREATE TABLE fileDB.");
        sqlStatement.Append(result->GetString(Attr::name, i));
        sqlStatement.Append(" AS SELECT * FROM ");
        sqlStatement.Append(result->GetString(Attr::name, i));
        sqlite3_exec(this->sqliteHandle, sqlStatement.AsCharPtr(), NULL, NULL, &errMsg);
    }
    this->DetachDatabase("fileDB");
}

} // namespace Db