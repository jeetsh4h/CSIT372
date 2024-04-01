#include <string>
#include <iostream>

enum MetaCommandResult {
    MC_SUCCESS,
    MC_UNRECOGNIZED_COMMAND
};

// TODO: make sure this is changed
// Discuss everything with one more person
// so you have it decided in your head
MetaCommandResult do_meta_command(std::string input) {
    if (input[0] == '.') {
        return MC_SUCCESS;
    }

    if (input == ".exit") {
        std::cout << "Exiting the database..." << std::endl;
        exit(EXIT_SUCCESS);
    } else {
        return MC_UNRECOGNIZED_COMMAND;
    }
}

void parse_input(std::string input) {
    if (input[0] == '.') {
        switch (do_meta_command(input)) {
            case (MC_SUCCESS):
                // execute meta command
                break;
            case (MC_UNRECOGNIZED_COMMAND):
                std::cout << "Unrecognized command: " << input << std::endl;
                break;
        }
    }
}

void dbLoop() {
    std::string input;
    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, input);

        // TODO: implement what to do in the loop after parsing
        parse_input(input);
    }
}
