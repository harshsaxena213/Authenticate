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
        return false;
    }
    
    std::string query="INSERT INTO USERS_DATA VALUES(?,?,?)";
    sqlite3_stmt* final_statement=nullptr;
    exit=sqlite3_prepare_v2(db,query.c_str(),-1,&final_statement,nullptr);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"Erro While Preparing The Query"<<sqlite3_errmsg(db);
        return false;
    }

    std::string salted=password+username;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)salted.data(),salted.size(), hash);
    std::stringstream ss;
    for (unsigned char c : hash)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    std::string hashed_password = ss.str();

    sqlite3_bind_text(final_statement, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(final_statement, 2, hashed_password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(final_statement, 5, email.c_str(), -1, SQLITE_TRANSIENT);


    exit=sqlite3_step(final_statement);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"Erro While Executing The Statemetn";
        return false;
    }
    
    return true;
}