#include "exception.h"

namespace Odoo {
        ServerError::ServerError(const std::string& what_arg ): std::runtime_error(what_arg) {

        }

        ServerError::ServerError(const char* what_arg ): std::runtime_error(what_arg) {

        }

    void checkError(const json& response) {
        if(response.find("error") != response.end()) {
            throw ServerError(response["error"]["data"]["message"].get<std::string>());
        }
    }

    json parseResponse(const std::string& response) {
        json parsed;
        try {
            parsed = json::parse(response);
        } catch(...) {
            throw ServerError(
                "Server Response is not json serializable, Request may be invalid"
            );
        }
        checkError(parsed);
        return parsed["result"];
    }
}