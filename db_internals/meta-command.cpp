#include <iostream>

#include "meta-command.hpp"

// TODO: Implement the exec_meta_command function
// make sure to create an enum or something
void exec_meta_command(const std::string& command) {
    // type shenanigans
    char *cmd = const_cast<char *>(command.data()); 

    switch (cmd) {
        case "exit":
            exit(EXIT_SUCCESS);
            break;
        default:
            std::cout << "Unrecognized command: " << command << std::endl;
            break;
    }
}