#include <Memory.h>
#include "Menu.h"
#include "Config.h"

#include <iostream>
#include <thread>
#include <chrono>
#include "curl/include/curl/curl.h"
/*




                    Made by nikgeneburn/lolz5465az Credits to @VFVV for his awesome black-ops-6 V1 base
                                            which core code was used from.










*/
// if you want a pasted p2c
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string curlRequest(const std::string& url, std::string postfields) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    return response;
}

bool fileExists(const std::string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}



int main() {
        int temp;
        const std::string configFile = "config.cfg";
        Config config;
        if (fileExists(configFile)) {
            std::cout << "Config file found. Loading configuration..." << std::endl;
            ReadConfig(config);
        }
        else {
            std::cout << "No config file found. Creating a new one..." << std::endl;
            std::cout << std::endl << "[1] Arduino/b+Pro\n[2] Net\nEnter: ";
            std::cin >> temp;
            if (temp == 1)
            {
                config.kmboxType = "Arduino";
            }
            else { config.kmboxType = "Net"; }
            if (config.kmboxType == "Net")
            {
                std::cout << std::endl << "Enter KMBOX Ip: ";
                std::cin >> config.kmboxIP;
                std::cout << std::endl << "Enter KMBOX Port: ";
                std::cin >> config.kmboxPort;
                std::cout << std::endl << "Enter KMBOX UUID: ";
                std::cin >> config.kmboxUUID;
                std::cout << std::endl;
            }
            else {
                std::cout << std::endl << "Enter Ardunio Port: ";
                std::cin >> config.KmboxComPort;
                std::cout << std::endl << "Enter Ardunio BAUD (Default 115200): ";
                std::cin >> config.kmboxBaud;
            }

            SaveConfig(config);
        }
        mem.Init((LPSTR)"cod.exe");
        Addresses::base = mem.GetBaseDaddy("cod.exe"); // cod.exe = bo6 or above - mw24.exe or whatever for below
        Game game;
        //mem.GetKeyboard()->InitKeyboard(); // not writing keyboard library
        try {
            Render(config, mem, game);
        }
        catch (const std::exception& e) {
            logException(e);
        }
        return 0;
}
