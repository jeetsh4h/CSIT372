#include <filesystem>
#include <map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "jjma.hpp"
#include "build-jjdb.hpp"

#include "csv-parsing.hpp"

/*
 * Assuming that the CSV file **has headers**
 * All blank values will be treated as NULL values
 * 
 * The db_id needs to be the first row and has to start
 * with 0... and then increment by one
 * 
 * If the first column does not match that pattern then
 * we create it within the struct by default.
 */
std::map<std::string, jjma_dataTypes> parse_csv(const std::filesystem::path& file_path) {
    std::ifstream file(file_path);
    std::string line;
    std::vector<std::vector<std::string>> data;
    std::vector<std::string> headers;
    std::map<std::string, jjma_dataTypes> dataTypeMap; // Map to store column headers and their data types
    
    std::getline(file, line);
    std::istringstream ss(line);
    std::string cell;
    while (std::getline(ss, cell, ',')) {
        headers.push_back(cell);
    }

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::vector<std::string> row;
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    for (int i = 0; i < (int)headers.size(); i++) { // Traverse column-wise
        std::vector<std::string> column;
        for (int j = 0; j < (int)data.size(); j++) {
            column.push_back(data[j][i]);
        }
        std::string dataType = determineDataType(column[0]);
        for (int j = 1; j < (int)column.size(); j++) {
            if (determineDataType(column[j]) != dataType) {
                dataType = "string";
                break;
            }
        }
        dataTypeMap[headers[i]] = convertToEnum(dataType); // Store header and uska data type in the map
    }

    // Output the data types stored in the map
    for (const auto& pair : dataTypeMap) {
        std::cout << pair.first << " is of type " << pair.second << std::endl;
    }

    return dataTypeMap;
}

std::string determineDataType(const std::string& value) {
    std::istringstream iss(value);
    int intVal;
    double doubleVal;
    if (iss >> intVal) {
        if (iss.eof()) {
            return "int"; // Only an integer
        } else if (iss >> doubleVal && iss.eof()) {
            return "double"; // Integer followed by a double
        } else {
            return "string";
        }
    } else if (iss >> doubleVal && iss.eof()) {
        return "double"; // Only a double
    } else {
        std::unordered_set<std::string> booleanValues{"true", "false", "T", "F", "t", "f"}; // Possible boolean values
        if (booleanValues.find(value) != booleanValues.end())
            return "boolean"; // If value is one of the boolean values
        else
            return "string";
    }
}

jjma_dataTypes convertToEnum(const std::string& dataType) {
    if (dataType == "int") {
        return jjma_dataTypes::INT;
    } else if (dataType == "double") {
        return jjma_dataTypes::DOUBLE;
    } else if (dataType == "string") {
        return jjma_dataTypes::STRING;
    } else if (dataType == "boolean") {
        return jjma_dataTypes::BOOL;
    } else {
        return jjma_dataTypes::STRING;
    }
}