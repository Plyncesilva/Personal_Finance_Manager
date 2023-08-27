#include <Date.hh>

const std::string Date::SECOND = "second";
const std::string Date::MINUTE = "minute";
const std::string Date::HOUR = "hour";
const std::string Date::DAY = "day";
const std::string Date::MONTH = "month";
const std::string Date::YEAR = "year";
const std::string Date::WEEKDAY = "weekday";
const std::string Date::YEAR_DAY = "year_day";

Date::Date() : date(time(0)) {}

Date::Date(time_t date) : date(date) {}

Date::Date(nlohmann::json j) {
    tm t;

    j.at(SECOND).get_to(t.tm_sec);
    j.at(MINUTE).get_to(t.tm_min);
    j.at(HOUR).get_to(t.tm_hour);
    j.at(DAY).get_to(t.tm_mday);
    j.at(MONTH).get_to(t.tm_mon);
    j.at(YEAR).get_to(t.tm_year);
    j.at(WEEKDAY).get_to(t.tm_wday);
    j.at(YEAR_DAY).get_to(t.tm_yday);

    date = mktime(&t);
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

int Date::get_month() const{
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

nlohmann::json Date::to_json() const {
    return nlohmann::json {
        {SECOND, get_second()},
        {MINUTE, get_minute()},
        {HOUR, get_hour()},
        {DAY, get_day()},
        {MONTH, get_month()},
        {YEAR, get_year()},
        {WEEKDAY, get_weekday()},
        {YEAR_DAY, get_yearday()},
    };
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