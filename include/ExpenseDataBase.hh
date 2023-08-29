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
#include <ExpenseCollection.hh>

class ExpenseDataBase {

    private:
        
        static const std::string USERNAME;
        static const std::string PASSWORD;
        static const std::string DB_URL;
        static const std::string BUCKET_NAME;
        static const std::string SCOPE_NAME;
        static const std::string COLLECTION_NAME;
        
        std::shared_ptr<couchbase::cluster> cluster;
        std::shared_ptr<couchbase::bucket> bucket;
        std::shared_ptr<couchbase::collection> collection;
        std::shared_ptr<couchbase::transactions::transactions> transactions;

        lcb_INSTANCE* instance;

        struct Rows {
            std::vector<std::string> rows{};
            std::string metadata{};
        };
        
        static void check(lcb_STATUS err, const char *msg);
        static void query_callback(lcb_INSTANCE *, int, const lcb_RESPQUERY *resp);

        void query_setup(lcb_INSTANCE**) const;

        std::shared_ptr<couchbase::bucket> get_bucket();
        std::shared_ptr<couchbase::collection> get_default_collection();
        std::shared_ptr<couchbase::collection> get_collection(std::string cname);
        std::string get_collection_name() const;

    public:

        ExpenseDataBase ();
        ~ExpenseDataBase ();

        void add_expense(Expense e);
        void add_expense_collection(ExpenseCollection ec);
        void rm_expense(std::string uuid);
        int max_uuid() const;
        std::vector<nlohmann::json> query(std::string statement) const;
};