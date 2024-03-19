#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "person.hpp"

template<typename field>
struct index_field {
    int index;
    field sort_key;

    index_field() {
        this->index = -1;
        this->sort_key = field();
    }

    index_field(int index, field sort_key) {
        this->index = index;
        this->sort_key = sort_key;
    }
};

/* TODO: add a proper error message here */
template<typename field>
std::vector<index_field<field>> index_field_vector(const std::vector<Person *> people, person_fields sort_key) {

    std::vector<index_field<field>> index;
    switch (sort_key) {
        case id:
            if constexpr (std::is_convertible_v<field, unsigned short int>) {
                for (Person *person : people) {
                    index.push_back(index_field<field>(person->id, person->id));
                }
            }
            break;
        case first_name:
            if constexpr (std::is_convertible_v<field, std::string>) {
                for (Person *person : people) {
                    index.push_back(index_field<field>(person->id, person->first_name));
                }
            }
            break;
        case last_name:
            if constexpr (std::is_convertible_v<field, std::string>) {
                for (Person *person : people) {
                    index.push_back(index_field<field>(person->id, person->last_name));
                }
            }
            break;
        case email:
            if constexpr (std::is_convertible_v<field, std::string>) {
                for (Person *person : people) {
                    index.push_back(index_field<field>(person->id, person->email));
                }
            }
            break;
        case gender:
            if constexpr (std::is_convertible_v<field, std::string>) {
                for (Person *person : people) {
                    index.push_back(index_field<field>(person->id, person->gender));
                }
            }
            break;
    }

/* performs the sorting in-place,, the vector passed as the argument is worked on */
    mergesort(index, 0, index.size() - 1);
    return index;
}

template<typename field>
void mergesort(std::vector<index_field<field>>& index, int low, int high) {
    if (low < high) {
        int pivot = partition(index, low, high);
        mergesort(index, low, pivot - 1);
        mergesort(index, pivot + 1, high);
    }
}

template<typename field>
int partition(std::vector<index_field<field>>& index, int low, int high) {
    field pivot = index[high].sort_key;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (index[j].sort_key < pivot) {
            i++;
            std::swap(index[i], index[j]);
        }
    }

    std::swap(index[i + 1], index[high]);
    return i + 1;
    
}

template<typename field>
std::string save_index_on_disk(const std::vector<index_field<field>> index, person_fields sort_key) {
    std::string file_path;
    switch (sort_key) {
        case id:
            file_path = "./id_index.bin";
            break;
        case first_name:
            file_path = "./first_name_index.bin";
            break;
        case last_name:
            file_path = "./last_name_index.bin";
            break;
        case email:
            file_path = "./email_index.bin";
            break;
        case gender:
            file_path = "./gender_index.bin";
            break;
    }
    
    std::ofstream index_file(file_path, std::ios::out | std::ios::binary);
    
    for (index_field<field> idx_field : index) {
        index_file.write((char*)&idx_field, sizeof(index_field<field>));
    }

    index_file.close();

    return file_path;
}

template<typename field>
std::vector<index_field<field>> load_index_from_disk(std::string file_path) {    
    std::ifstream index_file(file_path, std::ios::in | std::ios::binary);
    
    std::vector<index_field<field>> index;
    index_field<field> idx_field;
    while (index_file.read((char*)&idx_field, sizeof(index_field<field>))) {
        index.push_back(idx_field);
    }

    index_file.close();
    return index;
}

template<typename field>
void insert_into_index(std::vector<index_field<field>>& index, index_field<field> record) {
    index.push_back(record);
    mergesort(*index, 0, index.size() - 1);
}


/* TODO: finish writing this function,,, quite a few things wrong with it */
template<typename field>
std::vector<index_field<field>> insert_index(const std::vector<index_field<field>>& index, field key_to_delete) {
    // finding which place to insert new record, or delete new record
    auto it = std::lower_bound(index.begin(), index.end(), search_key, [](const index_field<field>& idx, const field& key_to_delete) {
        return idx.sort_key < key_to_delete);
    });

    std::vector<index_field<field>> result;
    while (it != index.end() && it->sort_key == search_key) {
        result.push_back(*it);
        ++it;
    }

    return result;
}

/* TODO: write a function to search for persons using the index */