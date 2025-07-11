#include "crow_all.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

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
    // Windows: Use ShellExecute to open default browser
    ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#else
    // Linux: Use xdg-open
    std::string command = "xdg-open " + url;
    system(command.c_str());
#endif
}

int main()
{
    crow::SimpleApp app;

    // Serve the main HTML file
    CROW_ROUTE(app, "/")([]()
                         {
        std::string file_path = get_file_path("public/index.html");
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "❌ Error: Could not open " << file_path << std::endl;
            std::cerr << "   Task Scheduler working directory issue - using absolute path now" << std::endl;
            return std::string("Error: Could not load index.html from: " + file_path);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        
        if (content.empty()) {
            std::cerr << "❌ Warning: index.html appears to be empty" << std::endl;
            return std::string("Error: index.html is empty");
        }
        
        std::cout << "✅ Served index.html from: " << file_path << std::endl;
        return content; });

    // Serve static files (CSS, JS, etc.)
    CROW_ROUTE(app, "/style.css")([]()
                                  {
        std::string file_path = get_file_path("public/style.css");
        std::ifstream file(file_path);
        if (!file.is_open()) {
            return crow::response(404, "CSS file not found at: " + file_path);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        auto response = crow::response(buffer.str());
        response.set_header("Content-Type", "text/css");
        return response; });

    CROW_ROUTE(app, "/script.js")([]()
                                  {
        std::string file_path = get_file_path("public/script.js");
        std::ifstream file(file_path);
        if (!file.is_open()) {
            return crow::response(404, "JS file not found at: " + file_path);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        auto response = crow::response(buffer.str());
        response.set_header("Content-Type", "application/javascript");
        return response; });

    // API: Check voting eligibility
    CROW_ROUTE(app, "/check_age").methods("POST"_method)([](const crow::request &req)
                                                         {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400);

        int age = body["age"].i();
        std::string result = (age >= 18) ? "Eligible to vote" : "Not eligible";

        crow::json::wvalue res;
        res["message"] = result;
        return crow::response{res}; });

    std::cout << "🚀 Starting Crow Bank App server..." << std::endl;
    std::cout << "📍 Server will be available at: http://localhost:8080" << std::endl;

    // Show executable directory for debugging Task Scheduler issues
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
