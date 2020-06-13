#include "model.h"
#include <sstream>
#include <iostream>
#include "exception.h"

// Private utilities prototype
void _removeDuplicate(Ids& ids);
namespace Odoo {

    Model::Model(
        const SharedOdooRPC& rpc,
        const std::string& name,
        const Ids& ids
    ): _rpc(rpc), _name(name), _ids(ids)
    {}

    // Utilities

    void Model::removeDuplicate() {
        _removeDuplicate(_ids);
    }

    // Operators

    Model& Model::operator+=(const Ids& ids) {
        _ids.reserve(_ids.size() + ids.size());
        for(auto id: ids) {
            _ids.push_back(id);
        }
        return *this;
    }

    Model& Model::operator-=(const Ids& ids) {
        size_t cursor = 0;
        bool not_found = true;
        for(size_t index = 0; index < _ids.size(); ++index) {
            for(size_t check_index = 0; check_index < ids.size() and not_found; ++check_index) {
                if(_ids[index] == ids[check_index])
                    not_found = false;
            }
            if(not_found) {
                _ids[cursor] = _ids[index];
                ++cursor;
            } else {
                not_found = true;
            }
        }
        _ids.resize(cursor);
        return *this;
    }

    Model& Model::operator|=(const Ids& ids) {
        *this += ids;
        removeDuplicate();
        return *this;
    }

    Model& Model::operator&=(const Ids& ids) {
        size_t cursor = 0;
        bool not_found = true;
        for(size_t index = 0; index < _ids.size(); ++index) {
            for(size_t check_index = 0; check_index < ids.size() and not_found; ++check_index) {
                if(_ids[index] == ids[check_index])
                    not_found = false;
            }
            if(!not_found) {
                _ids[cursor] = _ids[index];
                ++cursor;
                not_found = true;
            }
        }
        _ids.resize(cursor);
        return *this;
    }


    Model& Model::operator+=(const Model& model) {
        return *this += model._ids;
    }
    Model& Model::operator-=(const Model& model) {
        return *this -= model._ids;
    }
    Model& Model::operator|=(const Model& model) {
        return *this |= model._ids;
    }
    Model& Model::operator&=(const Model& model) {
        return *this &= model._ids;
    }


    Model::operator std::string() const {
        std::string res(_name + '(');
        if(!_ids.empty())
            res += std::to_string(_ids[0]);
        for(size_t index = 1; index < _ids.size(); ++index) {
            res += ", " + std::to_string(_ids[index]);
        }
        return res += ')';
    }


    Model operator+(Model model, const Ids& ids) {
        return model += ids;
    }
    Model operator-(Model model, const Ids& ids) {
        return model -= ids;
    }
    Model operator|(Model model, const Ids& ids) {
        return model |= ids;
    }
    Model operator&(Model model, const Ids& ids) {
        return model &= ids;
    }

    Model operator+(Model model, const Model& model2) {
        return model += model2;
    }
    Model operator-(Model model, const Model& model2) {
        return model -= model2;
    }
    Model operator|(Model model, const Model& model2) {
        return model |= model2;
    }
    Model operator&(Model model, const Model& model2) {
        return model &= model2;
    }

    // Highleve methods

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


void _removeDuplicate(Ids& ids) {
    size_t cursor = 0;
    bool not_found = true;
    for(size_t index = 0; index < ids.size(); ++index) {
        for(size_t check_index = 0; check_index < cursor and not_found; ++check_index) {
            if(ids[index] == ids[check_index])
                not_found = false;
        }
        if(not_found) {
            ids[cursor] = ids[index];
            ++cursor;
        } else {
            not_found = true;
        }
    }
    ids.resize(cursor);
}

std::ostream& operator<<(std::ostream& stream, const Odoo::Model& model) {
    return stream << std::string(model);
}

namespace std {
    string to_string(const Odoo::Model& model) {
        return std::string(model);
    }
}