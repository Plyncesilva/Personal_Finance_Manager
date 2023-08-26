#include <iostream>
#include <Expense.hh>
#include <ExpenseCollection.hh>
#include <ExpenseManager.hh>
#include <Command.hh>
#include <ListCommand.hh>
#include <CommandInvoker.hh>
#include <ExpenseDataBase.hh>

int main () {

    ExpenseDataBase eb;
    eb.add_doc_test();
    
    return 0;
}