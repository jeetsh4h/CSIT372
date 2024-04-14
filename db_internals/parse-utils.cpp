#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "parse-utils.hpp"


std::string rstrip(const std::string& str) {
    std::string rstrip_str = str;
    
    size_t trailing_whitespace_start_pos = rstrip_str.find_last_not_of(" \n\r\t\f\v");
    
    if (trailing_whitespace_start_pos != std::string::npos) {
        rstrip_str.erase(trailing_whitespace_start_pos + 1, std::string::npos);
    } else {
        rstrip_str = "";
    }
    
    return rstrip_str;
}

std::string lstrip(const std::string& str) {
    std::string lstrip_str = str;
    size_t leading_whitespace_end_pos = lstrip_str.find_first_not_of(" \n\r\t\f\v");

    if (leading_whitespace_end_pos != std::string::npos) {
        lstrip_str.erase(0, leading_whitespace_end_pos);
    } else {
        lstrip_str = "";
    }

    return lstrip_str;
}

std::string strip(const std::string& str) {
    return rstrip(lstrip(str));
}

std::vector<std::string> split_string(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        std::string stripped_token = strip(token);
        tokens.push_back(stripped_token);
    }

    return tokens;
}

std::string to_lower(const std::string& str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower_str;
}