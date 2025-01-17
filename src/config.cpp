#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include "config.hpp"

Config sysfex_config;

Config *Config::the() {
    return &sysfex_config;
}

void Config::setValue(std::string key, std::string value) {
    if (config.find(key) != config.end()) {
        config[key] = value;
    }
}


std::string Config::getValue(std::string key) {
    if (config.find(key) != config.end()) {
        return config[key];
    }
    return "";
}

void Config::init(std::string dir) {
    config = {
            {"pregap",            "6"},
            {"gap",               "10"},
            {"ascii",             "1"},
            {"ascii_path",        "/opt/sysfex/ascii/tux.txt"},
            {"ascii_beside_text", "1"},
            {"color_block",       "⬤"},
            {"separator",         ":"},
            {"bold_ascii",        "1"},
            {"bold_text",         "1"}
    };

    if (!std::filesystem::exists(dir)) {
        return;
    }

    std::ifstream configFile;
    configFile.open(dir);
    if (!configFile.is_open()) {
        return;
    }

    while (configFile.good()) {
        std::string currentLine;
        getline(configFile, currentLine);
        currentLine.erase(std::remove_if(currentLine.begin(), currentLine.end(), isspace),
                          currentLine.end());  /* Remove unnecessary spaces */

        /*
          Lines starting with '#' will be considered as comments
          Ignore the comments and empty lines
        */
        if (currentLine[0] == '#' or currentLine.empty()) {
            continue;
        }

        int delimiter = currentLine.find("=");
        std::string key = currentLine.substr(0, delimiter), value = currentLine.substr(delimiter + 1);
        setValue(key, value);
    }
}
