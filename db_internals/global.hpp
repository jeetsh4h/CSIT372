#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <filesystem>
#include <map>

#include "jjma.hpp"

namespace global {
    extern std::filesystem::path *cwd;
    extern bool db_open;
    extern std::map<std::string, jjma_dataTypes> db_schema_map;
}

#endif /* GLOBAL_HPP */