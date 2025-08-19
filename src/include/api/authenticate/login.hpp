#include <crow.h>
#include <sqlite3.h>
#include <random>
#include <string>
#include <iostream>
#include <openssl/sha.h>


struct IsLogedIn{
    bool isLooged=false;
    bool WrongPassword=false;
    bool WrongUsername=false;
};


std::string Login(std::string username,std::string password){
    IsLogedIn cb_data;
    sqlite3* db;
    int exit=sqlite3_open("db.sqlite",&db);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"Error WHile Opening The DataBase";
    }
    
    const char* query="SELECT password_hash FROM USERS_DATA WHERE USERNAME=?;";
    sqlite3_stmt* final_query=nullptr;
    exit=sqlite3_prepare_v2(db,query,-1,&final_query,nullptr);
    if(exit!=SQLITE_OK){
        CROW_LOG_INFO<<"Error WHile Preapering The Statement";
    }

    sqlite3_bind_text(final_query,1,username.c_str(),-1,SQLITE_TRANSIENT);
    int execute=sqlite3_step(final_query);

    if(execute==SQLITE_ROW){
        const unsigned char* db_hash = sqlite3_column_text(final_query, 0);
        if(!db_hash){
            cb_data.WrongUsername=true;
            sqlite3_finalize(final_query);
            sqlite3_close(db);
             return "Wrong Username";
        }

        
        std::string stored_hash(reinterpret_cast<const char*>(db_hash));
        std::string computed_salted=username+password;
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((const unsigned char*)computed_salted.data(),computed_salted.size(), hash);
        std::stringstream ss;
        for (unsigned char c : hash)
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
        std::string computed_password_hash = ss.str();

        if(stored_hash!=computed_password_hash){
            cb_data.WrongPassword=true;
            sqlite3_finalize(final_query);
            sqlite3_close(db);
            return "Wrong Password";
        }
        else if (stored_hash==computed_password_hash)
        {
            return "Correct Password";
            sqlite3_finalize(final_query);
            sqlite3_close(db);
        }
        else{
            return "Error Occured";
        }
        
    }
    
}
