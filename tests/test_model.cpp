#define CATCH_CONFIG_MAIN // provides main(); this line is required in only one .cpp file
#include "Catch2/single_include/catch2/catch.hpp"
#include "../src/client.h"

#include <string>
#include <iostream>
using namespace Odoo;

TEST_CASE( "tests models", "[model]") {


    Client client (
        "URL",
        "DATABASE",
        {
            "LOGIN",
            "PASSWORD"
        }
    );

    Model m = client.env("res.partner").browse({1,2,3,4,5});

    SECTION( "Empty tree" ) {
        m += Ids({6, 7, 8});
        CHECK(std::string(m)  == "res.partner(1, 2, 3, 4, 5, 6, 7, 8)");

        m += Ids({6, 7, 8});
        CHECK(std::string(m)  == "res.partner(1, 2, 3, 4, 5, 6, 7, 8, 6, 7, 8)");

        m -= Ids({6, 7, 8});
        CHECK(std::string(m)  == "res.partner(1, 2, 3, 4, 5)");

        m |= Ids({4, 5, 6, 7});
        CHECK(std::string(m)  == "res.partner(1, 2, 3, 4, 5, 6, 7)");

        m &= Ids({6, 7, 8, 9});
        std::cout << m << std::endl;
        CHECK(std::string(m)  == "res.partner(6, 7)");
    }
}