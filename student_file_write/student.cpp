#include "student.hpp"

void createFile(const std::string& file_path) {
    std::ofstream file(file_path);
    file.close();
}

void recreateFile(const std::string& file_path) {
    std::remove(file_path.c_str());
    createFile(file_path);
}

int main() {
    student student1(1, "John", "Doe", 20, "Computer Science");
    student student2(2, "Jane", "Doe", 19, "Computer Science");
    student student3(3, "John", "Goddamn", 21, "Computer Science");
    student student4(4, "Jane", "Smith", 18, "Computer Science");
    student student5(5, "Jeet", "Doe", 20, "Computer Science");
    student student6(6, "Archit", "Doe", 19, "Computer Science");
    student student7(7, "John", "Smith", 21, "Computer Science");
    student student8(8, "Jane", "Crazy", 18, "Computer Science");
    student student9(9, "Crazy", "Rich", 24, "Film & Television");

    student *students[] = {&student1, &student2, &student3, &student4, &student5, &student6, &student7, &student8};

    std::string file_path = "students.dat";
    
    std::ifstream file_check(file_path);
    if (file_check.good()) {
        file_check.close();
        recreateFile(file_path);
    } else {
        file_check.close();
        createFile(file_path);
    }
    

    for (student *curr_student: students) {
        // curr_student->print();
        write(file_path, curr_student);
    }
    
    read(file_path, 8);
    std::cout << std::endl;


    write(file_path, &student9);
    read(file_path, 9);

    return 0;
}