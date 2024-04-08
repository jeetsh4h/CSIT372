#include <iostream>

#include "interface-parsing.hpp"
#include "parse-utils.hpp"
#include "meta-command.hpp"

void dbLoop() {
    while (true) {
        int parse_result = -1;
        std::string input;
        std::cout << ">>> ";
        
        // Have to end the command input with a newline
        // for the getline to give back control to dbLoop
        std::getline(std::cin, input);

        std::string rstrip_input = rstrip(input);
        if (rstrip_input[rstrip_input.size() - 1] == ';') {
            parse_result = parse_input(rstrip_input);
        } else {
            while (true) {
                std::cout << "... ";
                std::string input_continue;
                
                std::getline(std::cin, input_continue);

                std::string rstrip_input_continue = rstrip(input_continue);
                input += rstrip_input_continue;
                if (rstrip_input_continue[rstrip_input_continue.size() - 1] == ';') {
                    parse_result = parse_input(input);
                    break;
                }
            }
        }

        if (parse_result == -1) {
            std::cout << "Error parsing the input\n";
        }
    }
}

/*
 * The commands must be terminated by a semicolon
 * The function which the command will call 
 * will be done from here itself
 * 
 * The control will be handed back to parse_input function
 * after the command has been executed.
 * 
 * Unless the command is something like .exit
 */
int parse_input(const std::string& input) {
    std::string stripped_input = strip(input);
    std::vector<std::string> commands = split_string(stripped_input, ';');

    for (const std::string& command : commands) {
    /* DEBUG */
        std::cout << command << std::endl;
    /* DEBUG */
    if (command[0] == '.') {
        std::string meta_command = command.substr(1);
        exec_meta_command(meta_command);
    } else {

    }


    return 0;
}