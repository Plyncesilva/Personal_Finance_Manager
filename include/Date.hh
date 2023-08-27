#pragma once

#include <iostream>
#include <ctime>
#include <cstring>
#include <couchbase/transactions.hxx>

class Date {

    time_t date;

    private:
        time_t get_date() const;
        std::string time_chars_to_string(char * chars) const;

        static const std::string SECOND; 
        static const std::string MINUTE;
        static const std::string HOUR;
        static const std::string DAY;
        static const std::string MONTH;
        static const std::string YEAR;
        static const std::string WEEKDAY;
        static const std::string YEAR_DAY;
        

    public:
        Date();
        Date (time_t date);
        Date (nlohmann::json j);

        std::string get_ascitime() const;
        std::string get_ctime() const;

        int get_second() const;
        int get_minute() const;
        int get_hour() const;
        int get_day() const;
        int get_month() const;
        int get_year() const;
        int get_weekday() const;
        int get_yearday() const;

        nlohmann::json to_json() const;

        bool operator==(const Date &other) const;
        double operator-(const Date &other) const;
        friend std::ostream& operator<<(std::ostream &strm, Date &d);
};