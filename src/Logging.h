#pragma once

#include <iostream>

#ifdef LOG_LEVEL_3
#define LOG_LEVEL_2
#endif

#ifdef LOG_LEVEL_2
#define LOG_LEVEL_1
#endif


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