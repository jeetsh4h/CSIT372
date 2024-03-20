#include <string>
#include <vector>
#include <fstream>

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


std::vector<std::string> read_csv(std::string file_path, size_t max_lines) {
    std::vector<std::string> first_names;

    std::ifstream csv_file(file_path);
    if (csv_file) {
        std::string line;
        size_t idx = 0;
        while (std::getline(csv_file, line)) {
            std::string first_name = get_first_name_from_line(line);
            if (first_name == "") {
                printf("Error: first name is empty; idx: %lu\n", idx);
            }
            first_names.push_back(first_name);
            idx++;
            if (idx >= max_lines) {
                break;
            }
        }
        csv_file.close();
    } else {
        printf("Failed to open the CSV file.\n");
        return std::vector<std::string>();
    }

    return first_names;
}