#include "model.h"
#include <sstream>
#include <iostream>

namespace Odoo {

    Model::Model(
        const OdooRPC& rpc,
        const std::string& name,
        const Ids& ids
    ): _rpc(rpc), _name(name), _ids(ids)
    {}

    std::ostream& Model::writeToStream(std::ostream& stream) const {
        stream << _name + '(';
        if(!_ids.empty())
            stream << _ids[0];
        for(size_t index = 1; index < _ids.size(); ++index) {
            stream << ", ";
            stream << _ids[index];
        }
        stream << ')';
        return stream;
    }

    Model Model::browse(const Ids& ids) const {
        Model tmp = Model(*this);
        tmp._ids = ids;
        return tmp;
    }

    Model Model::create(const json& values) const {
        json response = json::parse(_rpc.raw_query(
            _name,
            "create",
            {
                values.dump()
            }
        ));
        return Model(_rpc, _name, {response["result"].get<size_t>()});
    }
}

std::ostream& operator<<(std::ostream& stream, const Odoo::Model& model) {
    return model.writeToStream(stream);
}

namespace std {
    string to_string(const Odoo::Model& model) {
        stringstream stream;
        model.writeToStream(stream);
        return stream.str();
    }
}