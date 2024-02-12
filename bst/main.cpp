#include <fstream>

#include "bst.hpp"

Person parse_person_from_line(std::string line) {
    Person *person = new Person();

    size_t pos = 0;
    std::string token;
    int field = 0;

    while ((pos = line.find(',')) != std::string::npos) {
        token = line.substr(0, pos);
        line.erase(0, pos + 1);
        switch (field) {
            case 0:
                person->id = std::stoi(token);
                break;
            case 1:
                person->first_name = token;
                break;
            case 2:
                person->last_name = token;
                break;
            case 3:
                person->email = token;
                break;
        }
        field++;
    }
/* sanity check */
    // if (field == 4) {
    //     person->gender = line;
    // }
    person->gender = line;

    return *person;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: <name_to_search>\n");
        return -1;
    }
    std::string name_to_search = argv[1];

    // std::ifstream csv_file("./MOCK_DATA.csv");
    std::ifstream csv_file("./test.csv");
    BSTree tree;

    if (csv_file) {
        std::string line;
        while (std::getline(csv_file, line)) {
            Person person = parse_person_from_line(line);
            tree.addNode(person);
        }
        csv_file.close();
    } else {
        printf("Failed to open the CSV file.\n");
        return -1;
    }

    Person *found = tree.search(name_to_search);
    if (found != nullptr) {
        found->print();
    } else {
        printf("Person not found.\n");
    }

    return 0;
}