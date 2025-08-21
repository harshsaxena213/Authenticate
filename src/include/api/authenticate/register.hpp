#include <crow.h>
#include <sqlite3.h>
#include <openssl/sha.h>
#include <iostream>
#include <random>
#include <string>

bool Register(std::string username,std::string password,std::string email){

    sqlite3* db;
    int exit=sqlite3_open("db.sqlite",&db);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"ERROR WHILE OPENING THE DATABSE";
        sqlite3_close(db);
        return false;
    }
    
    std::string query="INSERT INTO USERS_DATA VALUES(?,?,?)";
    sqlite3_stmt* final_statement=nullptr;
    exit=sqlite3_prepare_v2(db,query.c_str(),-1,&final_statement,nullptr);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"Erro While Preparing The Query"<<sqlite3_errmsg(db);
        sqlite3_finalize(final_statement);
        sqlite3_close(db);
        return false;
    }

    std::string salted=password;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)salted.data(),salted.size(), hash);
    std::stringstream ss;
    for (unsigned char c : hash)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    std::string password_hash = ss.str();

    std::string query_username_check="SELECT * FROM USERS_DATA WHERE USERNAME=?";
    sqlite3_stmt* username_check=nullptr;
    int exit_username_check=sqlite3_prepare_v2(db,query_username_check.c_str(),-1,&username_check,nullptr);
    if(exit_username_check!=SQLITE_OK){
        CROW_LOG_INFO<<"Erro While Executing The Statemetn"<<sqlite3_errmsg(db)<<sqlite3_errcode(db);
        sqlite3_finalize(final_statement);
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(username_check, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    exit_username_check=sqlite3_step(username_check);
    if(exit_username_check==SQLITE_ROW){
        sqlite3_finalize(username_check);
        sqlite3_close(db);
        return false;

    }

    sqlite3_bind_text(final_statement, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(final_statement, 2, password_hash.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(final_statement, 3, email.c_str(), -1, SQLITE_TRANSIENT);

    CROW_LOG_INFO<<username.c_str()<<password_hash.c_str()<<email.c_str();

    exit=sqlite3_step(final_statement);
    if(exit!=SQLITE_DONE){
        CROW_LOG_INFO<<"Erro While Executing The Statemetn"<<sqlite3_errmsg(db)<<sqlite3_errcode(db);
        sqlite3_finalize(final_statement);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(final_statement);
    sqlite3_close(db);

    return true;
}