#include <Expense.hh>

const std::string Expense::UUID = "uuid";
const std::string Expense::VALUE = "value";
const std::string Expense::TYPE = "type";
const std::string Expense::DESCRIPTION = "description";
const std::string Expense::DATE = "date";

int Expense::GLOBAL_UUID = 0;

Expense::Expense(double value, ExpenseType type, std::string description, Date date) : value(value), type(type), description(description), date(date){
    uuid = GLOBAL_UUID++;
}

Expense::Expense (nlohmann::json j){
    j.at(UUID).get_to(uuid);
    j.at(VALUE).get_to(value);
    type = ExpenseType(j.at(TYPE));
    j.at(DESCRIPTION).get_to(description);
    date = Date(j.at(DATE));
}

int Expense::get_uuid() const {return uuid;}
std::string Expense::get_uuid_str() const {return std::to_string(uuid);}
double Expense::get_value() const {return value;}
void Expense::set_value(double value){value = value;}
ExpenseType Expense::get_expense_type() const {return type;}
void Expense::set_expense_type(ExpenseType type){type = type;}
Date Expense::get_date() const {return date;}
void Expense::set_date(Date date){date = date;}
std::string Expense::get_description() const {return description;}
void Expense::set_description(std::string description){description = description;}

std::string Expense::to_string(){
    std::ostringstream oss;
    oss << *(this);

    return oss.str();
}

nlohmann::json Expense::to_json() const {
    return nlohmann::json {
        { UUID, uuid},
        { VALUE, value },
        { TYPE, type.to_json() },
        { DESCRIPTION, description },
        { DATE, date.to_json() },    
    };
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