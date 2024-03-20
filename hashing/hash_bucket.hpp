#include <list>

#include "hashing.hpp"


struct bucketHash {
    size_t max_memory_addr = 1009;
    size_t max_bucket_size = 10;
    
    std::list<std::string> values[1009];

    void insert(std::string value);
    bool exists(std::string value);
    void delete_value(std::string value);

    float average_bucket_size();
};



void bucketHash::insert(std::string value) {
    if (value == "" || value == "####") {
        printf("Error: can not insert value: %s\n", value.c_str());
        return;
    }

    int hash_value = hash_first_name(value, max_memory_addr);

    if (values[hash_value].size() < max_bucket_size) {
        values[hash_value].push_back(value);
        return;
    } else {
        printf("Bucket overflowed, there is something wrong... debug\n");
        return;
    }
}

bool bucketHash::exists(std::string value) {
    int hash_value = hash_first_name(value, max_memory_addr);

    for (std::string val : values[hash_value]) {
        if (val == value) {
            return true;
        }
    }

    return false;
}

void bucketHash::delete_value(std::string value) {
    int hash_value = hash_first_name(value, max_memory_addr);

    for (std::list<std::string>::iterator it = values[hash_value].begin(); it != values[hash_value].end(); it++) {
        if (*it == value) {
            values[hash_value].erase(it);
            return;
        }
    }
    return;
}


float bucketHash::average_bucket_size() {
    float total_bucket_size = 0;

    for (size_t i = 0; i < max_memory_addr; i++) {
        total_bucket_size += values[i].size();
    }

    return total_bucket_size / (float) max_memory_addr;
}