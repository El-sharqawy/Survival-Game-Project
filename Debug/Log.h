//
// Created by osama on 09/05/24.
//

#ifndef LOG_H
#define LOG_H

#define DEBUG_STRING_MAX_LEN 1024

extern void TraceError(const char * c_szFormat, ...);
extern void TraceLog(const char * c_szFormat, ...);
extern void OpenLogFile(bool bUseLogFile);

#endif //LOG_H
