#ifndef JJDB_HPP
#define JJDB_HPP

#include <filesystem>

#include "jjma.hpp"

typedef std::variant<int, double, std::string> jjdb_field;
typedef std::map<std::string, jjdb_field> jjdb_row;

std::filesystem::path build_jjdb_for_csv(const std::filesystem::path&);

std::map<std::string, jjdb_field> 
csv_row_to_jjdb_row(const std::vector<std::string>,const std::vector<std::string>, const int);

std::string serialise_jjdb_row(const jjdb_row&);
jjdb_row deserialise_jjdb_row(const std::string&);

void write_to_jjdb(std::ostream&, std::string&);

void serialise_and_write_to_jjdb(std::ostream&, const std::map<std::string, jjdb_field>&);

jjdb_row read_row(int);
void append_row(jjdb_row&);
void update_jjdb_row(int, jjdb_row&);
void delete_jjdb_row(int);

int find_num_records();

#endif /* JJDB_HPP */