#include <fstream>
#include <iostream>

#include "global.hpp"
#include "csv-parsing.hpp"
#include "jjma.hpp"

#include "jjdb.hpp"

std::filesystem::path build_jjdb_for_csv(const std::filesystem::path& csv_path) {
    if (!global::db_open) {
        std::cout << "No JJDB is open. Please open one to build the jjdb file from a csv." << std::endl;
        return *global::cwd;
    }
    
    std::string db_name = global::cwd->filename().string();
    std::filesystem::path jjma_path = *global::cwd / (db_name + ".jjma");
    
    if (!std::filesystem::exists(jjma_path)) {
        std::cout << "No JJMA file found. Please create a JJMA file before creating a JJDB file." << std::endl;
        return *global::cwd;
    }
    
    std::filesystem::path jjdb_path = *global::cwd / (db_name + ".jjdb");
    std::map<std::string, jjma_dataTypes> schema_map = parse_schema(jjma_path);
    // Dealing with unknown or corrupted schema
    if (schema_map.empty()) {
        return *global::cwd;
    }

    std::ifstream csv_file(csv_path);
    std::ofstream jjdb_file(jjdb_path);

    std::string line;
    getline(csv_file, line);
    std::vector<std::string> headers = parse_csv_line(line);
    
    int auto_id = 0;
    while(getline(csv_file, line)) {
        std::vector<std::string> row = parse_csv_line(line);
        
        /* std::map<std::string, jjdb_field> */
        auto jjdb_row = csv_row_to_jjdb_row(row, schema_map, headers, auto_id);
        if (jjdb_row.empty()) {
            return *global::cwd;
        }
        
        write_serially_to_jjdb(jjdb_file, jjdb_row);

        /* DEBUG */
        deserialise_jjdb(jjdb_path, jjma_path);

        auto_id++;
    }

    return jjdb_path;
}

// TODO:
std::map<std::string, jjdb_field>
csv_row_to_jjdb_row(
    const std::vector<std::string> row, 
    const std::map<std::string, jjma_dataTypes>& schema_map,
    const std::vector<std::string> headers, int auto_id) {
    
    std::map<std::string, jjdb_field> jjdb_row;
    
    for (size_t i = 0; i < row.size(); i++) {
        std::string col_name = headers.at(i);
        jjma_dataTypes col_type = schema_map.at(col_name);

        switch (col_type) {
            case jjma_dataTypes::ID:
                jjdb_row[col_name] = auto_id;
                break;

            case jjma_dataTypes::INT:
                jjdb_row[col_name] = std::stoi(row.at(i));
                break;

            case jjma_dataTypes::DOUBLE:
                jjdb_row[col_name] = std::stod(row.at(i));
                break;

            case jjma_dataTypes::STRING:
                jjdb_row[col_name] = row.at(i);
                break;

            case jjma_dataTypes::UNKNOWN:
                std::cout << "Unknown data type found in schema. Please correct the schema." << std::endl;
                return {};
        }

    }

    return jjdb_row;
}

void write_serially_to_jjdb(std::ofstream& jjdb_file, const std::map<std::string, jjdb_field>& jjdb_row) {
    // size_t size = jjdb_row.size();
    // jjdb_file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (const auto& [header, field] : jjdb_row) {
        size_t header_size = sizeof(header.c_str());
        jjdb_file.write(reinterpret_cast<const char*>(&header_size), sizeof(header_size));
        jjdb_file.write(header.c_str(), header_size);

        size_t field_size = sizeof(field);
        jjdb_file.write(reinterpret_cast<const char*>(&field_size), sizeof(field_size));
        jjdb_file.write(reinterpret_cast<const char*>(&field), field_size);
    }
}

void deserialise_jjdb(const std::filesystem::path& jjdb_path, const std::filesystem::path& jjma_path) {
    std::ifstream jjdb_file(jjdb_path);
    std::map<std::string, jjma_dataTypes> db_map = parse_schema(jjma_path);

    std::vector<std::map<std::string, jjdb_field>> jjdb_data;

    while (!jjdb_file.eof()) {
        std::map<std::string, jjdb_field> jjdb_row;
        
        for (const auto& [_, dtype] : db_map) {
            size_t header_size;
            jjdb_file.read(reinterpret_cast<char*>(&header_size), sizeof(header_size));
            char* header_c = new char[header_size];
            jjdb_file.read(header_c, header_size);
            std::string header = std::string(header_c);
            
            size_t field_size;
            jjdb_file.read(reinterpret_cast<char*>(&field_size), sizeof(field_size));
            char* field_c = new char[field_size];
            jjdb_file.read(field_c, field_size);
            switch (dtype) {
                case jjma_dataTypes::ID:
                    jjdb_row[header] = std::stoi(field_c);
                    break;

                case jjma_dataTypes::INT:
                    jjdb_row[header] = std::stoi(field_c);
                    break;

                case jjma_dataTypes::DOUBLE:
                    jjdb_row[header] = std::stod(field_c);
                    break;

                case jjma_dataTypes::STRING:
                    jjdb_row[header] = std::string(field_c);
                    break;

                case jjma_dataTypes::UNKNOWN:
                    std::cout << "Unknown data type found in schema. Please correct the schema." << std::endl;
                    return;
            }
        }
        jjdb_data.push_back(jjdb_row);
    }

    for (const auto& row : jjdb_data) {
        for (const auto& [header, field] : row) {
            std::cout << header << ": "; 
            
            switch (db_map[header]) {
                case jjma_dataTypes::ID:
                    std::cout << std::get<int>(field) << std::endl;
                    break;

                case jjma_dataTypes::INT:
                    std::cout << std::get<int>(field) << std::endl;
                    break;
                
                case jjma_dataTypes::DOUBLE:
                    std::cout << std::get<double>(field) << std::endl;
                    break;
                
                case jjma_dataTypes::STRING:
                    std::cout << std::get<std::string>(field) << std::endl;
                    break;
                
                case jjma_dataTypes::UNKNOWN:
                    std::cout << "Unknown data type found in schema. Please correct the schema." << std::endl;
                    return;
            }
        }
        std::cout << std::endl;
    }
}
