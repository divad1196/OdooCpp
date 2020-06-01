#include "model.h"
#include <sstream>
#include <iostream>
#include "exception.h"

namespace Odoo {

    Model::Model(
        const SharedOdooRPC& rpc,
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
        json response = parseResponse(_rpc->raw_query(
            _name,
            "create",
            {
                values.dump()
            }
        ));
        checkError(response);
        return Model(_rpc, _name, {response.get<size_t>()});
    }


    void Model::write(const json& values) const {
        parseResponse(_rpc->raw_query(
            _name,
            "write",
            {
                json(_ids).dump(),
                values.dump()
            }
        ));
    }

    void Model::unlink() const {
        parseResponse(_rpc->raw_query(
            _name,
            "unlink",
            {
                json(_ids).dump(),
            }
        ));
    }

    Model Model::exists() const {
        json response = parseResponse(_rpc->raw_query(
            _name,
            "exists",
            {
                json(_ids).dump(),
            }
        ));
        return Model(_rpc, _name, response.get<Ids>());
    }

    Model Model::search(
        const char* domain,
        size_t offset,
        size_t limit,
        const std::string& order
    ) const {
        return search(std::string(domain), offset, limit, order);
    }
    
    Model Model::search(
        const std::string& domain,
        size_t offset,
        size_t limit,
        const std::string& order
    ) const {
        json response = parseResponse(_rpc->raw_query(
            _name,
            "search",
            {
                domain,
                std::to_string(offset),
                limit == (size_t)-1 ? "null" : std::to_string(limit),
                json(order).dump()
            }
        ));
        return Model(_rpc, _name, response.get<Ids>());
    }

    Model Model::search(
        const json& domain,
        size_t offset,
        size_t limit,
        const std::string& order
    ) const {
        return search(domain.dump(), offset, limit, order);
    }

    json Model::search_read(
        const std::string& domain,
        const std::vector<std::string>& fields,
        size_t offset,
        size_t limit,
        const std::string& order
    ) const {
        return parseResponse(_rpc->raw_query(
            _name,
            "search_read",
            {
                domain,
                json(fields).dump(),
                std::to_string(offset),
                limit == (size_t)-1 ? "null" : std::to_string(limit),
                json(order).dump()
            }
        ));
    }

    json Model::read(const std::vector<std::string>& fields, bool load) const {
        return parseResponse(_rpc->raw_query(
            _name,
            "read",
            {
                json(_ids).dump(),
                json(fields).dump(),
                load ? R"("_classic_read")" : "null"
            }
        ));
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