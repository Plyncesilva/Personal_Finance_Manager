#pragma once

#include <iostream>
#include <ExpenseType.hh>
#include <Date.hh>
#include <sstream>
#include <couchbase/transactions.hxx>

class Expense {
    
    private:

        static const std::string UUID;
        static const std::string VALUE;
        static const std::string TYPE;
        static const std::string DESCRIPTION;
        static const std::string DATE;

        static int GLOBAL_UUID;
        int uuid;
        double value;
        ExpenseType type;
        Date date;
        std::string description;


    public:
        Expense (double value, ExpenseType type = ExpenseType(), std::string description = "", Date date = Date());
        Expense (nlohmann::json j);

        friend class ExpenseManager;

        int get_uuid() const;
        std::string get_uuid_str() const;
        double get_value() const;
        void set_value(double value);
        ExpenseType get_expense_type() const;
        void set_expense_type(ExpenseType type);
        Date get_date() const;
        void set_date(Date date);
        std::string get_description() const;
        void set_description(std::string description);    
        std::string to_string();

        nlohmann::json to_json() const;
        

        bool operator!=(const Expense& other) const;
        bool operator==(const Expense& other) const;
        friend std::ostream& operator<<(std::ostream &strm, Expense &e);

};