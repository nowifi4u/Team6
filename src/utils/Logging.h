#pragma once

#include <iostream>
#include <iomanip>

#ifdef LOG_LEVEL_3
#define LOG_LEVEL_2
#endif

#ifdef LOG_LEVEL_2
#define LOG_LEVEL_1
#endif


#define LOG(args) std::cout << args << std::endl


#ifdef LOG_LEVEL_1
#define LOG_1(args) std::cout << "[log_1]: " << args << std::endl

#else
#define LOG_1(args) 1

#endif

#ifdef LOG_LEVEL_2
#define LOG_2(args) std::cout << "[log_2]: " << args << std::endl

#else
#define LOG_2(args) 1

#endif

#ifdef LOG_LEVEL_3
#define LOG_3(args) std::cout << "[log_3]: " << args << std::endl

#else
#define LOG_3(args) 1

#endif

inline std::string toHex(const std::string& s, const std::string& sep = " ", bool upper_case = true)
{
    std::ostringstream ret;

    for (char ch : s)
        ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << (int)(unsigned char)ch << sep;

    return ret.str();
}