#include <iostream>
#include <Expense.hh>
#include <ExpenseCollection.hh>
#include <ExpenseManager.hh>
#include <Command.hh>
#include <ListCommand.hh>
#include <CommandInvoker.hh>

int main () {
    ExpenseManager em;
    
    em.add_expense(Expense(1, ExpenseType("Food")));
    em.add_expense(Expense(2, ExpenseType("Transportation")));
    em.add_expense(Expense(3));
    em.add_expense(Expense(4));
    em.add_expense(Expense(5));

    CommandInvoker ci;
    ci.set_command(new ListCommand(em));

    ci.execute_command();

    return 0;
}