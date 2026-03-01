#pragma once
#include <iostream>

#ifdef DEBUG
    #define DEBUG_LOG(x) do { std::cout << "DEBUG: " << x << std::endl; } while (0)
#else
    #define DEBUG_LOG(x) do {} while (0)
#endif