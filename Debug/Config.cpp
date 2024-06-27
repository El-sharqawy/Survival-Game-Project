//
// Created by osama on 09/05/24.
//
#include <fstream>
#include <sstream>
#include <utility>
#include "Log.h"
#include "Config.h"
#include "../game.h"

std::map<std::string, int32_t> LoadConfigFile(const std::string& fileName)
{
    std::map<std::string, int32_t> configData;
    static std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (std::getline(myFile, line))
        {
            std::istringstream iss(line);
            static std::string key;
            static int32_t value;
            if (iss >> key >> value)
            {
                configData[key] = value;
            }
        }
        myFile.close();
    }
    else
    {
        static std::ofstream outFile(fileName);
        if (outFile.is_open())
        {
            outFile << "width\t" << WINDOW_WIDTH << "\n";
            outFile << "height\t" << WINDOW_HEIGHT << "\n";
            outFile << "full_screen\t" << FULL_SCREEN << "\n";
            outFile.close();
        }
        else
        {
            TraceError("Failed to create Config File");
        }
        configData.insert({"width", WINDOW_WIDTH});
        configData.insert({"height", WINDOW_HEIGHT});
        configData.insert({"full_screen", FULL_SCREEN});
    }
    return (configData);
}