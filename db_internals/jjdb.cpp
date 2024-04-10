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
    
    while(getline(csv_file, line)) {
        std::vector<std::string> row = parse_csv_line(line);
        
        /* std::vector<std::map<std::string, jjdb_field>> */
        auto jjdb_row = csv_row_to_jjdb_row(row, schema_map);
        
        
    }

    return jjdb_path;
}

// TODO:
std::vector<std::map<std::string, jjdb_field>>
csv_row_to_jjdb_row(
    const std::vector<std::string> row, 
    const std::vector<std::string, jjma_dataTypes> schema_map,
    const std::vector<std::string> headers) {
    
    std::vector<std::map<std::string, jjdb_field>> jjdb_row;
    jjdb_row.push_back({
            {"auto_id", 0}
        });
    
    for (size_t i = 0; i < row.size(); i++) {
        std::string col_name = headers.at(i);
        jjma_dataTypes col_type = schema_map[col_name];

        switch (col_type) {
            case jjma_dataTypes::ID:
                jjdb_row.push_back({
                    {col_name, i}
                });
                break;

            case jjma_dataTypes::INT:
                jjdb_row.push_back({
                    {col_name, std::stoi(row.at(i))}
                });
                break;

            case jjma_dataTypes::DOUBLE:
                jjdb_row.push_back({
                    {col_name, std::stod(row.at(i))}
                });
                break;

            case jjma_dataTypes::STRING:
                jjdb_row.push_back({
                    {col_name, row.at(i)}
                });
                break;
        }

    }

    return jjdb_row;
}