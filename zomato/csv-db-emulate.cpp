/*
 * On a cold start, this program, without parsing the line, took 1.584s
 * to print the entire file to the console (emulating the `SELECT * FROM PUNE;` query).
 * On a cold start, in the sqlite3 shell, the same query took 1.723s
 *
 * On a warm start, this program took 1.413s
 * On a warm start, in the sqlite3 shell, the same query took 1.436s
 *
 *
 * On a cold start, this program, with parsing the line, took 1.944s
 * to print the entire file to the console emulating the exact output of the query.
 *
 * On a warm start, this program took 1.603s
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * I believe the above results are not the best way to compare the two methods;
 * the bottleneck for both programs here is write to console operation.
 *
 * On a side note, the program can be made the tiniest bit faster not checking for the
 * command-line arguments as they are not necessary for THIS version of the program.
 *
 */

#include <fstream>
#include <iostream>
#include <string>

std::string parseLine(std::string);

int main(int argc, char **argv)
{
    std::ifstream csv_file;
    csv_file.open(".zomato/zomato_pune.csv");

    if (!csv_file.is_open())
    {
        std::cerr << "Failed to open the CSV file." << std::endl;
        return 1;
    }

    std::string line;
    while (getline(csv_file, line))
    {
        /*
         * This is the code snippet that parses the string
         * formats it based on my my arbitrary rules and then
         * prints it to the console.
         */

        std::string parsed_line = parseLine(line);
        std::cout << parsed_line << std::endl;

        /*
         * This prints the string as is, the way sqlite3 does it,
         * when using it in the shell.
         */

        // std::cout << line << std::endl;
    }

    csv_file.close();
    return 0;
}

std::string parseLine(std::string line)
{
    std::string parsed_line;
    bool inQuotes = false;

    for (char c : line)
    {
        if (c == '"')
            inQuotes = !inQuotes;
        else if (c == ',' && !inQuotes)
            parsed_line += '|';
        else
            parsed_line += c;
    }

    return parsed_line;
}