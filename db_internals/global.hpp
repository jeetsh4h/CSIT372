#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <filesystem>

namespace global {
    extern std::filesystem::path *cwd;
    extern bool db_open;
}

#endif /* GLOBAL_HPP */