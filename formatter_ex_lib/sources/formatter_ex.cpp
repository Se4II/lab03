#include "formatter_ex.h"
#include "formatter.h"
#include <iostream>
void formatter_ex::print_ex(const std::string& text) {
    std::cout << "[EX] " << text << std::endl;
    formatter::print(text);
}
