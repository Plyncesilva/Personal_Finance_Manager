#include <iostream>
#include <Expense.hh>
#include <ExpenseCollection.hh>
#include <ExpenseManager.hh>
#include <Command.hh>
#include <ListCommand.hh>
#include <CommandInvoker.hh>
#include <ExpenseDataBase.hh>
#include <gtkmm-3.0/gtkmm/application.h>
#include <gtkmm/main.h>
#include <ExpenseRegister.hh>

int main () {

    auto app = Gtk::Application::create("org.gtkmm.example");

    ExpenseRegister er;
    return app->run(er);
}