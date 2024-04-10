#ifndef JJMA_HPP
#define JJMA_HPP

#include <map>
#include <string>
#include <filesystem>

/*
 * if an id is not present within the 
 * source (essentially the csv file),
 * then we create it.
 * 
 * The id must be an unsigned int
 */
enum class jjma_dataTypes {
    ID,     // unsigned int /* auto-incrementing ID column always generated by us */
    INT,    // long
    DOUBLE, // double
    STRING, // std::string
    BOOL    // bool
};

std::filesystem::path build_jjma(const std::map<std::string, jjma_dataTypes>);


#endif /* JJMA_HPP */