#pragma once

#include <vector>
#include <Expense.hh>

class ExpenseCollection {

    private:
        std::vector<Expense> exps;

    public:
        class iterator {

            private:
                int pos;
                ExpenseCollection &expc;

            public:
                iterator (int pos, ExpenseCollection &expc);
                Expense operator*();
                iterator operator++(int);
                iterator operator++();
                bool operator==(const iterator &other);
                bool operator!=(const iterator &other);
        };

    public:

        int size() const;
        void add_expense(Expense exp);
        Expense get_expense(int pos);

        // Iterator pattern
        iterator begin();
        iterator end();

        // operators
        bool operator==(const ExpenseCollection& other) const;
};