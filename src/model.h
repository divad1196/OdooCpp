#ifndef ODOO_MODEL_H
#define ODOO_MODEL_H

#include <ostream>
#include <string>
#include <memory>
#include <vector>
typedef std::vector<size_t> Ids;

#include "../external/OdooRPC/src/odoorpc.h"
#include "json.h"

typedef std::shared_ptr<OdooRPC> SharedOdooRPC;
namespace Odoo {

    class Model {
        public:
            Model(const SharedOdooRPC& rpc, const std::string& name, const Ids& ids = {});

            // Utilities

            /**
             * @brief remove duplicate ids in the recordset
             */
            void removeDuplicate();

            // Operators

            /**
             * @param ids ids to add to the recordset
             * @brief add ids to the recordset without check for duplicate
             */
            Model& operator+=(const Ids& ids);

            /**
             * @param ids ids to remove from the recordset
             * @brief remove all occurence of id in ids from the recordset
             */
            Model& operator-=(const Ids& ids);

            /**
             * @param ids ids to add to the recordset
             * @brief add ids to the recordset and remove all duplicate
             */
            Model& operator|=(const Ids& ids);

            /**
             * @param ids ids to keep in the recordset
             * @brief remove all ids from recordset which are not in ids
             */
            Model& operator&=(const Ids& ids);

            Model& operator+=(const Model& model);
            Model& operator-=(const Model& model);
            Model& operator|=(const Model& model);
            Model& operator&=(const Model& model);

            operator std::string() const;

            // Highleve methods

            /**
             * @param ids ids for the new recordset
             * @brief Create a new recordset with the given ids
             */
            Model browse(const Ids& ids) const;

            /**
             * @param values json values as for Odoo create method
             * @brief Create in Odoo a new record with the given values
             * @return the record created
             */
            Model create(const json& values) const;

            /**
             * @param values json values as for Odoo write method
             * @brief Update in Odoo the records from the recordset
             */
            void write(const json& values) const;

            /**
             * @brief Delete in Odoo the records from the recordset
             */
            void unlink() const;

            /**
             * @brief Return a recordset of the ids that exists in Odoo
             */
            Model exists() const;

            /**
             * @brief Return a recordset matching the search
             */
            Model search(const char* domain, size_t offset=0, size_t limit=-1, const std::string& order="") const;
            Model search(const std::string& domain, size_t offset=0, size_t limit=-1, const std::string& order="") const;
            Model search(const json& domain, size_t offset=0, size_t limit=-1, const std::string& order="") const;

            /**
             * @brief Return json of the requested values for the records that match the search
             */
            json search_read(
                const std::string& domain,
                const std::vector<std::string>& fields,
                size_t offset=0,
                size_t limit=-1,
                const std::string& order=""
            ) const;

            /**
             * @brief Return json of the requested values for the records in the recordset
             */
            json read(const std::vector<std::string>& fields, bool load=false) const;
            
        private:
            SharedOdooRPC _rpc;
            std::string _name;
            Ids _ids;
    };

    Model operator+(Model model, const Ids& ids);
    Model operator-(Model model, const Ids& ids);
    Model operator|(Model model, const Ids& ids);
    Model operator&(Model model, const Ids& ids);

    Model operator+(Model model, const Model& model2);
    Model operator-(Model model, const Model& model2);
    Model operator|(Model model, const Model& model2);
    Model operator&(Model model, const Model& model2);
}

std::ostream& operator<<(std::ostream& stream, const Odoo::Model& model);

namespace std {
    string to_string(const Odoo::Model& model);
}

#endif // ODOO_MODEL_H
