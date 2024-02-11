#include <iostream>
#include <fstream>
#include <string>

struct student {
    unsigned int roll_no;
    std::string first_name;
    std::string last_name;
    unsigned int age;
    std::string major;

    student() {
        roll_no = 0;
        first_name = "";
        last_name = "";
        age = 0;
        major = "";
    }

    student(unsigned int roll_no, std::string first_name, std::string last_name, unsigned int age, std::string major) {
        this->roll_no = roll_no;
        this->first_name = first_name;
        this->last_name = last_name;
        this->age = age;
        this->major = major;
    }

    void print() {
        std::cout << "Roll No: " << roll_no << std::endl;
        std::cout << "First Name: " << first_name << std::endl;
        std::cout << "Last Name: " << last_name << std::endl;
        std::cout << "Age: " << age << std::endl;
        std::cout << "Major: " << major << std::endl;
    }
};

void read(std::string file_path, unsigned int find_roll_no) {
    FILE *fp = fopen(file_path.c_str(), "rb");    
    student *s = new student();

    rewind(fp);
    fseek(fp, (find_roll_no - 1) * sizeof(student), SEEK_SET);
    
    fread(s, sizeof(student), 1, fp);
    s->print();

    fclose(fp);
    return;
}

void write(std::string file_path, student *s) {
    FILE *fp = fopen(file_path.c_str(), "ab");   
    
    fwrite(s, sizeof(student), 1, fp);
    fclose(fp);
}