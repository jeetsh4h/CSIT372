#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "global.hpp"
#include "jjdb.hpp"
#include "jjma.hpp"
#include "parse-utils.hpp"

#include "sql-query.hpp"


// TODO: implement where condition
void exec_select(const std::string& args, const std::vector<std::string> condition = std::vector<std::string>(), int take = -1) {
    if (!global::db_open) {
        std::cout << "No database is open. Please open a database before executing SQL commands." << std::endl;
        return;
    }

    if (global::db_schema_map.empty()) {
        std::cout << "No schema found OR schema corrupted." << std::endl;
        return;
    }

    std::vector<std::string> schema_headers;
    for (const auto& [header, _] : global::db_schema_map) {
        schema_headers.push_back(header);
    }

    std::vector<std::string> select_headers;
    if (args == "*") {
        select_headers = schema_headers;
    } else {
        select_headers = split_string(args, ',');
    }

    // Ensure that every element in select_headers exists in schema_headers
    for (const auto& header : select_headers) {
        if (std::find(schema_headers.begin(), schema_headers.end(), header) == schema_headers.end()) {
            std::cout << "Invalid header: " << header << std::endl;
            return;
        }
    }

    for (const auto& header : select_headers) {
        std::cout << header << " | ";
    }
    std::cout << std::endl;

    int line_num = 0;
    while (take == -1 || line_num < take) {
        jjdb_row db_row;
        try {
            db_row = read_row(line_num);
        } catch (std::out_of_range& e) {
            break;
        }

        for (const auto& header : select_headers) {
            switch (global::db_schema_map[header]) {
                case jjma_dataTypes::ID:
                    std::cout << std::get<int>(db_row[header]) << " | ";
                    break;
                case jjma_dataTypes::INT:
                    std::cout << std::get<int>(db_row[header]) << " | ";
                    break;
                case jjma_dataTypes::DOUBLE:
                    std::cout << std::get<double>(db_row[header]) << " | ";
                    break;
                case jjma_dataTypes::STRING:
                    std::cout << std::get<std::string>(db_row[header]) << " | ";
                    break;
                case jjma_dataTypes::UNKNOWN:
                    std::cout << "Unknown data type found in schema. Please correct the schema." << std::endl;
                    return;
            }
        }
        std::cout << std::endl;

        line_num++;
    }
}