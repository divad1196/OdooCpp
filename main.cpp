#include <stdlib.h>
#include <iostream>
#include "src/client.h"
#include <string>


int main(int argc, char *argv[]) {
    if(argc < 4){
        std::cerr << "Parameters: URL DATABASE LOGIN PASSWORD" << std::endl;
        return EXIT_FAILURE;
    }

    const char* URL = argv[1];
    const char* DATABASE = argv[2];
    const char* LOGIN = argv[3];
    const char* PASSWORD = argv[4];

    std::cout   << "URL: "      << URL          << '\n'
                << "DATABASE: " << DATABASE     << '\n'
                << "LOGIN: "    << LOGIN        << '\n'
                << "PASSWORD: " << "******"     << '\n'
    << std::endl;

    Odoo::Client client (
        URL,
        DATABASE,
        {
            LOGIN,
            PASSWORD
        }
    );
    
    Odoo::Model partner_ids = client.model("res.partner");
    std::cout << partner_ids << std::endl;
    
    Odoo::Model new_partner_ids = partner_ids.create(R"({
        "name": "abc",
        "supplier": true
    })"_json);
    std::cout << new_partner_ids << std::endl;

    new_partner_ids.write(R"({
        "customer": true,
        "name": "new abc"
    })"_json);

    Odoo::Model found_partner_ids = partner_ids.search(R"([
            ["supplier", "=", true]
        ])",
        0, 10
    );
    std::cout << found_partner_ids << std::endl;

    json read_partner_ids = partner_ids.search_read(R"([
            ["supplier", "=", true]
        ])",
        {"name", "customer"},
        0, 10, "id desc"
    );
    std::cout << read_partner_ids.dump(4) << std::endl;

    new_partner_ids.unlink();
    std::cout << new_partner_ids << std::endl;

    new_partner_ids = new_partner_ids.exists();
    std::cout << new_partner_ids << std::endl;


    return EXIT_SUCCESS;
}