//
// Created by osama on 09/05/24.
//

#ifndef LOG_H
#define LOG_H

#include <string>

extern void TraceLog(const std::string& szStr, ...);
extern void TraceError(const std::string& szStr, ...);

extern void InitializeLog();
#endif //LOG_H
