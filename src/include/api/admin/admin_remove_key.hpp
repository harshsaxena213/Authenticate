#include <crow.h>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <random>

bool remove_key(std::string username,std::string api_key){
    sqlite3* db;
    int exit=sqlite3_open("db.sqlite",&db);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"Erro While Opening The Database";
        return false;
    
    }
    const char* query="SELECT * FROM api_keys WHERE username=? AND api_key=?";
    sqlite3_stmt* final_statement=nullptr;
    exit=sqlite3_prepare_v2(db,query,-1,&final_statement,nullptr);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"Erro While Preparing The Database"<<sqlite3_errmsg(db);
        return false;
    }
    sqlite3_bind_text(final_statement ,1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(final_statement, 2, api_key.c_str(), -1, SQLITE_TRANSIENT);
    exit=sqlite3_step(final_statement);
    if(exit==SQLITE_ROW){
        const char* delete_query = "DELETE FROM api_keys WHERE username=? AND api_key=?;";
        sqlite3_stmt* delete_stmt = nullptr;
        int rc = sqlite3_prepare_v2(db, delete_query, -1, &delete_stmt, nullptr);
        if (rc != SQLITE_OK) {
                CROW_LOG_ERROR << "Failed to prepare DELETE statement: " << sqlite3_errmsg(db);
                return false;
                }
        else{
                sqlite3_bind_text(delete_stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(delete_stmt, 2, api_key.c_str(), -1, SQLITE_TRANSIENT);
                rc = sqlite3_step(delete_stmt);
    if (rc != SQLITE_DONE) {
        CROW_LOG_ERROR << "Failed to execute DELETE: " << sqlite3_errmsg(db);
                return false;

    }
    else{
        return true;
    }
        }
    }
}