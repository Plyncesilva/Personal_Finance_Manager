#pragma once

#include <iostream>
#include <ctime>
#include <cstring>

class Date {

    time_t date;

    private:
        time_t get_date() const;
        std::string time_chars_to_string(char * chars) const;

    public:
        Date();
        Date (time_t date);

        std::string get_ascitime() const;
        std::string get_ctime() const;

        int get_second() const;
        int get_minute() const;
        int get_hour() const;
        int get_day() const;
        int get_moth() const;
        int get_year() const;
        int get_weekday() const;
        int get_yearday() const;

        bool operator==(const Date &other) const;
        double operator-(const Date &other) const;
        friend std::ostream& operator<<(std::ostream &strm, Date &d);
};