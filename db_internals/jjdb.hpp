#ifndef JJDB_HPP
#define JJDB_HPP

#include <filesystem>

#include "jjma.hpp"

typedef std::variant<int, double, std::string> jjdb_field;
typedef std::map<std::string, jjdb_field> jjdb_row;

std::filesystem::path build_jjdb_for_csv(const std::filesystem::path&);

std::map<std::string, jjdb_field> 
csv_row_to_jjdb_row(const std::vector<std::string>,const std::vector<std::string>, const int);

void write_serially_to_jjdb(std::ofstream&, const std::map<std::string, jjdb_field>&);
std::map<std::string, jjdb_field> deserialise_jjdb_row(const std::string&);

std::map<std::string, jjdb_field> read_row(int);

#endif /* JJDB_HPP */