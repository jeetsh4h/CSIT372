#include <vector>
#include <fstream>

#include "csv.hpp"

template <typename field>
struct index_field {
    unsigned int id;
    field sorted_field;
};

std::vector<index_field> person_sort_by_field(std::vector<Person *>, std::string);

int main() {
    std::vector<Person *> people;
    std::ifstream csv_file("./test.csv", std::ios::in);

    if (csv_file) {
        std::string line;
        while (std::getline(csv_file, line)) {
            people.push_back(parse_person_from_line(line));
        }
    }

    return 0;
}


auto person_sort_by_field(auto people, std::string field) {   
    switch (field) {
        case "id":
            std::vector<index_field<unsigned short int> unsorted_index;
            for (Person *p : people) {
                unsorted_index.push_back(index_field(p->id, p->id));
            }
            break;

        case "first_name":  
            for (Person *p : people) {
                unsorted_index.push_back(index_field(p->id, p->first_name));
            }  
            break;
        
        case "last_name":
            for (Person *p : people) {
                unsorted_index.push_back(index_field(p->id, p->last_name));
            }
            break;
        
        case "email":
            for (Person *p : people) {
                unsorted_index.push_back(index_field(p->id, p->email));
            }
            break;
        
        case "gender":
            for (Person *p : people) {
                unsorted_index.push_back(index_field(p->id, p->email));
            }
            break;
    }

    return unsorted_index;
}