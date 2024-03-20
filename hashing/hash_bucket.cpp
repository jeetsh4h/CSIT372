#include "read_csv.hpp"
#include "hash_bucket.hpp"

int main() {
    std::vector<std::string> first_names = read_csv("./test.csv", 1000);
    if (first_names.empty()) {
        return -1;
    }

    bucketHash hash_table;
    for (std::string name : first_names) {
        hash_table.insert(name);
    }

    // searching through the hash table
    for (std::string name : first_names) {
        hash_table.exists(name);
    }

    printf("Average bucket size: %f\n", hash_table.average_bucket_size());
    return 0;
}