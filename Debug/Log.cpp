#include "Log.h"

#include <cstdarg>
#include <ctime>
#include "../game.h"

void TraceLog(const std::string& szStr, ...)
{
    std::string sTempBuf;

    va_list args;
    va_start(args, szStr);
    const size_t len = vsnprintf(nullptr, 0, szStr.c_str(), args);
    sTempBuf.resize(len + 1);
    vsnprintf(&sTempBuf[0], len + 1, szStr.c_str(), args);
    va_end(args);
    sTempBuf.resize(len);
    sTempBuf.append("\n");

    time_t curTime = time(nullptr);
    struct tm curTimeLocal = *localtime(&curTime);
    fprintf(stdout, "LOG: %02d%02d %02d:%02d :: %s",
        curTimeLocal.tm_mon + 1,
        curTimeLocal.tm_mday,
        curTimeLocal.tm_hour,
        curTimeLocal.tm_min,
        sTempBuf.c_str());
    fflush(stdout);
}

void TraceError(const std::string& szStr, ...)
{
    std::string sTempBuf;

    va_list args;
    va_start(args, szStr);
    const size_t len = vsnprintf(nullptr, 0, szStr.c_str(), args);
    sTempBuf.resize(len + 1);
    vsnprintf(&sTempBuf[0], len + 1, szStr.c_str(), args);
    va_end(args);
    sTempBuf.resize(len);
    sTempBuf.append("\n");

    time_t curTime = time(nullptr);
    struct tm curTimeLocal = *localtime(&curTime);
    fprintf(stderr, "SYSERR: %02d%02d %02d:%02d :: %s",
        curTimeLocal.tm_mon + 1,
        curTimeLocal.tm_mday,
        curTimeLocal.tm_hour,
        curTimeLocal.tm_min,
        sTempBuf.c_str());
    fflush(stderr);
}

void InitializeLog()
{
    freopen64("syserr.txt", "w", stderr);
#if defined(ENABLE_DEBUGGING)
    freopen64("log.txt", "w", stdout);
#endif
}
