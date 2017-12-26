#include "latex.h"
#include <sstream>

std::string ltx::BeginStr(std::string s) {
    std::stringstream ss;
    ss << "\\begin{" << s << "}";
    return ss.str();
}

std::string ltx::EndStr(std::string s) {
    std::stringstream ss;
    ss << "\\end{" << s << "}";
    return ss.str();
}

void ltx::Begin(std::string s) {
    std::cout << "\\begin{" << s << "}";
}

void ltx::End(std::string s) {
    std::cout << "\\end{" << s << "}";
}

