#pragma once

#include <couchbase/transactions.hxx>

class ExpenseType {

    private:
        std::string name;
        static const std::string NAME;

    public:
        static const std::string notype;

        ExpenseType (std::string type_name);
        ExpenseType (const char* type_name);
        ExpenseType (nlohmann::json j);
        ExpenseType ();
        
        std::string get_name() const;
        void set_name(std::string name);
        
        nlohmann::json to_json() const;

        bool operator==(const ExpenseType &other) const;
        friend std::ostream& operator<<(std::ostream &strm, const ExpenseType &t);
};