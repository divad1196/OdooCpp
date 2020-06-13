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

            /**
             * @param name technical name of the model
             * @param ids ids of the recordset 
             * @return a recordset of the requested model
             */
            Model model(const std::string& name, Ids ids = {}) const;

            /**
             * @param name technical name of the model
             * @brief minimal alias for model methods.
             */
            Model env(const std::string& name) const;

        private:
            const SharedOdooRPC _rpc;
    };


}



#endif // ODOO_CLIENT_H
