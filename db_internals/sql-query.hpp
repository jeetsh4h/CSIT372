#ifndef SQL_QUERY_HPP
#define SQL_QUERY_HPP

/* header-value pair, where condition, take */
void exec_select(const std::string&, const std::vector<std::string> = std::vector<std::string>(), int = -1);
void exec_insert(const std::string&);
void exec_update(const std::string&, const std::vector<std::string>, int);


#endif /* SQL_QUERY_HPP */