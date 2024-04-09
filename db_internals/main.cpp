/*
 * The command for compiling this project is:
 * `g++ -std=c++17 -Wall -Wpedantic -Werror *.cpp -o db_test.out && ./db_test.out`
 * The above command uses the g++ 9.4.0-1ubunutu1~20.04.2 version of the compiler
 * 
 * Make sure the above command is run in the same directory
 * as the file main.cpp
 */
#include <filesystem>

#include "global.hpp"
#include "interface-parsing.hpp"

int main() {
    // setting the current working directory in the heap
    // this will be updated by the open and close meta-commands
    cwd = new std::filesystem::path(std::filesystem::current_path());

    dbLoop();
}
