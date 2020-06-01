#ifndef ODOO_EXCEPTION_H
#define ODOO_EXCEPTION_H

#include <exception>
#include <string>
#include "json.h"

namespace Odoo {
    class ServerError: public std::runtime_error {
        public:
            ServerError(const std::string& what_arg );
            ServerError(const char* what_arg );
    };

    void checkError(const json& response);
    json parseResponse(const std::string& response);
}

#endif // ODOO_EXCEPTION_H