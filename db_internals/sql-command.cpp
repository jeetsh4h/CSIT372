#include <iostream>

#include "sql-command.hpp"

void exec_sql_command(const std::string& sql_command) {
    std::cout << "Executing SQL command: " << sql_command << std::endl;
    return;
}