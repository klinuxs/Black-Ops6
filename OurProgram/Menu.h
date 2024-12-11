#pragma once
#include "config.h"
#include <blops6/blackops6.h>
void Render(Config& config, Memory& mem, Game& game);
void logException(const std::exception& e, const std::string& logFilename = "Log.txt") {
    if (!std::filesystem::exists(logFilename)) {
        std::ofstream logFile(logFilename);
        logFile.close();
    }


    std::ofstream logFile(logFilename, std::ios::app);
    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);


        logFile << "Exception occurred at: " << std::ctime(&now_time);
        logFile << "Exception message: " << e.what() << std::endl;
        logFile << "----------------------------------------" << std::endl;

        logFile.close();
        std::cout << "Exception logged to " << logFilename << std::endl;
    }
    else {
        std::cerr << "Error opening log file." << std::endl;
    }
}