#include <string>

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

/* this is based on sorting the first names alphabetically */
    bool operator<(const Person& other) const {
        return this->first_name < other.first_name;
    }

    bool operator>(const Person& other) const {
        return this->first_name > other.first_name;
    }

    bool operator==(const Person& other) const {
        return this->first_name == other.first_name;
    }

/* not needed in this particular situation */
    // bool operator!=(const Person& other) const {
    //     return this->first_name != other.first_name;
    // }

    // bool operator<=(const Person& other) const {
    //     return this->first_name <= other.first_name;
    // }

    // bool operator>=(const Person& other) const {
    //     return this->first_name >= other.first_name;
    // }

// debugging functions
    void print() const {
        printf("ID: %d, First Name: %s, Last Name: %s, Email: %s, Gender: %s\n", 
                this->id, this->first_name.c_str(), this->last_name.c_str(), 
                this->email.c_str(), this->gender.c_str()
            );
    }
};
