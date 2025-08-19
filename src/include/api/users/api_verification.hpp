#include <crow.h>
#include <sqlite3.h>


struct ApiVerifyData {
    std::string passed_key;
    bool found=false;

};

static int api_verify_callback(void* data, int argc, char** argv, char** azColName){
    auto* cb_data=static_cast<ApiVerifyData*>(data);
    for (int i = 0; i < argc; i++) {
        std::string key=argv[i];
        if(key == cb_data->passed_key){
            cb_data->found=true;
            return 1;
        }
    }
    return 0;
    
}
    

bool api_verify(std::string passed_key){
    sqlite3* db;
    int exit=sqlite3_open("db.sqlite",&db);
    if(exit!=SQLITE_OK){
        std::cout<<"Error While Opening The DataBase!";
    }
    std::string query="SELECT password_hash FROM USERS_DATA ;";

    ApiVerifyData cb_data;
    cb_data.passed_key=passed_key;

    exit=sqlite3_exec(db,query.c_str(),api_verify_callback,&cb_data,nullptr);

    if(exit!=SQLITE_OK){
        std::cout<<"Error WHile Runnig Query!";
    }

    sqlite3_close(db);
    return cb_data.found;
}
