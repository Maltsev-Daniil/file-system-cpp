#include "../include/io.h"
#include <iostream>

void ConsoleOutput::write(const std::string &text)
{
    std::cout << text;
}

void ConsoleOutput::writeln(const std::string &line) {
    std::cout << line << std::endl;
}