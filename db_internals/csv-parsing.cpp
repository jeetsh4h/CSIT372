#include <fstream>
#include <iostream>

#include "jjma.hpp"
#include "parse-utils.hpp"
#include "jjdb.hpp"

#include "csv-parsing.hpp"


/* delimiter is assumed to be a comma
 * this also assumes that double quotes are 
 * to escape commas
 * Only double quotes can be escaped using a 
 * '\' character, and that too just one.
 * No other character can be escaped like that, even commas.
 */
std::vector<std::string> parse_csv_line(const std::string& line) {
    std::vector<std::string> parsed_line;
    bool inQuotes = false;
    char prev_char = '\0';
    
    std::string field;
    for (char c : line)
    {
        if (c == '"' && prev_char != '\\') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes){

            parsed_line.push_back(field);
            field = "";
        } else {
            field += c;
        }
        prev_char = c;
    }

    return parsed_line;
}

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
    std::ifstream csv_file(file_path);
    std::string line;
    
    getline(csv_file, line); // Get the first line (header row)
    std::vector<std::string> header = parse_csv_line(line);

    std::map<std::string, jjma_dataTypes> jjma_for_csv;
    for (std::string& column : header) {
        jjma_for_csv[column] = jjma_dataTypes::ID;
    }
    
    while (getline(csv_file, line)) {
        std::vector<std::string> row = parse_csv_line(line);
        for (size_t i = 0; i < (size_t)row.size(); i++) {
            // no need to check further for string
            if (jjma_for_csv[header[i]] == jjma_dataTypes::STRING) {
                continue;
            }
            jjma_for_csv[header[i]] = determineDataType(row[i]);
        }
    }

    return jjma_for_csv;
}

jjma_dataTypes determineDataType(const std::string& value) {
    std::istringstream iss(value);
    int intVal;
    double doubleVal;
    if (iss >> intVal) {
        if (iss.eof()) {
            return jjma_dataTypes::INT;      // Only an integer
        } else if (iss >> doubleVal && iss.eof()) {
            return jjma_dataTypes::DOUBLE;   // Integer followed by a double
        } else {
            return jjma_dataTypes::STRING;   // Integer followed by a string;
        }
    } else if (iss >> doubleVal && iss.eof()) {
        return jjma_dataTypes::DOUBLE;      // Only a double
    } else {
        return jjma_dataTypes::STRING;      // Only a string
    }
}

// std::string convertEnumToString(jjma_dataTypes dataType) {
//     switch (dataType) {
//         case jjma_dataTypes::ID:
//             return "auto_id";
//         case jjma_dataTypes::INT:
//             return "int";
//         case jjma_dataTypes::DOUBLE:
//             return "double";
//         case jjma_dataTypes::STRING:
//             return "string";
//         // case jjma_dataTypes::BOOL:
//         //     return "bool";
//     }
//     return "unknown";
// }
