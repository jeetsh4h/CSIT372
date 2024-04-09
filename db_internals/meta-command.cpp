#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "global.hpp"

#include "parse-utils.hpp"
#include "csv-parsing.hpp"

#include "meta-command.hpp"

// TODO: Implement the exec_meta_command function
// make sure to create an enum or something
void exec_meta_command(const std::string& command) {
    // type shenanigans
    MetaCommand cmd = getMetaCommand(command); 

    switch (cmd) {
        case MetaCommand::Exit:
            exit(EXIT_SUCCESS);
            break;
        
        case MetaCommand::LoadCSV:
            // only passing along the path
            load_csv(command.substr(9));
            break;

        case MetaCommand::Open:
            // only passing along the path
            open_db(command.substr(5));
            break;

        case MetaCommand::Close:
            // TODO: implement the close command
            *global::cwd = std::filesystem::current_path();
            break;

        case MetaCommand::CWD:
            std::cout << *global::cwd << std::endl;
            break;

        case MetaCommand::Unknown:
            std::cout << "Unrecognized command: " << command << std::endl;
            break;
        
        default:
            std::cout << "ERROR: check getMetaCommand func; " 
            << (int) cmd << ' ' << command << std::endl;
            break;
    }
}

MetaCommand getMetaCommand(const std::string& command) {
    if (command == "exit") {
        return MetaCommand::Exit;
    }

    if (command == "close") {
        return MetaCommand::Open;
    }

    if (command == "cwd") {
        return MetaCommand::CWD;
    }

    std::vector<std::string> command_tokens = split_string(command, ' ');
    size_t len_command = command_tokens.size();
    
    if (len_command == 2 && command_tokens[0] == "load_csv") {
        return MetaCommand::LoadCSV;
    }

    if (len_command == 2 && command_tokens[0] == "open") {
        return MetaCommand::Open;
    }

    return MetaCommand::Unknown;
}

void load_csv(const std::string& path) {
    std::filesystem::path file_path(path);

    // check if the file exists
    if (!std::filesystem::exists(file_path)) {
        std::cout << "File does not exist" << std::endl;
        return;
    }

    // check if the file is a csv file
    if (file_path.extension() != ".csv") {
        std::cout << "Not a csv file" << std::endl;
        return;
    }

    csv_to_table(file_path);
    return;
}

/*
 * open essentially accesses the directory which
 * we are going to be currently working in
 * 
 * the directory will have three kinds of files:
 * *.jjdb -> this is the actual data, this represents one table
 * *.jjma -> this is the schema of the table (*.jjdb file)
 * *.jjdx -> these are the index files for the given table, 
 *           they can be multiple for a single table
 * 
 * the open command will either verify that the directory 
 * we are trying to open is a valid database directory
 * If the directory provided does not exist, it will create it
 * and make it a valid database directory create the jjdb and jjma file
 * 
 * the directory name is the name of the table
 */
void open_db(const std::string& path) {
    std::filesystem::path dir_path(path);

    // TODO: Implement the open command
    *global::cwd = dir_path;
}
