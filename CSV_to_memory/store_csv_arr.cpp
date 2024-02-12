#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Person {
    unsigned int id;
    std::string first_name;
    std::string last_name;
    std::string email;
    std::string gender;
};

int main() {
    std::ifstream csv_file("./MOCK_DATA.csv");
    std::vector<Person> persons;

    if (csv_file) {
        std::string line;
        while (std::getline(csv_file, line)) {
            Person person;

            size_t pos = 0;
            std::string token;
            int field = 0;

            while ((pos = line.find(',')) != std::string::npos) {
                token = line.substr(0, pos);
                line.erase(0, pos + 1);
                switch (field) {
                    case 0:
                        person.id = std::stoi(token);
                        break;
                    case 1:
                        person.first_name = token;
                        break;
                    case 2:
                        person.last_name = token;
                        break;
                    case 3:
                        person.email = token;
                        break;
                }
                field++;
            }
            if (field == 4) {
                person.gender = line;
            }
            persons.push_back(person);
        }
        csv_file.close();
    } else {
        std::cout << "Failed to open the CSV file." << std::endl;
        return 1;
    }

    size_t test = 0;
    for (const Person curr_per : persons) {
        if (test > 10) {
            break;
        }
        printf("ID: %d, First Name: %s, Last Name: %s, Email: %s, Gender: %s\n", 
                curr_per.id, curr_per.first_name.c_str(), curr_per.last_name.c_str(), 
                curr_per.email.c_str(), curr_per.gender.c_str()
            );
        
        test++;
    }

    return 0;
}