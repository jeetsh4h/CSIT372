#ifndef CSV_PARSING_HPP
#define CSV_PARSING_HPP

#include <filesystem>
#include <map>

std::map<std::string, jjma_dataTypes> parse_csv(const std::filesystem::path&);


#endif /* CSV_PARSING_HPP */