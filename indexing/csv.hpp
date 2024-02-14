#include "person.hpp"

Person *parse_person_from_line(std::string line) {
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

    return person;
}