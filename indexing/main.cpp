#include <vector>
#include <fstream>

#include "csv.hpp"

int main() {
    std::vector<Person *> people;
    std::ifstream csv_file("./test.csv");

    if (csv_file) {
        std::string line;
        while (std::getline(csv_file, line)) {
            people.push_back(parse_person_from_line(line));
        }
    }

/* test case for vector Person parsing into vector */
    // size_t check = 1;
    // for (Person *person : people) {
    //     person->print();
    //     check++;
    //     if (check > 10) {
    //         break;
    //     }
    // }

    return 0;
}