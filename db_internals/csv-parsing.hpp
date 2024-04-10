#ifndef CSV_PARSING_HPP
#define CSV_PARSING_HPP

#include <filesystem>
#include <vector>
#include <map>

#include "jjma.hpp"

std::map<std::string, jjma_dataTypes> parse_csv(const std::filesystem::path&);
std::vector<std::string> parse_csv_line(const std::string&);
jjma_dataTypes determineDataType(const std::string&); // Determines the data type of a column

// std::string convertEnumToString(jjma_dataTypes);


#endif /* CSV_PARSING_HPP */