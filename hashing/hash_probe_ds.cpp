#include <fstream>
#include <iostream>

#include "hash_probe.hpp"

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
    for (std::string name : first_names) {
        hash_table.exists(name);
    }

    printf("Average number of searches: %f\n", hash_table.average_searches);

    return 0;
}