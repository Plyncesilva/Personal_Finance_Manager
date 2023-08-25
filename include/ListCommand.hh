#pragma once

#include <Command.hh>
#include <ExpenseManager.hh>

class ListCommand : public Command {
    private:
        ExpenseManager &em;

    public:
        ListCommand (ExpenseManager &em);
        void execute() const;
};