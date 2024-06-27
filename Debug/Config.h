//
// Created by osama on 09/05/24.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <map>

extern std::map<std::string, int32_t> LoadConfigFile(const std::string& fileName);
#endif //CONFIG_H
