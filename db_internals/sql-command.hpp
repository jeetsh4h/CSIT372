#ifndef SQL_COMMAND_HPP
#define SQL_COMMAND_HPP

#include <string>
#include <vector>
#include <map>

/*
 * select header1,header2,header3...;
 * cannot have spaces for the header names
 * for all headers -> use `*`
 * 
 * select/delete/update ... where ( condition );
 * conditions supported &&, ||, ==, !=, <, >, <=, >=
 * conditions must be enclosed in round brackets
 * There must be only one space between the brackets and the
 * conditional statement
 * 
 * 
 * insert header1=value1,header2=value2...;
 * if all the header-value pairs are not provided then they
 * will use their default values
 * string = NA; int = 0; double = 0.0;
 * auto_id cannot be inserted, will be done automatically
 * 
 * update header1=value1,header2=value2... where ( condition );
 * 
 * delete k; delete where ( condition );
 * n can be the auto_id of the record to delete
 * 
 * select ... take k;
 * k is the number of rows to output
 * if no take, then all rows are ouput
 * 
 * create header;
 * it only accepts one header_name
 * 
 * drop header;
 * it only accepts one header_name
 * 
 * ***All whitespace within a command must be a single \space character***
 */
enum class SQLCommand {
    Select,     // read data
    Where,      // operation with a condition
    Insert,     // write new row
    Update,     // re-write old row(s)
    Delete,     // delete row(s)
    Take,       // output k rows
    Create,     // create a new index
    Drop,       // delete an index
    Unknown     // syntax error
};

void exec_sql_command(const std::string&);
std::map<SQLCommand, std::string> parse_sql_command(const std::string&);
SQLCommand getSQLCommand(const std::string&);

#endif /* SQL-COMMAND_HPP */