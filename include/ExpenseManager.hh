#pragma once

#include <ExpenseCollection.hh>
#include <ExpenseDataBase.hh>
#include <iostream>

class ExpenseManager {
    private:
        ExpenseCollection all;
        ExpenseDataBase e_db;

    public:
        ExpenseManager ();
        void add_expense(Expense e);
        ExpenseCollection get_expense_collection() const;
        void set_expense_collection(ExpenseCollection ec);
        std::string get_expenses_astring();
        int ec_size() const;
};