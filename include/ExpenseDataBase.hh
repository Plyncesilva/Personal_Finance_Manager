#pragma once

#include <libcouchbase/couchbase.h>
#include <couchbase/client/cluster.hxx>
#include <couchbase/transactions.hxx>
#include <vector>
#include <string>
#include <iostream>
#include <Expense.hh>
#include <Date.hh>
#include <thread>
#include <list>

class ExpenseDataBase {

    private:
        
        struct Rows {
            std::vector<std::string> rows{};
            std::string metadata{};
        };
        
        static void check(lcb_STATUS err, const char *msg);
        static void query_callback(lcb_INSTANCE *, int, const lcb_RESPQUERY *resp);
        static void query_city(lcb_INSTANCE *instance, const std::string &bucket_name, const std::string &city);
        static void expense_to_json(nlohmann::json& j, const Expense &e);
        static void expense_from_json(const nlohmann::json& j, Expense &e);

    public:

        void test();
        void add_doc_test();

};