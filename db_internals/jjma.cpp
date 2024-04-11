#include <iostream>
#include <fstream>

#include "global.hpp"
#include "parse-utils.hpp"

#include "jjma.hpp"


std::filesystem::path build_jjma_for_csv(const std::map<std::string, jjma_dataTypes> jjma_data) {
    if (!global::db_open) {
        std::cout << "No JJDB is open. Please open a JJDB before creating a JJMA file." << std::endl;
        return *global::cwd;
    }

    std::string db_name = global::cwd->filename().string();
    std::filesystem::path jjma_path = *global::cwd / (db_name + ".jjma");

    std::ofstream jjma_file(jjma_path);

    // adding auto_id
    jjma_file << "auto_id : id" << std::endl;

    /* const std::string& key; const jjma_dataTypes& value */
    for (const auto& [key, value] : jjma_data) {
        switch (value) {
            // should never enter this case
            case jjma_dataTypes::ID:
                /* DEBUG */
                std::cout << "ID column should be auto-inserted\n";
                /* DEBUG */
                break;

            case jjma_dataTypes::INT:
                jjma_file << key << " : int" << std::endl;
                break;

            case jjma_dataTypes::DOUBLE:
                jjma_file << key << " : double" << std::endl;
                break;

            case jjma_dataTypes::STRING:
                jjma_file << key << " : string" << std::endl;
                break;

            // case jjma_dataTypes::BOOL:
            //     jjma_file << key << " : bool" << std::endl;
            //     break;

            case jjma_dataTypes::UNKNOWN:
                std::cout << "Unknown data type found in JJMA file." << std::endl;
                return jjma_path;
        }
    }

    jjma_file.close();
    return jjma_path;
}

std::map<std::string, jjma_dataTypes> parse_schema(const std::filesystem::path& jjma_path) {
    std::ifstream jjma_file(jjma_path);
    std::string line;
    std::map<std::string, jjma_dataTypes> jjma_data;

    while (getline(jjma_file, line)) {
        std::vector<std::string> header_dtypes = split_string(line, ':');
        if (!line.size()) {
            break;
        }

        std::string header = strip(header_dtypes[0]);
        std::string dtype = strip(header_dtypes[1]);

        jjma_dataTypes header_jjma = stringToEnum(dtype);
        if (header_jjma == jjma_dataTypes::UNKNOWN) {
            std::cout << "Unknown data type found in JJMA file." << std::endl;
            return {};
        } else {
            jjma_data[header] = header_jjma;
        }
    }

    return jjma_data;
}

jjma_dataTypes stringToEnum(const std::string& dType_str) {
    if (dType_str == "id") {
        return jjma_dataTypes::ID;
    }

    if (dType_str == "int") {
        return jjma_dataTypes::INT;
    }

    if (dType_str == "double") {
        return jjma_dataTypes::DOUBLE;
    }

    if (dType_str == "string") {
        return jjma_dataTypes::STRING;
    }

    // if (dType_str == "bool") {
    //     return jjma_dataTypes::BOOL;
    // }

    return jjma_dataTypes::UNKNOWN;
}