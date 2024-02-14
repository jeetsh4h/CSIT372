#include <string>

/* useful for indexing */
enum person_fields {
    id          = 0,
    first_name  = 1,
    last_name   = 2,
    email       = 3,
    gender      = 4
};

struct Person {
    unsigned int id;
    std::string first_name;
    std::string last_name;
    std::string email;
    std::string gender;

    Person() {
        this->id = -1;
        this->first_name = "";
        this->last_name = "";
        this->email = "";
        this->gender = "";
    }

    Person(
        unsigned int id, std::string first_name, 
        std::string last_name, std::string email,
        std::string gender
    ) {
        this->id = id;
        this->first_name = first_name;
        this->last_name = last_name;
        this->email = email;
        this->gender = gender;
    }

// debugging functions
    void print() const {
        printf("ID: %d, First Name: %s, Last Name: %s, Email: %s, Gender: %s\n", 
                this->id, this->first_name.c_str(), this->last_name.c_str(), 
                this->email.c_str(), this->gender.c_str()
            );
    }
};

/* parsing string from CSV into the struct */
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