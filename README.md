# C++ OdooCpp

**author**: Gallay David

### Goals

* powerful
* simple
* automated
* as if in Odoo's python code

### Requirements
#### Dependencies
* std=c++11
* libcurl

#### Tests

Tests are done using [Catch2](https://github.com/catchorg/Catch2) C++ testing framework. It is BDD-style and header-only.
This comes with the recursive clone and is not mandatory for the library to work.

#### Included thirdparty
Those library are included as git submodules, nothing needs to be done
* [OdooRPC](https://github.com/divad1196/OdooRPC): minimal version of this library
* [nlohmann/json](https://github.com/nlohmann/json/tree/master): json library that can be access through this library

### Installation

depending on your git version, you can try one of the following

```bash
git clone --recursive https://github.com/divad1196/OdooCpp.git
```

```bash
git clone https://github.com/divad1196/OdooCpp.git
git submodule init
git submodule update
```

### Compilation

Compilation is done using [Cmake](https://cmake.org/).
for manual compilation, create a folder, e.g. "builds", and from there run:

```bash
cmake .. # Generate build tree and Makefile
make
```

Special flag can be used for profiling with [gprof](https://man7.org/linux/man-pages/man1/gprof.1.html)

```bash
cmake -DGRPOF=1 .. # use -pg flag
```


### Use

1. Create a client

   ```c++

   const std::string URL = "my-url.com";
   const std::string DATABASE = "my-database";

   // Create Credentials
   Credentials creds("login", "password");

   Odoo::Client client (
       URL,
       DATABASE,
       creds
   );
   ```

   could be written all-in-one

   ```c++
   Odoo::Client client (
       "my-url.com",
       "my-database",
       {
       	"login",
       	"password"
       }
   );
   ```

   This will automatically authenticate the user and retrieve it's uid which is needed for most of the queries.
   The uid is related to the database, it is stored into the RPC client.
   
2. Use high level features

   ```c++
   Odoo::Model new_partner_ids = client.model("res.partner").create(R"({
   	"name": "abc",
   	"supplier": true
   })"_json);
   ```

   Here we use [raw-string-literal](https://www.geeksforgeeks.org/raw-string-literal-c/) from C++11 and [user-defined-literals](https://en.cppreference.com/w/cpp/language/user_literal) (""_json) from nlohmann's json library.

   model keeps a [std::shared_ptr](https://fr.cppreference.com/w/cpp/memory/shared_ptr) on OdooRPC object for convenience and safety.
   
   here are some examples:
   
   ```c++
   // Create a record
   Odoo::Model new_partner_ids = partner_ids.create(R"({
       "name": "abc",
       "supplier": true
   })"_json);
   std::cout << new_partner_ids << std::endl;
   
   // Update records
   new_partner_ids.write(R"({
       "customer": true,
       "name": "new abc"
   })"_json);
   
   // Create Model object matching the search
   Odoo::Model found_partner_ids = partner_ids.search(R"([
       	["supplier", "=", true]
       ])",
       0, 10
   );
   std::cout << found_partner_ids << std::endl;
   
   // Create a json object with requested values
   json read_partner_ids = partner_ids.search_read(R"([
       	["supplier", "=", true]
       ])",
       {"name", "customer"},
       0, 10, "id desc"
   );
   std::cout << read_partner_ids.dump(4) << std::endl;
   
   // Delete records
   new_partner_ids.unlink();
   std::cout << new_partner_ids << std::endl;
   
   // Return records that exists in database
   new_partner_ids = new_partner_ids.exists();
   std::cout << new_partner_ids << std::endl;
   
   ```
   
   Each of these methods checks if an error occurred and **will throw an Exception** if the server return an error.

Odoo does not hold a set of ids but a list of id, that means both + and | operator exists and are different.
The same behaviour is kept here.

```c++
...
Model m = client.env("res.partner").browse({1, 2, 3, 4});
m += {3, 4, 5}; // 1, 2, 3, 4, 3, 4, 5
m -= {4, 5}; // 1, 2, 3, 3
m |= {4, 5, 5, 6} // 1, 2, 3, 4, 5, 6
m &= {4, 5, 6, 7, 8, 9} // 4, 5, 6
```

NB:

* operators +, -, | and & also exists
* all operators that can take a list of ids can take a Model object.