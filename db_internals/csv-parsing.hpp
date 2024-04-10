#ifndef CSV_PARSING_HPP
#define CSV_PARSING_HPP

#include <filesystem>
#include <map>

#include "jjma.hpp"

std::map<std::string, jjma_dataTypes> parse_csv(const std::filesystem::path&);

std::string determineDataType(const std::string&); // Determines the data type of a column
jjma_dataTypes convertToEnum(const std::string&);


#endif /* CSV_PARSING_HPP */