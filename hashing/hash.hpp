#include <random>
#include <string>


int generate_random_number(int seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, 1000);
    
    return dist(rng);
}

int hash_first_name(std::string name, size_t max_memory_addr = 1009) {
    int odd_positional_sum = 0;
    int even_positional_sum = 0;
    for (size_t i = 0; i < name.length(); i++) {
        char c = name[i];
        int c_lower = (int) tolower(c);
        
        if ((i + 1) % 2 == 0) {
            even_positional_sum += c_lower * ((i + 1) >> 1);
        } else {
            odd_positional_sum += c_lower * ((i + 1) >> 1);
        }
    }
    int hash = even_positional_sum * odd_positional_sum;

    return (hash * generate_random_number(hash)) % max_memory_addr;
}


struct naiveHash {
    size_t max_memory_addr = 1009;
    std::string values[1009] = {""};

    void insert(std::string value);
    bool exists(std::string value);
    void delete_value(std::string value);
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

        printf("Number of searches: %d\n", num_of_searches);
        return true;
    } else {
        int roll_over_value = (hash_value + 1) % max_memory_addr;

        while (roll_over_value != hash_value) {
            num_of_searches++;

            if (values[roll_over_value] == value) {
                printf("Number of searches: %d\n", num_of_searches);
                return true;
            }
            if (values[roll_over_value] == "") {
                printf("Number of searches: %d\n", num_of_searches);
                return false;
            }
            roll_over_value = (roll_over_value + 1) % max_memory_addr;
        }
    }

    printf("Number of searches: %d\n", num_of_searches);
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