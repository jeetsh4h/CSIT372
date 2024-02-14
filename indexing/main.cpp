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
    csv_file.close();

    std::vector<index_field<std::string>> first_name_index = index_field_vector<std::string>(people, first_name);
    std::string fname_index_file_path = save_index_on_disk(first_name_index, first_name); 
    
/* 
 * cannot delete objects allocated on the stack
 * therefore assuming that the first_name_index does not exist
 */
    // delete &first_name_index;
    std::vector<index_field<std::string>> fname_index_from_file = load_index_from_disk<std::string>(fname_index_file_path);

/* for debugging purposes,, this works */
    for (index_field<std::string> idx : fname_index_from_file) {
        people[idx.index - 1]->print();
    }

    return 0;
}