#ifndef META_COMMAND_HPP
#define META_COMMAND_HPP

#include <string>

enum class MetaCommand {
    Exit,       // .exit
    LoadCSV,    // .load_csv /path/to/csv
    Open,
    Close,
    CWD,
    Unknown
};

void exec_meta_command(const std::string&);
MetaCommand getMetaCommand(const std::string&);

void load_csv(const std::string&);


#endif /* META_COMMAND_HPP */