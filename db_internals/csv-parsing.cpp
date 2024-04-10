#include <filesystem>
#include <map>

#include "jjma.hpp"
#include "build-jjdb.hpp"

#include "csv-parsing.hpp"

/*
 * Assuming that the CSV file **has headers**
 * All blank values will be treated as NULL values
 * 
 * The db_id needs to be the first row and has to start
 * with 0... and then increment by one
 * 
 * If the first column does not match that pattern then
 * we create it within the struct by default.
 */
std::map<std::string, jjma_dataTypes> parse_csv(const std::filesystem::path& file_path) {
    // TODO: Jash is implementing

    /*
     * This function takes the file path to the csv file
     * and then creates a map of the column-names to dataTypes.
     * 
     * The map is then written to the jjma file.
     * 
     * The next step will be to generate the struct 
     * that is used to interface with the database.
     */
}