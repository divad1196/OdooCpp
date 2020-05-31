#include "client.h"


namespace Odoo {

    Client::Client(
        const std::string& url,
        const std::string& database,
        const Credentials& credentials
    ): _rpc(url, database, credentials)
    {

    }

    Model Client::model(const std::string& name, Ids ids) const {
        return Model(_rpc, name, ids);
    }
    Model Client::env(const std::string& name) const {
        return Model(_rpc, name);
    }
}