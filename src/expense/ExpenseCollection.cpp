#include <ExpenseCollection.hh>

int ExpenseCollection::size() const {
    return exps.size();
}

void ExpenseCollection::add_expense(Expense exp) {
    exps.push_back(exp);
}

ExpenseCollection::iterator::iterator (int pos, ExpenseCollection &expc) : pos(pos), expc(expc) {}

Expense ExpenseCollection::iterator::operator*(){
    return expc.get_expense(pos);
}

ExpenseCollection::iterator ExpenseCollection::iterator::operator++(int){
    return iterator(pos++, expc);
}

ExpenseCollection::iterator ExpenseCollection::iterator::operator++(){
    return iterator(pos++, expc);
}

bool ExpenseCollection::iterator::operator==(const iterator &other) {
    return pos == other.pos && expc == other.expc;
}

bool ExpenseCollection::iterator::operator!=(const iterator &other) {
    return !(pos == other.pos && expc == other.expc);
}

ExpenseCollection::iterator ExpenseCollection::begin(){
    return ExpenseCollection::iterator(0, *this);
}

ExpenseCollection::iterator ExpenseCollection::end(){
    return ExpenseCollection::iterator(size(), *this);
}

bool ExpenseCollection::operator==(const ExpenseCollection& other) const {
    if (size() != other.size()) return false;

    for (int i = 0; i < size(); i++){
        if (exps[i] != other.exps[i]){
            return false;
        }
    }
    return true;
}

Expense ExpenseCollection::get_expense(int pos){
    // TODO: check for invalid positions and exceptions
    return exps[pos];
}