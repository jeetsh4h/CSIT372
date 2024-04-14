#include <fstream>
#include <iostream>

#include "global.hpp"
#include "csv-parsing.hpp"
#include "jjma.hpp"
#include "parse-utils.hpp"

#include "jjdb.hpp"

std::filesystem::path build_jjdb_for_csv(const std::filesystem::path& csv_path) {
    if (!global::db_open) {
        std::cout << "No JJDB is open. Please open one to build the jjdb file from a csv." << std::endl;
        return *global::cwd;
    }
    
    std::string db_name = global::cwd->filename().string();

    if (global::db_schema_map.empty()) {
        std::cout << "No schema found. Please create a jjma file before creating a jjdb file." << std::endl;
        return *global::cwd;
    }

    std::filesystem::path jjdb_path = *global::cwd / (db_name + ".jjdb");
    
    std::ifstream csv_file(csv_path);
    std::ofstream jjdb_file(jjdb_path);

    std::string line;
    getline(csv_file, line);
    std::vector<std::string> headers = parse_csv_line(line);
    
    /*
     * assumes that the auto_id is the last column
     * it does break if that is not the case
     * CANNOT have a header being auto_id
     */  
    headers.push_back("auto_id");
    
    int auto_id = 0;
    while(getline(csv_file, line)) {
        std::vector<std::string> row = parse_csv_line(line);
        
        jjdb_row db_row = csv_row_to_jjdb_row(row, headers, auto_id);
        if (db_row.empty()) {
            return *global::cwd;
        }        
        serialise_and_write_to_jjdb(jjdb_file, db_row);

        auto_id++;
    }

    csv_file.close();
    jjdb_file.close();
    return jjdb_path;
}

jjdb_row csv_row_to_jjdb_row(const std::vector<std::string> row, const std::vector<std::string> headers, int auto_id) {
    jjdb_row db_row;

    /* headers can only be one length bigger than the row */
    for (size_t i = 0; i < headers.size(); i++) {
        std::string col_name = headers.at(i);
        jjma_dataTypes col_type = global::db_schema_map.at(col_name);

        switch (col_type) {
            case jjma_dataTypes::ID:
                // std::cout << "id being added to the jjdb row";
                db_row[col_name] = auto_id;
                break;

            case jjma_dataTypes::INT:
                db_row[col_name] = std::stoi(row.at(i));
                break;

            case jjma_dataTypes::DOUBLE:
                db_row[col_name] = std::stod(row.at(i));
                break;

            case jjma_dataTypes::STRING:
                db_row[col_name] = row.at(i);
                break;

            case jjma_dataTypes::UNKNOWN:
                std::cout << "Unknown data type found in schema. Please correct the schema." << std::endl;
                return {};
        }

    }

    return db_row;
}

/* 
 * closing of the jjdb_file is handled outside of this function
 * this function only writes one row to the jjdb file at a time
 */
void serialise_and_write_to_jjdb(std::ostream& jjdb_file, const jjdb_row& db_row) {
    jjdb_file << serialise_jjdb_row(db_row) << std::endl;
}

jjdb_row read_row(int row_num) {
    if (!global::db_open) {
        std::cout << "No JJDB is open. Open one to read from it." << std::endl;
        return {};
    }
    std::string db_name = global::cwd->filename().string();
    std::filesystem::path jjdb_path = *global::cwd / (db_name + ".jjdb");

    std::ifstream jjdb_file(jjdb_path);
    std::string line;
    for (int i = 0; i <= row_num; ++i) {
        if (!std::getline(jjdb_file, line)) {
            throw std::out_of_range("Line number out of range");
        }
    }

    jjdb_file.close();

    return deserialise_jjdb_row(lstrip(line));
}

jjdb_row deserialise_jjdb_row(const std::string& jjdb_line) {
    if (jjdb_line == "") {
        return {};
    }
    jjdb_row db_row;

    for (const std::string& field : split_string(lstrip(jjdb_line), ';')) {
        std::vector<std::string> field_tokens = split_string(field, ':');
        std::string header = field_tokens.at(0);
        std::string value = field_tokens.at(1);

        jjma_dataTypes col_type = global::db_schema_map.at(header);
        switch (col_type) {
            case jjma_dataTypes::ID:
                db_row[header] = std::stoi(value);
                break;
            case jjma_dataTypes::INT:
                db_row[header] = std::stoi(value);
                break;
            case jjma_dataTypes::DOUBLE:
                db_row[header] = std::stod(value);
                break;
            case jjma_dataTypes::STRING:
                db_row[header] = value;
                break;
            default:
                std::cout << "Unknown data type found in schema. Please correct the schema." << std::endl;
                return {};
        }
    }
    return db_row;
}

void write_to_jjdb(std::ostream& jjdb_file, std::string& serialised_row) {
    jjdb_file << serialised_row << std::endl;
    return;
}

std::string serialise_jjdb_row(const jjdb_row& db_row) {
    std::string serialised_row;
    for (const auto& [header, field] : db_row) {
        serialised_row += header + ":";
        jjma_dataTypes col_type = global::db_schema_map.at(header);
        switch (col_type) {
            case jjma_dataTypes::ID:
                serialised_row += std::to_string(std::get<int>(field));
                break;
            case jjma_dataTypes::INT:
                serialised_row += std::to_string(std::get<int>(field));
                break;
            case jjma_dataTypes::DOUBLE:
                serialised_row += std::to_string(std::get<double>(field));
                break;
            case jjma_dataTypes::STRING:
                serialised_row += std::get<std::string>(field);
                break;
            default:
                std::cout << "Unknown data type found in schema. Please correct the schema." << std::endl;
                return "";
        }
        serialised_row += ";";
    }
    return serialised_row;
}

void append_row(jjdb_row& db_row) {
    if (!global::db_open) {
        std::cout << "No JJDB is open. Open one to append to it." << std::endl;
        return;
    }
    db_row["auto_id"] = find_num_records();
    
    std::string db_name = global::cwd->filename().string();
    std::filesystem::path jjdb_path = *global::cwd / (db_name + ".jjdb");

    std::ofstream jjdb_file(jjdb_path, std::ios::app);
    serialise_and_write_to_jjdb(jjdb_file, db_row);
    jjdb_file.close();
}

int find_num_records() {
    if (!global::db_open) {
        std::cout << "No JJDB is open. Open one to find the number of records." << std::endl;
        return -1;
    }
    std::string db_name = global::cwd->filename().string();
    std::filesystem::path jjdb_path = *global::cwd / (db_name + ".jjdb");

    std::ifstream jjdb_file(jjdb_path);
    std::string line;
    int num_records = 0;
    while (std::getline(jjdb_file, line)) {
        num_records++;
    }
    jjdb_file.close();
    return num_records;
}

void update_jjdb_row(int id, jjdb_row& db_row) {
    if (!global::db_open) {
        std::cout << "No JJDB is open. Open one to update a row." << std::endl;
        return;
    }
    std::string db_name = global::cwd->filename().string();
    std::filesystem::path jjdb_path = *global::cwd / (db_name + ".jjdb");
    std::filesystem::path tmp_path = jjdb_path.string() + ".tmp";

    std::ofstream tmp_file(tmp_path);
    std::ifstream jjdb_file(jjdb_path);

    std::string line;
    for (int i = 0; i < id; ++i) {
        if (!std::getline(jjdb_file, line)) {
            std::cout << "ID provided is greater than the largest auto_id value in the database." << std::endl;
            return;
        }
        tmp_file << line << std::endl;
    }

    std::string old_line;
    getline(jjdb_file, old_line);
    jjdb_row old_row = deserialise_jjdb_row(lstrip(old_line));
    jjdb_row new_row = old_row;
    for (const auto& [header, field] : db_row) {
        new_row[header] = field;
    }
    std::string new_line = serialise_jjdb_row(new_row);

    tmp_file << new_line << std::endl;

    while (getline(jjdb_file, line)) {
        tmp_file << line << std::endl;
    }

    jjdb_file.close();
    tmp_file.close();

    std::filesystem::remove(jjdb_path);
    std::filesystem::rename(tmp_path, jjdb_path);
    return;
}

void delete_jjdb_row(int id) {
    if (!global::db_open) {
        std::cout << "No JJDB is open. Open one to update a row." << std::endl;
        return;
    }
    std::string db_name = global::cwd->filename().string();
    std::filesystem::path jjdb_path = *global::cwd / (db_name + ".jjdb");
    std::filesystem::path tmp_path = jjdb_path.string() + ".tmp";

    std::ofstream tmp_file(tmp_path);
    std::ifstream jjdb_file(jjdb_path);

    std::string line;
    for (int i = 0; i < id; ++i) {
        if (!std::getline(jjdb_file, line)) {
            std::cout << "ID provided is greater than the largest auto_id value in the database." << std::endl;
            return;
        }
        tmp_file << line << std::endl;
    }

    tmp_file << std::endl;
    getline(jjdb_file, line); // deleting file
    
    while (getline(jjdb_file, line)) {
        tmp_file << line << std::endl;
    }

    jjdb_file.close();
    tmp_file.close();

    std::filesystem::remove(jjdb_path);
    std::filesystem::rename(tmp_path, jjdb_path);
    return;
}