#include <string>
#include <fstream>
#include <iostream>

#include <random>
int generate_random_number(int seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, 1000);
    
    return dist(rng);
}

int hash_first_name(std::string name) {
    int odd_positional_sum = 0;
    int even_positional_sum = 0;
    for (size_t i = 0; i < name.length(); i++) {
        char c = name[i];
        int c_lower = (int) tolower(c);
        
        if ((i + 1) % 2 == 0) {
            even_positional_sum += c_lower * ((i + 1) >> 1);
        } else {
            odd_positional_sum += c_lower * ((i + 1) >> 1);
        }
    }
    int hash = even_positional_sum * odd_positional_sum;

    return (hash * generate_random_number(hash)) % 1009;
    // return hash % 1009;
}


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
            first_names[idx] = first_name;
            idx++;
        }
        csv_file.close();
    } else {
        printf("Failed to open the CSV file.\n");
        return -1;
    }

    std::ofstream hash_file("./hash.txt");
    for (std::string name : first_names) {
        hash_file << hash_first_name(name) << ',' << name << std::endl;
    }

    return 0;
}