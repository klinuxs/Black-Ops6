#pragma once
#include <string>
struct Config
{
    std::string kmboxIP;
    int kmboxPort;
    int kmboxBaud;
    std::string kmboxUUID;
    int KmboxComPort;
    int screenWidth;
    int screenHeight;
    std::string kmboxType;
    std::string key;
};

bool ReadConfig(Config& config);
bool SaveConfig(const Config& config);
