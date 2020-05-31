#ifndef ODOO_MODEL_H
#define ODOO_MODEL_H

#include <ostream>
#include <string>
#include <memory>
#include <vector>
typedef std::vector<size_t> Ids;

#include "../external/OdooRPC/src/odoorpc.h"
#include "json.h"

using nlohmann::json;
typedef std::shared_ptr<OdooRPC> SharedOdooRPC;
namespace Odoo {

    class Model {
        public:
            Model(const SharedOdooRPC& rpc, const std::string& name, const Ids& ids = {});
            std::ostream& writeToStream(std::ostream& stream) const;
            Model browse(const Ids& ids) const;
            Model create(const json& values) const;
        private:
            const SharedOdooRPC _rpc;
            std::string _name;
            Ids _ids;
    };
}

std::ostream& operator<<(std::ostream& stream, const Odoo::Model& model);

namespace std {
    string to_string(const Odoo::Model& model);
}

#endif // ODOO_MODEL_H
