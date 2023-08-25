#include <ListCommand.hh>

ListCommand::ListCommand(ExpenseManager &em) : em(em) {}

void ListCommand::execute() const{
    std::cout << em.get_expenses_astring() << std::endl;
}