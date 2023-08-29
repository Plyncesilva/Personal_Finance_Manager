#include <ExpenseManager.hh>

ExpenseManager::ExpenseManager(){
    Expense::GLOBAL_UUID = e_db.max_uuid() + 1;
}

void ExpenseManager::add_expense(Expense e){
    e_db.add_expense(e);
}

ExpenseCollection ExpenseManager::get_expense_collection() const{
    return all;
}

void ExpenseManager::set_expense_collection(ExpenseCollection ec){
    all = ec;
}

std::string ExpenseManager::get_expenses_astring() {
    std::string out;
    for (Expense e : all){
        out += e.to_string() + "\n";
    }
    return out;
}

int ExpenseManager::ec_size() const {
    return all.size();
}