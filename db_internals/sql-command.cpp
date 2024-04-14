#include <iostream>

#include "global.hpp"
#include "parse-utils.hpp"
#include "sql-query.hpp"

#include "sql-command.hpp"

// void exec_sql_command(const std::string& sql_command) {
//     std::map<SQLCommand, std::string> query_split = parse_sql_command(sql_command);

//     // std::cout << "Executing SQL command: " << sql_command << std::endl;
//     return;
// }


void exec_sql_command(const std::string& sql_command) {
    if (!global::db_open) {
        std::cout << "No database is open. Please open a database before executing SQL commands." << std::endl;
        return;
    }

    std::vector<std::string> tokens = split_string(sql_command, ' ');
    
    int num_tokens = tokens.size();
    int token_idx = 0;

    /* Assumes non-empty command passed to this function */
    SQLCommand command = getSQLCommand(tokens.at(token_idx));
    if (command == SQLCommand::Unknown) {
        std::cout << "Unknown SQLite lite command: " << sql_command << std::endl;
        return;
    }

    /* Every command has an argument */
    if (num_tokens < 2) {
        std::cout << "No arguments passed to the command" << std::endl;
        return;
    }

    token_idx++;
    std::string cmd_arg = tokens.at(token_idx);

/* This is dealing with the first token of a sql query */
    switch (command) {
        case SQLCommand::Select:
            token_idx++;
            if (token_idx >= num_tokens) {
                exec_select(cmd_arg);
                return;
            }

            if (tokens.at(token_idx) == "where") {
                token_idx++;
                if (token_idx >= num_tokens) {
                    std::cout << "Expected condition after `where`" << std::endl;
                    return;
                }
                std::vector<std::string> condition_tokens;
                if (tokens.at(token_idx) == "(") {
                    token_idx++;
                    while (true) {
                        if (token_idx >= num_tokens) {
                            std::cout << "Expected `)` in SQLite lite query" << std::endl;
                            return;
                        }
                        if (tokens.at(token_idx) == ")") {
                            break;
                        }
                        condition_tokens.push_back(tokens.at(token_idx));
                        token_idx++;
                    }
                }

                token_idx++;
                if (token_idx >= num_tokens) {
                    exec_select(cmd_arg, condition_tokens);
                    return;
                }

                if (tokens.at(token_idx) == "take") {
                    token_idx++;
                    if (token_idx >= num_tokens) {
                        std::cout << "Expected number after `take`" << std::endl;
                        return;
                    }
                    int take_num = std::stoi(tokens.at(token_idx));
                    
                    token_idx++;
                    if (token_idx < num_tokens) {
                        std::cout << "Syntax Error: Expected `end of cmd`" << std::endl;
                        return;
                    }

                    exec_select(cmd_arg, condition_tokens, take_num);
                    return;
                } else {
                    std::cout << "Syntax Error: Only expecting `take` after condition" << std::endl;
                }
            }

            if (tokens.at(token_idx) == "take") {
                token_idx++;
                if (token_idx >= num_tokens) {
                    std::cout << "Expected number after `take`" << std::endl;
                    return;
                }

                int take_num;
                try {
                    take_num = std::stoi(tokens.at(token_idx));
                } catch (std::invalid_argument& e) {
                    std::cout << "not a number after `take`" << std::endl;
                    return;
                }

                token_idx++;
                if (token_idx < num_tokens) {
                    std::cout << "Syntax Error: Expected `end of cmd`" << std::endl;
                    return;
                }

                exec_select(cmd_arg, std::vector<std::string>(), take_num);
                return;
            }
            break;

        case SQLCommand::Where:
            std::cout << "Syntax Error; condition without command: " << sql_command << std::endl;
            return;

        case SQLCommand::Insert:
            token_idx++;
            if (token_idx < num_tokens) {
                std::cout << "Expected `end of cmd` after " << cmd_arg << std::endl;
                return;
            }
            
            exec_insert(cmd_arg);
            break;

        case SQLCommand::Update:
            token_idx++;
            if (tokens.at(token_idx) == "where") {
                token_idx++;
                if (token_idx >= num_tokens) {
                    std::cout << "Expected arguments after `where`" << std::endl;
                    return;
                }

                std::vector<std::string> condition_tokens = std::vector<std::string>();
                int condition_auto_id = -1;
                if (tokens.at(token_idx) == "(") {
                    token_idx++;
                    while (true) {
                        if (token_idx >= num_tokens) {
                            std::cout << "Expected `)` in SQLite lite query" << std::endl;
                            return;
                        }
                        if (tokens.at(token_idx) == ")") {
                            break;
                        }
                        condition_tokens.push_back(tokens.at(token_idx));
                        token_idx++;
                    }
                } else {
                    try {
                        condition_auto_id = std::stoi(tokens.at(token_idx));
                    } catch (std::invalid_argument& e) {
                        std::cout << "not a number after `where`" << std::endl;
                        return;
                    }
                }

                token_idx++;
                if (token_idx < num_tokens) {
                    std::cout << "Syntax Error: Expected `end of cmd`" << std::endl;
                    return;
                }

                exec_update(cmd_arg, condition_tokens, condition_auto_id);
                return;

            } else {
                std::cout << "Syntax Error: Expected `where` after " << cmd_arg << std::endl;
                return;
            }

        case SQLCommand::Delete:
            std::cout << "Deleting " << cmd_arg << std::endl;
            break;

        case SQLCommand::Take:
            std::cout << "Syntax Error: take without `select` query:" << sql_command << std::endl;
            break;

        case SQLCommand::Create:
            if (token_idx + 1 >= num_tokens) {
                std::cout << "Expected `end of cmd` after " << cmd_arg << std::endl;
                return;
            }
            break;

        case SQLCommand::Drop:
            if (token_idx + 1 >= num_tokens) {
                std::cout << "Expected `end of cmd` after " << cmd_arg << std::endl;
                return;
            }
            break;

        case SQLCommand::Unknown:
            std::cout << "Unknown SQLite lite command: " << sql_command << std::endl;
            break;
    }

    return;
}


SQLCommand getSQLCommand(const std::string& command) {
    std::string norm_command = to_lower(command);
    if (norm_command == "select") {
        return SQLCommand::Select;
    }

    if (norm_command == "where") {
        return SQLCommand::Where;
    }

    if (norm_command == "insert") {
        return SQLCommand::Insert;
    }

    if (norm_command == "update") {
        return SQLCommand::Update;
    }

    if (norm_command == "delete") {
        return SQLCommand::Delete;
    }

    if (norm_command == "drop") {
        return SQLCommand::Update;
    }

    if (norm_command == "take") {
        return SQLCommand::Take;
    }

    if (norm_command == "create") {
        return SQLCommand::Create;
    }

    if (norm_command == "drop") {
        return SQLCommand::Drop;
    }

    
    return SQLCommand::Unknown;
}
