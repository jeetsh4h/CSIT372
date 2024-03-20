#include <string>
#include <random>

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
