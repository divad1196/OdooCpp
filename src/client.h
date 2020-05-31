#ifndef ODOO_CLIENT_H
#define ODOO_CLIENT_H

#include "../external/OdooRPC/src/odoorpc.h"
#include "model.h"
#include <memory>

namespace Odoo {

    class Client {
        public:
            Client(
                const std::string& url,
                const std::string& database,
                const Credentials& credentials
            );

            Model model(const std::string& name, Ids ids = {}) const;
            Model env(const std::string& name) const;

        private:
            const SharedOdooRPC _rpc;
    };


}



#endif // ODOO_CLIENT_H
