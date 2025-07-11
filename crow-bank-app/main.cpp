#include "crow_all.h"
#include <fstream>

int main()
{
    crow::SimpleApp app;

    // Serve the main HTML file
    CROW_ROUTE(app, "/")([]() {
        std::ifstream file("public/index.html");
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    });

    // API: Check voting eligibility
    CROW_ROUTE(app, "/check_age").methods("POST"_method)([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400);

        int age = body["age"].i();
        std::string result = (age >= 18) ? "Eligible to vote" : "Not eligible";

        crow::json::wvalue res;
        res["message"] = result;
        return crow::response{res};
    });

    app.port(8080).multithreaded().run();
}
