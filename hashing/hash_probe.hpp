#include "hashing.hpp"

struct naiveHash {
    size_t max_memory_addr = 1009;
    std::string values[1009] = {""};

    void insert(std::string value);
    bool exists(std::string value);
    void delete_value(std::string value);

    int total_searches_per_val = 0;
    int total_searches = 0;

    float average_searches = -1.0;
};


void naiveHash::insert(std::string value) {
    if (value == "" || value == "####") {
        printf("Error: can not insert value: %s\n", value.c_str());
        return;
    }

    int hash_value = hash_first_name(value, max_memory_addr);

    if (values[hash_value] == "" || values[hash_value] == "####") {
        values[hash_value] = value;
        return;
    } else {
        int roll_over_value = (hash_value + 1) % max_memory_addr;

        while (roll_over_value != hash_value) {
            if (values[roll_over_value] == "" || values[roll_over_value] == "####") {
                values[roll_over_value] = value;
                return;
            }
            roll_over_value = (roll_over_value + 1) % max_memory_addr;
        }
    }

    printf("Error: could not insert value: %s\n", value.c_str());
    return;
}

// linear probe
bool naiveHash::exists(std::string value) {
    int hash_value = hash_first_name(value, max_memory_addr);

    int num_of_searches = 0;
    if (values[hash_value] == value) {
        num_of_searches++;

        // printf("Number of searches: %d\n", num_of_searches);
        total_searches++;
        total_searches_per_val += num_of_searches;
        average_searches = (float) total_searches_per_val / (float) total_searches;

        return true;
    } else {
        int roll_over_value = (hash_value + 1) % max_memory_addr;

        while (roll_over_value != hash_value) {
            num_of_searches++;

            if (values[roll_over_value] == value) {
                // printf("Number of searches: %d\n", num_of_searches);
                total_searches++;
                total_searches_per_val += num_of_searches;
                average_searches = (float) total_searches_per_val / (float) total_searches;
                
                return true;
            }
            if (values[roll_over_value] == "") {
                // printf("Number of searches: %d\n", num_of_searches);
                total_searches++;
                total_searches_per_val += num_of_searches;
                average_searches = (float) total_searches_per_val / (float) total_searches;
                
                return false;
            }
            roll_over_value = (roll_over_value + 1) % max_memory_addr;
        }
    }

    // printf("Number of searches: %d\n", num_of_searches);
    
    total_searches++;
    total_searches_per_val += num_of_searches;
    average_searches = (float) total_searches_per_val / (float) total_searches;

    return false;
}

void naiveHash::delete_value(std::string value) {
    int hash_value = hash_first_name(value, max_memory_addr);

    if (values[hash_value] == value) {
        values[hash_value] = "####";
        return;
    } else {
        int roll_over_value = (hash_value + 1) % max_memory_addr;

        while (roll_over_value != hash_value) {
            if (values[roll_over_value] == value) {
                values[roll_over_value] = "####";
                return;
            }
            roll_over_value = (roll_over_value + 1) % max_memory_addr;
        }
    }

    printf("Error: could not delete value: %s\n", value.c_str());
    return;
}