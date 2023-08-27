#include <iostream>
#include <Expense.hh>
#include <ExpenseCollection.hh>
#include <ExpenseManager.hh>
#include <Command.hh>
#include <ListCommand.hh>
#include <CommandInvoker.hh>
#include <ExpenseDataBase.hh>

int main () {

    ExpenseDataBase eb("expenses");

    Expense e1(200.0, ExpenseType("sex"), "fook yaaaaa"); 
    Expense e2(213.0, ExpenseType("food"), "yaaaaa buyyyyy"); 
    Expense e3(643.0, ExpenseType("girl"), "dinner"); 
    Expense e4(2.0, ExpenseType("camping"), "sakjda"); 
    Expense e5(0.2332, ExpenseType("food"), "akdjd"); 
    Expense e6(2.0, ExpenseType("transport"), "yaaaaa buyyyyy"); 
    
    return 0;
}