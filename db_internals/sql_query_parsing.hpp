#include <iostream>
#include <string>

#include <sstream>
#include <iterator>
#include <vector>

/* DEBUG */
#include <fstream>
/* DEBUG */

enum SQLCommand {
    ERROR  = 0,
    NO_EOC = 1,     // No end of command -> no ';' at the end

// CRUD operations,,, should always be at the start of a command 
    SELECT = 2,
    INSERT = 3,
    UPDATE = 4,
    DELETE = 5,

// read operations based on logic
    FROM   = 6,
    WHERE  = 7,

// table-wise operations,,, should always be at the start of a command
// these are the commands that will be used to create and drop indices too
// write operations on disk
    CREATE = 8,
    DROP   = 9,
};

/*
 * We are only supporting the basic CRUD operations
 * INSERT, SELECT, UPDATE, DELETE
 * 
 * We are also allowing multiple tables within a database
 * at a time, however there will be no relational modelling
 * Therefore we will need two table-wise operations
 * CREATE, DROP
 * 
 * The function below will parse the input and form a vector
 * which will then call the necessary functions to execute a 
 * given query.
 * 
 * 
 * NOTE: Currently, we can only parse and execute one command at a time
 *     : if there is a semicolon within the command sent, we will just stop
 *     : reading and executing the command after that
 */
int parse_input(std::string input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{}};

    long unsigned int last_token_idx = -1;
    for (long unsigned int i = 0; i < tokens.size(); i++) {
        std::string token = tokens[i];

        if (token == ";" || token[token.size() - 1] == ';') {
            last_token_idx = i;
            break;
        }
    }

    

    /* DEBUG */
    std::ofstream debug("tokens.debug", std::ios::app);
    for (long unsigned int i = 0; i <= last_token_idx; i++) {
        std::string token = tokens[i];
        
        debug << token << " | ";
    }
    debug << std::endl;
    /* DEBUG */


// TODO: make sure this is correct
    return 0;
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


