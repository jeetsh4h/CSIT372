# JJDB 
A fast and easy to use CLI based database system for storing and retrieving data.

## Installation
```bash
cd ${YOUR_PATH}
git clone https://github.com/jeetsh4h/CSIT372.git
cd ${YOUR_PATH}/CSIT372/db_internals
g++ -std=c++17 *.cpp -o jjdb
./jjdb
```

## Starting Out
To initialize or open an existing database:
`.open ${/path/to/database}/database_name;`  
A new folder will be created with the name of the database; all the files will be stored here.  
There are three types of file within an instance of a JJDB database:
- `database_name.jjdb` : This file contains all the raw data.
- `database_name.jjma` : Human-readable schema of the database.
- `header_name.jjdx`   : Index file for a given column of the database.

To perform any kind of queries within the database, one must create a schema file.
To do this:
- Create a .jjma file within the database folder
- The format for each line is as follows: `header_name : header_type`
- The database supports three types of data: `int`, `double`, and `string`
- First line of the schema **must** be: `auto_id : id`

**IMPORTANT**  
There can be no header name that is the same as `auto_id`. This is a reserved keyword for the database. No other headers/column can have the data type `id`.

<br>

One can also load a CSV file into an empty initialized database.
```bash
.open ${/path/to/database}/database_name;
.load_csv ${/path/to/csv}/file_name.csv;
```

The above commands will populate the jjdb file and create the schema for the same. If the schema is wrongly inferred, one can manually change it. The loaded csv **must** have a header row. Moreover, no field within the csv can have the character `:` and `,`. 

_Note_: the paths can be relative to the directory in which the jjdb executable is executed from. To check the database's current working directory, just type `.cwd;`  
The above command will print out the path to the executable's current working directory, if no database is open or the path of the database that is open.

Other meta-commands that are supported by JJDB are:
- `.drop;` : Deletes the current open database.
- `.close;` : Closes the current open database.
- `.exit;` : Exits the JJDB CLI.

<br>

**IMPORTANT**  
JJDB supports chaining commands and queries, within a single prompt. However, this is not to be assumed to be transactional. If an error occurs within the chain, the following commands will still be executed.
Every command must be terminated by a `;` character. The prompt will not pass the input until it has been terminated with a `;` character.

## Querying
JJDB supports a lite version of SQLite, we call it SQLite lite.  
Every single word/token within a query must be seperated by a single _space_ character, unless mentioned otherwise. 


Here are the following queries that are supported by JJDB:


### CREATE

- `insert header1=value1,header2=value2,header3=value3...;`

Not all header-value pairs must be present in the insert query. The headers that are not present will be set to default values based on their data types. The headers that are present must be present in the schema.
The default values are as followed:  

- `int` :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 0
- `double` : 0.0
- `string` : "NA"

The new record will be appended to the database. The `auto_id` will be incremented by 1 for every new record that is inserted into the database.


### READ

- `select * where ( condition );`
- `select header1,header2,header3... where ( condition );`
- `select * where ( condition ) take k;`


The *condition* is a boolean expression and it supports the following operators:
- `==` : Equality
- `!=` : Not Equal
- `>`  : Greater Than
- `<`  : Less Than
- `>=` : Greater Than or Equal
- `<=` : Less Than or Equal
- `&&` : Logical AND
- `||` : Logical OR

The condition must be enclosed within round brackets `()`. Moroever, the condition must have exactly one space between both brackets: `( condition )`.

The `take k` clause is optional and is used to limit the number of rows to `k` that are returned by the query. If the `take k` clause is not present, all the rows that satisfy the condition will be returned. Take always returns the **first** `k` rows that satisfy the condition.

The headers that must be returned must be present in the schema. The headers must be delimited by a comma `,` character. Spaces will not be considered as a delimiter; cause syntax errors. `*` is used to return all the headers for the open database.

**IMPORTANT**  
The `where ( condition )` clause is currently non-functional. It will be implemented in the near-future. 

### UPDATE

- `update header1=value1,header2=value2,...;`
- `update header1=value1,header2=value2,... where ( condition );`
- `update header1=value1,header2=value2,... where k;`

The headers not present in the query will retain their original data. If only first_name is mentioned, then the last_name will not be changed.  
The `k` is the auto_id of the record you wish to update. The `k` must be a non-negative integer. The `k` must be present in the database. If the `k` is not present, the query will not be executed.  
Refer [here](###READ) for more information about conditionals.


### DELETE

- `delete where ( condition );`
- `delete where k;`

The `k` is the auto_id of the record you wish to delete. The `k` must be a non-negative integer. The `k` must be present in the database. If the `k` is not present, the query will not be executed.  
Refer [here](###READ) for more information about conditionals.

### INDEXING

- `create header_name;`
- `drop header_name;`

The `create` query will create an index file for the header_name. The index file will be stored in the database folder. The index file will be named as `header_name.jjdx`. The index file stores the column-wise values of the `header_name` column in a sorted order with a reference to the record's `auto_id`. The index file can only be created if the `header_name` is present in the open database's schema.

The `drop` query will delete the index file for the `header_name`.

Indexing is used to speed up queries by caching the sorted values for a given column and then using fast look-up data structures such as binary search trees, and B+ trees.

**IMPORTANT**  
Due to the current implementation of data read and write; in conjunction with the state of `where` clause implementation: queries are currently not using indexing for searching. This will be implemented in the near-future, along with the `where` clause.

