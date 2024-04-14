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
void exec_select(const std::string& args, const std::vector<std::string> condition, int take) {
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

void exec_insert(const std::string& args) {
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

    std::vector<std::string> insert_headers;
    std::vector<std::string> insert_pairs = split_string(args, ',');
    jjdb_row insert_row;

    for (const auto& pair : insert_pairs) {
        std::vector<std::string> pair_tokens = split_string(pair, '=');
        if (pair_tokens.size() != 2) {
            std::cout << "Invalid insert syntax here: " << pair << std::endl;
            return;
        }

        std::string header = pair_tokens[0];
        std::string value = pair_tokens[1];

        if (header == "auto_id") {
            std::cout << "Cannot set auto_id. It is auto-generated." << std::endl;
            return;
        }
        insert_headers.push_back(header);
        
        if (std::find(schema_headers.begin(), schema_headers.end(), header) == schema_headers.end()) {
            std::cout << "Invalid header: " << header << std::endl;
            return;
        }

        switch (global::db_schema_map[header]) {
            case jjma_dataTypes::ID:
                insert_row[header] = std::stoi(value);
                break;
            case jjma_dataTypes::INT:
                insert_row[header] = std::stoi(value);
                break;
            case jjma_dataTypes::DOUBLE:
                insert_row[header] = std::stod(value);
                break;
            case jjma_dataTypes::STRING:
                insert_row[header] = value;
                break;
            case jjma_dataTypes::UNKNOWN:
                std::cout << "Unknown data type found in schema. Please correct the schema." << std::endl;
                return;
        }
    }

    for (const auto& header : schema_headers) {
        if (header == "auto_id") {
            continue;
        }
        if (std::find(insert_headers.begin(), insert_headers.end(), header) == insert_headers.end()) {
            switch (global::db_schema_map[header]) {
                case jjma_dataTypes::ID:
                    /* Should never enter here */
                    std::cout << "ID should not be present in the insert statement." << std::endl;
                    return;
                case jjma_dataTypes::INT:
                    insert_row[header] = 0;
                    break;
                case jjma_dataTypes::DOUBLE:
                    insert_row[header] = 0.0;
                    break;
                case jjma_dataTypes::STRING:
                    insert_row[header] = "NA";
                    break;
                case jjma_dataTypes::UNKNOWN:
                    std::cout << "Unknown data type found in schema. Please correct the schema." << std::endl;
                    return;
            }
        }
    }

    append_row(insert_row);
    return;
}

void exec_update(const std::string& args, const std::vector<std::string> condition, int id) {
    if (!global::db_open) {
        std::cout << "No database is open. Please open a database before executing SQL commands." << std::endl;
        return;
    }

    if (global::db_schema_map.empty()) {
        std::cout << "No schema found OR schema corrupted." << std::endl;
        return;
    }

    if (id < 0 && condition.empty()) {
        std::cout << "No condition or id provided for update." << std::endl;
        return;
    }

    if (id >= 0 && !condition.empty()) {
        std::cout << "Both condition and id provided for update. Please provide only one." << std::endl;
        return;
    }

    if (!condition.empty()) {
        std::cout << "Condition not yet implemented." << std::endl;
        return;
    }

    if (id >= find_num_records()) {
        std::cout << "ID provided is greater than the largest auto_id value in the database." << std::endl;
        return;
    }   

    std::vector<std::string> schema_headers;
    for (const auto& [header, _] : global::db_schema_map) {
        schema_headers.push_back(header);
    }

    std::vector<std::string> update_headers;
    std::vector<std::string> update_pairs = split_string(args, ',');
    jjdb_row update_row;

    for (const auto& pair : update_pairs) {
        std::vector<std::string> pair_tokens = split_string(pair, '=');
        if (pair_tokens.size() != 2) {
            std::cout << "Invalid insert syntax here: " << pair << std::endl;
            return;
        }

        std::string header = pair_tokens[0];
        std::string value = pair_tokens[1];

        if (header == "auto_id") {
            std::cout << "Cannot set auto_id. It is auto-generated." << std::endl;
            return;
        }
        update_headers.push_back(header);
        
        if (std::find(schema_headers.begin(), schema_headers.end(), header) == schema_headers.end()) {
            std::cout << "Invalid header: " << header << std::endl;
            return;
        }

        switch (global::db_schema_map[header]) {
            case jjma_dataTypes::ID:
                update_row[header] = std::stoi(value);
                break;
            case jjma_dataTypes::INT:
                update_row[header] = std::stoi(value);
                break;
            case jjma_dataTypes::DOUBLE:
                update_row[header] = std::stod(value);
                break;
            case jjma_dataTypes::STRING:
                update_row[header] = value;
                break;
            case jjma_dataTypes::UNKNOWN:
                std::cout << "Unknown data type found in schema. Please correct the schema." << std::endl;
                return;
        }
    }

    update_jjdb_row(id, update_row);
    return;
}