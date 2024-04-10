#ifndef JJDB_HPP
#define JJDB_HPP

#include <filesystem>

#include "jjma.hpp"

typedef std::variant<int, double, std::string> jjdb_field;

std::filesystem::path build_jjdb_for_csv(const std::filesystem::path&);

std::vector<std::map<std::string, std::variant<int, double, std::string>>> 
csv_row_to_jjdb_row(
    const std::vector<std::string>, 
    const std::map<std::string, jjma_dataTypes>,
    const std::vector<std::string>
);

#endif /* JJDB_HPP */