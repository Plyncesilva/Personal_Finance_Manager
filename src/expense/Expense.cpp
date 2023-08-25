#include <Expense.hh>

Expense::Expense(double value, ExpenseType type, std::string description, Date date) : value(value), type(type), description(description), date(date){
    // TODO: default expense type not working

}

double Expense::get_value() const {return value;}
void Expense::set_value(double value){value = value;}
ExpenseType Expense::get_expense_type() const {return type;}
void Expense::set_expense_type(ExpenseType type){type = type;}
Date Expense::get_date() const {return date;}
void Expense::set_date(Date date){date = date;}
std::string Expense::get_description() const {return description;}
void Expense::set_description(std::string description){description = description;}

std::string Expense::to_string(){
    // TODO
    std::ostringstream oss;
    oss << *(this);

    return oss.str();
}

bool Expense::operator!=(const Expense& other) const{
    return !(get_value() == other.get_value() && get_expense_type() == other.get_expense_type() && get_date() == other.get_date() && get_description() == other.get_description());
}


bool Expense::operator==(const Expense &other) const {
    return get_value() == other.get_value() && get_expense_type() == other.get_expense_type() && get_date() == other.get_date() && get_description() == other.get_description();
}

std::ostream& operator<<(std::ostream &strm, Expense &e){
    return strm << "#########\n\nExpense\n\nValue: " << e.value << "\n" << e.type << "\n" << e.date << "\nDescription: " << e.description << "\n\n#########";
}