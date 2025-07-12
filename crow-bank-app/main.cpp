#include "crow_all.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <map>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

// In-memory user storage
struct User
{
    std::string email;
    std::string password;
    std::string name;
    double balance;
};

std::map<std::string, User> users = {
    {"john@bank.com", {"john@bank.com", "password123", "John Doe", 5847.32}},
    {"jane@bank.com", {"jane@bank.com", "password456", "Jane Smith", 12450.89}}};

std::map<std::string, std::string> sessions; // token -> email

std::string get_executable_dir()
{
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string exe_path(path);
    size_t last_slash = exe_path.find_last_of("\\/");
    return exe_path.substr(0, last_slash);
#else
    std::string exe_path = std::filesystem::canonical("/proc/self/exe");
    return std::filesystem::path(exe_path).parent_path();
#endif
}

std::string get_file_path(const std::string &relative_path)
{
    static std::string exe_dir = get_executable_dir();
    return exe_dir + "/" + relative_path;
}

void open_browser(const std::string &url)
{
#ifdef _WIN32
    ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#else
    std::string command = "xdg-open " + url + " 2>/dev/null &";
    system(command.c_str());
#endif
}

std::string read_file(const std::string &filename)
{
    std::string file_path = get_file_path("public/" + filename);
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open " << file_path << std::endl;
        return "Error: Could not load " + filename + ". Make sure public folder exists in the same directory as the executable.";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string generate_token()
{
    return "token_" + std::to_string(rand()) + "_" + std::to_string(time(nullptr));
}

int main()
{
    crow::SimpleApp app;
    std::string exe_dir = get_executable_dir();
    std::cout << "📂 Executable directory: " << exe_dir << std::endl;

    // Check if public folder exists using absolute path
    std::string index_path = get_file_path("public/index.html");
    std::ifstream test_file(index_path);
    if (!test_file.is_open())
    {
        std::cerr << "⚠️  Warning: " << index_path << " not found!" << std::endl;
        std::cerr << "   This usually happens when Task Scheduler runs from wrong directory" << std::endl;
    }
    else
    {
        std::cout << "✅ Found: " << index_path << std::endl;
        test_file.close();
    }

    // Route for the main page (index/signup)
    CROW_ROUTE(app, "/")
    ([](const crow::request &req)
     { return crow::response(200, read_file("index.html")); });

    // Route for login page
    CROW_ROUTE(app, "/login")
    ([](const crow::request &req)
     { return crow::response(200, read_file("login.html")); });

    // Route for dashboard page
    CROW_ROUTE(app, "/dashboard")
    ([](const crow::request &req)
     { return crow::response(200, read_file("dashboard.html")); });

    // API route for login
    CROW_ROUTE(app, "/api/login").methods("POST"_method)([](const crow::request &req)
                                                         {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "{\"success\": false, \"message\": \"Invalid JSON\"}");
        }

        std::string email = body["email"].s();
        std::string password = body["password"].s();

        // Check credentials
        auto user_it = users.find(email);
        if (user_it != users.end() && user_it->second.password == password) {
            // Generate token
            std::string token = generate_token();
            sessions[token] = email;
            
            crow::json::wvalue response;
            response["success"] = true;
            response["token"] = token;
            response["message"] = "Login successful";
            
            return crow::response(200, response.dump());
        } else {
            crow::json::wvalue response;
            response["success"] = false;
            response["message"] = "Invalid email or password";
            
            return crow::response(401, response.dump());
        } });

    // API route for user data
    CROW_ROUTE(app, "/api/user-data")
    ([](const crow::request &req)
     {
        std::string auth_header = req.get_header_value("Authorization");
        if (auth_header.substr(0, 7) != "Bearer ") {
            return crow::response(401, "{\"error\": \"Unauthorized\"}");
        }
        
        std::string token = auth_header.substr(7);
        auto session_it = sessions.find(token);
        if (session_it == sessions.end()) {
            return crow::response(401, "{\"error\": \"Invalid token\"}");
        }
        
        std::string email = session_it->second;
        auto user_it = users.find(email);
        if (user_it == users.end()) {
            return crow::response(404, "{\"error\": \"User not found\"}");
        }
        
        crow::json::wvalue response;
        response["name"] = user_it->second.name;
        response["email"] = user_it->second.email;
        response["balance"] = user_it->second.balance;
        
        return crow::response(200, response.dump()); });

    // Route for CSS file
    CROW_ROUTE(app, "/style.css")
    ([](const crow::request &req)
     { return crow::response(200, read_file("style.css")); });

    // Start the server in a separate thread so we can open browser after startup
    std::thread server_thread([&app]()
                              { app.port(8080).multithreaded().run(); });

    // Wait a moment for server to start, then open browser
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "🌐 Opening browser..." << std::endl;
    open_browser("http://localhost:8080");

    // Keep the main thread alive
    server_thread.join();
}
