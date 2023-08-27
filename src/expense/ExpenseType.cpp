#include <iostream>
#include <string>
#include <ExpenseType.hh>

const std::string ExpenseType::notype = "None";
const std::string ExpenseType::NAME = "name";

ExpenseType::ExpenseType(std::string type_name) : name(type_name) {}

ExpenseType::ExpenseType (const char* type_name) {
    name = std::string(type_name);
}


ExpenseType::ExpenseType(nlohmann::json j){
    j.at(NAME).get_to(name);
}

ExpenseType::ExpenseType(){
    name = ExpenseType::notype;
}

std::string ExpenseType::get_name() const{
    return name;
}

void ExpenseType::set_name(std::string name){
    name = name;
}

nlohmann::json ExpenseType::to_json() const {
    return nlohmann::json {
        {NAME, name},
    };
}

bool ExpenseType::operator==(const ExpenseType &other) const {
    return get_name() == other.get_name();
}

std::ostream& operator<<(std::ostream &strm, const ExpenseType &t){
    return strm << "Expense Type: " << t.get_name();
}