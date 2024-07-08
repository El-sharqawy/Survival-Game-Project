#include "Log.h"

#include <string>
#include <cstring>
#include <cstdarg>

void TraceError(const char * c_szFormat, ...)
{
    char szBuf[DEBUG_STRING_MAX_LEN + 2];

    strncpy(szBuf, "SYSERR: ", DEBUG_STRING_MAX_LEN);
    size_t len = strlen(szBuf);

    va_list args;
    va_start(args, c_szFormat);
    len = vsnprintf(szBuf + len, sizeof(szBuf) - (len + 1), c_szFormat, args) + len;
    va_end(args);

    szBuf[len] = '\n';
    szBuf[len + 1] = '\0';

    const time_t ct = time(nullptr);
    auto [tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst, tm_gmtoff, tm_zone] = *localtime(&ct);

    fprintf(stderr, "%02d%02d %02d:%02d:%02d :: %s",
                    tm_mon + 1,
                    tm_mday,
                    tm_hour,
                    tm_min,
                    tm_sec,
                    szBuf + 8);
    fflush(stderr);
}

void TraceLog(const char * c_szFormat, ...)
{
    char szBuf[DEBUG_STRING_MAX_LEN + 2];

    strncpy(szBuf, "SYSERR: ", DEBUG_STRING_MAX_LEN);
    size_t len = strlen(szBuf);

    va_list args;
    va_start(args, c_szFormat);
    len = vsnprintf(szBuf + len, sizeof(szBuf) - (len + 1), c_szFormat, args) + len;
    va_end(args);

    szBuf[len] = '\n';
    szBuf[len + 1] = '\0';

    const time_t ct = time(nullptr);
    auto [tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst, tm_gmtoff, tm_zone] = *localtime(&ct);

    fprintf(stdout, "%02d%02d %02d:%02d:%02d :: %s",
                    tm_mon + 1,
                    tm_mday,
                    tm_hour,
                    tm_min,
                    tm_sec,
                    szBuf + 8);
    fflush(stdout);
}

void OpenLogFile(const bool bUseLogFile)
{
    freopen("syserr.txt", "w", stderr);
    if (bUseLogFile)
    {
        freopen("log.txt", "w", stdout);
    }
}

void InitializeLog()
{
    freopen64("syserr.txt", "w", stderr);
#if defined(ENABLE_DEBUGGING)
    freopen64("log.txt", "w", stdout);
#endif
}
