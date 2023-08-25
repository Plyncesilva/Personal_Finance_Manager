#pragma once

#include <iostream>
#include <ExpenseType.hh>
#include <Date.hh>
#include <sstream>

class Expense {

    double value;
    ExpenseType type;
    Date date;
    std::string description;

    public:
        Expense (double value, ExpenseType type = ExpenseType(), std::string description = "", Date date = Date());

        double get_value() const;
        void set_value(double value);
        ExpenseType get_expense_type() const;
        void set_expense_type(ExpenseType type);
        Date get_date() const;
        void set_date(Date date);
        std::string get_description() const;
        void set_description(std::string description);    
        std::string to_string();

        bool operator!=(const Expense& other) const;
        bool operator==(const Expense& other) const;
        friend std::ostream& operator<<(std::ostream &strm, Expense &e);
};