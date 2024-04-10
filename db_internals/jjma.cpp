#include <iostream>
#include <fstream>

#include "global.hpp"

#include "jjma.hpp"


std::filesystem::path build_jjma(const std::map<std::string, jjma_dataTypes> jjma_data) {
    if (!global::db_open) {
        std::cout << "No JJDB is open. Please open a JJDB before creating a JJMA file." << std::endl;
        return *global::cwd;
    }

    std::string db_name = global::cwd->filename().string();
    std::filesystem::path jjma_path = *global::cwd / (db_name + ".jjma");

    std::ofstream jjma_file(jjma_path);

    /* const std::string& key; const jjma_dataTypes& value */
    for (const auto& [key, value] : jjma_data) {
        switch (value) {
            case jjma_dataTypes::ID:
                jjma_file << key << " : auto_id;" << std::endl;
                break;
            case jjma_dataTypes::INT:
                jjma_file << key << " : int;" << std::endl;
                break;
            case jjma_dataTypes::DOUBLE:
                jjma_file << key << " : double;" << std::endl;
                break;
            case jjma_dataTypes::STRING:
                jjma_file << key << " : string;" << std::endl;
                break;
            case jjma_dataTypes::BOOL:
                jjma_file << key << " : bool;" << std::endl;
                break;
        }
    }

    jjma_file.close();
    return jjma_path;
}