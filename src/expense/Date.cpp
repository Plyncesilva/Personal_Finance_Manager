#include <Date.hh>

Date::Date() : date(time(0)) {}

Date::Date(time_t date) : date(date) {}

Date::Date(std::string ctime) {
    date = time(0); // TODO: convert string to time_t
}

time_t Date::get_date() const{
    return date;
}

std::string Date::time_chars_to_string(char* chars) const{
    if (chars != nullptr){
        chars[strlen(chars)-1] = '\0'; // remove new line
        return std::string(chars);
    }
    
    return "Error";
}

std::string Date::get_ascitime() const{
    char * time_chars = asctime(localtime(&date));

    return time_chars_to_string(time_chars);
}

std::string Date::get_ctime() const{
    char * time_chars = ctime(&date);

    return time_chars_to_string(time_chars);
}

int Date::get_second() const{
    return localtime(&date)->tm_sec;
}

int Date::get_minute() const {
    return localtime(&date)->tm_min;
}

int Date::get_hour() const{
    return localtime(&date)->tm_hour;
}

int Date::get_day() const{
    return localtime(&date)->tm_mday;
}

int Date::get_moth() const{
    return localtime(&date)->tm_mon;
}

int Date::get_year() const {
    return localtime(&date)->tm_year;
}

int Date::get_weekday() const {
    return localtime(&date)->tm_wday;
}

int Date::get_yearday() const {
    return localtime(&date)->tm_yday;
}

bool Date::operator==(const Date &other) const{
    return difftime(get_date(), other.get_date()) == 0;
}

double Date::operator-(const Date &other) const{
    return difftime(get_date(), other.get_date());
}

std::ostream& operator<<(std::ostream &strm, Date &d){
    return strm << "Date: " << d.get_ctime();  
}