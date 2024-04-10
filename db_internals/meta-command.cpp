#include <iostream>

#include "global.hpp"

#include "parse-utils.hpp"
#include "csv-parsing.hpp"
#include "jjma.hpp"
#include "jjdb.hpp"

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
            *global::cwd = std::filesystem::current_path();
            global::db_open = false;
            break;

        case MetaCommand::CWD:
            std::cout << *global::cwd << std::endl;
            break;

        case MetaCommand::DROP:
            // delete the current working directory (the database directory)
            if (global::db_open) {
                std::filesystem::remove_all(*global::cwd);
                // and then call close
                *global::cwd = std::filesystem::current_path();
                global::db_open = false;
            } else {
                std::cout << "No JJDB is open. Please open the JJDB before dropping it." << std::endl;
            }
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

    if (command == "open") {
        return MetaCommand::Open;
    }

    if (command == "cwd") {
        return MetaCommand::CWD;
    }

    if (command == "close") {
        return MetaCommand::Close;
    }

    if (command == "drop") {
        return MetaCommand::DROP;
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
    if (!global::db_open) {
        std::cout << "No JJDB is open. Please open a JJDB before loading a CSV file." << std::endl;
        return;
    }

    std::filesystem::path csv_path(path);

    // check if the file exists
    if (!std::filesystem::exists(csv_path)) {
        std::cout << "File does not exist." << std::endl;
        return;
    }

    // check if the file is a csv file
    if (csv_path.extension() != ".csv") {
        std::cout << "Path provided is not to a CSV file." << std::endl;
        return;
    }

    build_jjma_for_csv(parse_csv(csv_path));
    build_jjdb_for_csv(csv_path);
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

    if (!std::filesystem::exists(dir_path)) {
        if (std::filesystem::create_directory(dir_path)) {
            *global::cwd = dir_path;
            global::db_open = true;
            return;
        } else {
            std::cout << "Cannot create this directory." << std::endl;
            return;
        }
    }

    if (!std::filesystem::is_directory(dir_path)) {
        std::cout << "Path should be to a directory" << std::endl;
        return;
    } else {
        /* const std::filesystem::directory_entry& */
        for (auto const& dir_entry : std::filesystem::directory_iterator{dir_path}) {
            std::filesystem::path dir_entry_path = dir_entry.path();
            const std::string dir_entry_ext = dir_entry_path.extension().string();

            if (dir_entry_ext == ".jjdb" ||
                dir_entry_ext == ".jjma" ||
                dir_entry_ext == ".jjdx") {
                    continue;
            } else {
                std::cout << "Not a valid JJDB directory" << std::endl;
                return;
            }
        }
        *global::cwd = dir_path;
        global::db_open = true;
        return;
    }

}
