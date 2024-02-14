#include <string>
#include <vector>

#include "person.hpp"

struct index_field {
    unsigned int id;
    std::string sort_key;

    index_field(unsigned int id, std::string sort_key) {
        this->id = id;
        this->sort_key = sort_key;
    }

    void print() {
        printf("ID: %d, Sort Key: %s\n", 
        this->id, this->sort_key.c_str());
    }
};

namespace index {
    const int id = 0;
    const int first_name = 1;
    const int last_name = 2;
    const int email = 3;
    const int gender = 4;
}

std::vector<index_field> index_sort(std::vector<index_field> index) {
    for (int i = 0; i < index.size(); i++) {
        for (int j = i + 1; j < index.size(); j++) {
            if (index[i].sort_key > index[j].sort_key) {
                index_field temp = index[i];
                index[i] = index[j];
                index[j] = temp;
            }
        }
    }
    return index;
}

std::vector<index_field> index_people_by_field(std::vector<Person *> people, int field) {
    std::vector<index_field> index;
    switch (field) {
        case index::id:
            break;
        
        case index::first_name:
            for (Person *person : people) {
                index.push_back(index_field(person->id, person->first_name));
            }
            break;
        
        case index::last_name:
            for (Person *person : people) {
                index.push_back(index_field(person->id, person->last_name));
            }
            break;
        
        case index::email:
            for (Person *person : people) {
                index.push_back(index_field(person->id, person->email));
            }
            break;
        
        case index::gender:
            for (Person *person : people) {
                index.push_back(index_field(person->id, person->gender));
            }
            break;
        
        default:
            break;
    }

    return index_sort(index);
}

