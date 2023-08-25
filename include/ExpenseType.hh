#pragma once

class ExpenseType {

    std::string name;

    public:
        static const std::string notype;

        ExpenseType (std::string type_name);
        ExpenseType ();
        
        std::string get_name() const;
        void set_name(std::string name);
        
        bool operator==(const ExpenseType &other) const;
        friend std::ostream& operator<<(std::ostream &strm, const ExpenseType &t);
};