#include <crow.h>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <random>



bool add_key(std::string username){
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";


    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    sqlite3* db;
    int exit=sqlite3_open("db.sqlite",&db);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"Erro While Opening The Database";
        return false;
    
    }
       
    std::string api_key;
    for (size_t i = 0; i < 15; ++i) {
        api_key += characters[distribution(generator)];
    }
    int no_of_time = 0;



    const char* query="INSERT INTO admin_api_keys (api_key, username) VALUES (?, ?);";
    sqlite3_stmt* final_statement=nullptr;
    exit=sqlite3_prepare_v2(db,query,-1,&final_statement,nullptr);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"Erro While Preparing The Database"<<sqlite3_errmsg(db);
        return false;

    }
    



    sqlite3_bind_text(final_statement, 1, api_key.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(final_statement, 2, username.c_str(), -1, SQLITE_TRANSIENT);

    exit=sqlite3_step(final_statement);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"Erro While Executing The Statemetn";
        return false;

    
    }


    return true;
}
