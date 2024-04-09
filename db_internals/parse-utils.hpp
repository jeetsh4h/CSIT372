#ifndef PARSE_UTILS_HPP
#define PARSE_UTILS_HPP

#include <string>
#include <vector>

std::string rstrip(const std::string&);
std::string lstrip(const std::string&);
std::string strip(const std::string&);

std::vector<std::string> split_string(const std::string&, char);


#endif /* PARSE_UTILS_HPP */