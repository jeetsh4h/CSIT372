#include <vector>
#include <fstream>

#include "csv.hpp"
#include "index.hpp"


int main() {
    std::vector<Person *> people;
    std::ifstream csv_file("./test.csv", std::ios::in);

    if (csv_file) {
        std::string line;
        while (std::getline(csv_file, line)) {
            people.push_back(parse_person_from_line(line));
        }
    }

    std::vector<index_field> index = index_people_by_field(people, index::email);

    int check = 1;
    for (index_field field : index) {
        field.print();
        check++;
        if (check > 10) {
            break;
        }
    }

    return 0;
}