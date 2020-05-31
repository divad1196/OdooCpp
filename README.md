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
