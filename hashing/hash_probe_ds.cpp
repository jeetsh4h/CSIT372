#include <fstream>
#include <iostream>

#include "hash.hpp"

std::string get_first_name_from_line(std::string line) {
    size_t pos = 0;
    std::string token;
    int field = 0;

    while ((pos = line.find(',')) != std::string::npos) {
        token = line.substr(0, pos);
        line.erase(0, pos + 1);
        if (field == 1) {
            return token;
        }
        field++;
    }
    return "";
}


int main() {
    std::string first_names[1000];

    std::ifstream csv_file("./test.csv");
    if (csv_file) {
        std::string line;
        size_t idx = 0;
        while (std::getline(csv_file, line)) {
            std::string first_name = get_first_name_from_line(line);
            if (first_name == "") {
                std::cout << "Error: first name is empty; idx: " << idx  << std::endl;
            }
            first_names[idx] = first_name;
            idx++;
        }
        csv_file.close();
    } else {
        printf("Failed to open the CSV file.\n");
        return -1;
    }

    naiveHash hash_table;   // can only accept non-empty strings
    for (std::string name : first_names) {
        hash_table.insert(name);
    }

    // searching through the hash table
    bool alyce_exists = hash_table.exists("Alyce");

    if (alyce_exists) {
        std::cout << "Alyce exists in the hash table." << std::endl;
    } else {
        std::cout << "Alyce does not exist in the hash table." << std::endl;
    }

    bool fields_exist = hash_table.exists("Fields");
    if (fields_exist) {
        std::cout << "Fields exists in the hash table." << std::endl;
    } else {
        std::cout << "Fields does not exist in the hash table." << std::endl;
    }

    bool jeet_exists = hash_table.exists("Jeet");
    if (jeet_exists) {
        std::cout << "Jeet exists in the hash table." << std::endl;
    } else {
        std::cout << "Jeet does not exist in the hash table." << std::endl;
    }

    bool ankita_exists = hash_table.exists("Ankita");
    if (ankita_exists) {
        std::cout << "Ankita exists in the hash table." << std::endl;
    } else {
        std::cout << "Ankita does not exist in the hash table." << std::endl;
    }

    bool janet_exists = hash_table.exists("Janet");
    if (janet_exists) {
        std::cout << "Janet exists in the hash table." << std::endl;
    } else {
        std::cout << "Janet does not exist in the hash table." << std::endl;
    }

    return 0;
}