#include <crow.h>
#include <sqlite3.h>
#include "./include/api/users/clients_api_verification.hpp"
#include "./include/api/admin/admin_remove_key.hpp"
#include "./include/api/admin/admin_api_add.hpp"
#include "./include/api/admin/admin_api_verify.hpp"
#include "./include/api/authenticate/login.hpp"
#include "./include/api/authenticate/register.hpp"


struct api_key:crow::ILocalMiddleware{
    struct context {};
    
    void before_handle(crow::request& req,crow::response& res,context& ctx){
        std::string passed_key=req.get_header_value("user-api-key");
        bool check=api_verify(passed_key);
        CROW_LOG_INFO<<passed_key;
        if(!check){
            res.code=401;
            res.set_header("WWW-Authenticate","API key required");
            res.body="Invalid Api Key";
            res.end();    
        }
    }

    void after_handle(crow::request& req,crow::response& res,context& ctx){}
};

struct api_key_admin:crow::ILocalMiddleware{
    struct context {};
    
    void before_handle(crow::request& req,crow::response& res,context& ctx){
        std::string passed_key=req.get_header_value("admin-api-key");
        CROW_LOG_INFO<<"You Passed Key Is :-->"<<passed_key;
        bool check=admin_api_verify(passed_key);
        if(!check){
            res.code=401;
            res.set_header("WWW-Authenticate","API key required");
            res.body="Invalid Api Key";
            res.end();    
        }
    }

    void after_handle(crow::request& req,crow::response& res,context& ctx){}
};


int main(){
    crow::App<api_key,api_key_admin> app;

    crow::Blueprint bp("api");
    bp.CROW_MIDDLEWARES(app,api_key);

    crow::Blueprint bp_admin("admin");
    bp_admin.CROW_MIDDLEWARES(app,api_key_admin);

    CROW_BP_ROUTE(bp,"/")([](){
        return "Your Api Key Is Valid !";
    });

    CROW_BP_ROUTE(bp,"/register").methods(crow::HTTPMethod::GET,crow::HTTPMethod::POST)([](crow::request& req,crow::response& res){
        auto parameters=req.get_body_params();
        std::string username_post=parameters.get("username");
        std::string password_post=parameters.get("password");
        std::string email_post=parameters.get("email");
        bool IsRegistered=Register(username_post,password_post,email_post);
        if(IsRegistered){
            res.code=200;
            res.write("Registered");
            res.end();
        }
        else{
            res.code=404;
            res.write("Can't Register Or Username Used");
            res.end();            
        }

    });

    CROW_BP_ROUTE(bp,"/login").methods(crow::HTTPMethod::GET,crow::HTTPMethod::POST)([](crow::request& req,crow::response& res){
        auto parameters=req.get_body_params();
        std::string username_post=parameters.get("username");
        std::string password_post=parameters.get("password");
        std::string Login_Str=Login(username_post,password_post);
        if(Login_Str=="Wrong Username"){
            res.code=401;
            res.write("Wrong Username");
            res.end();

        }
        else if(Login_Str=="Wrong Password"){
            res.code=404;
            res.write("Wrong Password");
            res.end();

        }
        else if(Login_Str=="Correct Password"){
            res.code=200;
            res.write("Correct Password");
            res.end();

        }
        else{
            res.code=404;
            res.write("Error!");
            res.end();
        }
   
    });

    CROW_BP_ROUTE(bp_admin,"/")([](){
        return "Verified";
    });

    // CROW_BP_ROUTE(bp_admin,"/add_api_key").methods(crow::HTTPMethod::GET,crow::HTTPMethod::POST)([](crow::request& req,crow::response& res,std::string username,std::string email_id,std::string description,std::string id){
    //     CROW_LOG_INFO<<"PAssed";
    //     auto parameters=req.get_body_params();
    //     std::string username_post=parameters.get("username");
    //     // std::string password_post=parameters.get("password");
    //     std::string api_key_of_user=add_key(username,email_id,description,id_int);
    //     res.code=200;
    //     res.write(api_key_of_user);
    //     res.end();
    // });

    //     CROW_BP_ROUTE(bp_admin,"/remove_api_key/<string>/<string>")([](crow::request& req,crow::response& res,std::string api_key,std::string username){
    //     bool isDeleted=remove_key(username,api_key);
    //     if(isDeleted==true){
    //         res.code=200;
    //         res.write("Deleted");
    //         res.end();
    //     }
    //     else{
    //     res.code=200;
    //     res.write("Wrong Username Or API Key Please Check Again");
    //     res.end();
    //     }

    // });

    CROW_ROUTE(app,"/")([](){
        return "To Short URL /api/short And To Get Full /api/full";
    });

    app.register_blueprint(bp);
    app.register_blueprint(bp_admin);
    app.port(8081).multithreaded().run();
}   