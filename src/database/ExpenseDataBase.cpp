#include <ExpenseDataBase.hh>

const std::string ExpenseDataBase::db_user = "admin"; // TODO this must be hidden somehow
const std::string ExpenseDataBase::db_password = "admin123"; // TODO this must be hidden somehow
const std::string ExpenseDataBase::db_url = "couchbase://localhost"; // TODO this must be hidden somehow

void ExpenseDataBase::check(lcb_STATUS err, const char *msg) {
    if (err != LCB_SUCCESS) {
        std::cerr << "[ERROR] " << msg << ": " << lcb_strerror_short(err) << "\n";
        exit(EXIT_FAILURE);
    }
}

void ExpenseDataBase::query_callback(lcb_INSTANCE *, int, const lcb_RESPQUERY *resp){
    lcb_STATUS status = lcb_respquery_status(resp);
    if (status != LCB_SUCCESS) {
        const lcb_QUERY_ERROR_CONTEXT *ctx;
        lcb_respquery_error_context(resp, &ctx);

        uint32_t err_code = 0;
        lcb_errctx_query_first_error_code(ctx, &err_code);

        const char *err_msg = nullptr;
        size_t err_msg_len = 0;
        lcb_errctx_query_first_error_message(ctx, &err_msg, &err_msg_len);
        std::string error_message{};
        if (err_msg_len > 0) {
            error_message.assign(err_msg, err_msg_len);
        }

        std::cerr << "[ERROR] failed to execute query " << lcb_strerror_short(status) << ". "
                  << error_message << " ("
                  << err_code << ")\n";
        return;
    }

    const char *buf = nullptr;
    std::size_t buf_len = 0;
    lcb_respquery_row(resp, &buf, &buf_len);
    if (buf_len > 0) {
        Rows *result = nullptr;
        lcb_respquery_cookie(resp, reinterpret_cast<void **>(&result));
        if (lcb_respquery_is_final(resp)) {
            result->metadata.assign(buf, buf_len);
        } else {
            result->rows.emplace_back(std::string(buf, buf_len));
        }
    }
}

void ExpenseDataBase::query_city(lcb_INSTANCE *instance, const std::string &bucket_name, const std::string &city){
    Rows result{};

    // tag::placeholder[]
    std::string statement = "SELECT airportname, city, country FROM `" + bucket_name + R"(` WHERE type="airport" AND city=$1)";

    lcb_CMDQUERY *cmd = nullptr;
    check(lcb_cmdquery_create(&cmd), "create QUERY command");
    check(lcb_cmdquery_statement(cmd, statement.c_str(), statement.size()), "assign statement for QUERY command");
    std::string city_json = "\"" + city + "\""; // production code should use JSON encoding library
    check(lcb_cmdquery_positional_param(cmd, city_json.c_str(), city_json.size()), "add positional parameter for QUERY comand");
    // Enable using prepared (optimized) statements
    check(lcb_cmdquery_adhoc(cmd, false), "enable prepared statements for QUERY command");
    check(lcb_cmdquery_callback(cmd, query_callback), "assign callback for QUERY command");
    check(lcb_query(instance, &result, cmd), "schedule QUERY command");
    check(lcb_cmdquery_destroy(cmd), "destroy QUERY command");
    lcb_wait(instance, LCB_WAIT_DEFAULT);
    // end::placeholder[]

    std::cout << "\n--- Query returned " << result.rows.size() << " rows for " << city << std::endl;
    for (const auto &row : result.rows) {
        std::cout << row << "\n";
    }
}

void ExpenseDataBase::expense_to_json(nlohmann::json& j, const Expense &e){
    j = nlohmann::json{
        { "value", e.get_value() },
        { "type", e.get_expense_type().get_name() },
        { "description", e.get_description() },
        { "date", e.get_date().get_ctime() },
        { "uuid", e.get_uuid() },
    };
}

void ExpenseDataBase::expense_from_json(const nlohmann::json& j, Expense &e){
    j.at("value").get_to(e.value);
    // TODO: object conversions
}

void ExpenseDataBase::set_log_level(couchbase::log_level level){
    couchbase::transactions::set_transactions_log_level(level);
}

std::shared_ptr<couchbase::bucket> ExpenseDataBase::get_bucket(){
    return cluster->bucket(db_bucket);
}

std::shared_ptr<couchbase::collection> ExpenseDataBase::get_default_collection(){
    return get_bucket()->default_collection();
}

ExpenseDataBase::ExpenseDataBase (std::string bucket_name) : db_bucket(bucket_name) {
    set_log_level(couchbase::log_level::DEBUG);
    cluster = new couchbase::cluster(db_url, db_user, db_password);
}

ExpenseDataBase::ExpenseDataBase (){

}

ExpenseDataBase::~ExpenseDataBase(){
    delete cluster;
}

void ExpenseDataBase::test () {

    // testing couchbase

    std::string username{"admin"};
    std::string password{"admin123"};
    std::string connection_string{"couchbase://localhost"};
    std::string bucket_name{"expenses"};

    lcb_CREATEOPTS *create_options = nullptr;
    check(lcb_createopts_create(&create_options, LCB_TYPE_BUCKET), "build options object for lcb_create");
    check(lcb_createopts_credentials(create_options, username.c_str(), username.size(), password.c_str(), password.size()), "assign credentials");
    check(lcb_createopts_connstr(create_options, connection_string.c_str(), connection_string.size()), "assign connection string");
    check(lcb_createopts_bucket(create_options, bucket_name.c_str(), bucket_name.size()), "assign bucket name");

    lcb_INSTANCE *instance = nullptr;
    check(lcb_create(&instance, create_options), "create lcb_INSTANCE");
    check(lcb_createopts_destroy(create_options), "destroy options object");
    check(lcb_connect(instance), "schedule connection");
    check(lcb_wait(instance, LCB_WAIT_DEFAULT), "wait for connection");
    check(lcb_get_bootstrap_status(instance), "check bootstrap status");

    query_city(instance, bucket_name, "Calais");
    // query_city(instance, bucket_name, "Dallas");
    // query_city(instance, bucket_name, "Los Angeles");

    lcb_destroy(instance);
}

void ExpenseDataBase::add_doc_test() {

    // #tag::config_trace[]
    // Set logging level to Trace
    couchbase::transactions::set_transactions_log_level(couchbase::log_level::DEBUG);
    // #end::config_trace[]

    // #tag::init[]
    // Initialize the Couchbase cluster
    couchbase::cluster cluster("couchbase://localhost", "admin", "admin123");
    auto bucket = cluster.bucket("expenses");
    auto collection = bucket->default_collection();

    // Create the single Transactions object
    couchbase::transactions::transactions transactions(cluster, {});
    // #end::init[]

    {
        // #tag::config[]
        couchbase::transactions::transaction_config configuration;
        configuration.durability_level(couchbase::transactions::durability_level::PERSIST_TO_MAJORITY);
        couchbase::transactions::transactions transactions(cluster, configuration);
        // #end::config[]
    }

    {
        // #tag::config-expiration[]
        couchbase::transactions::transaction_config configuration;
        configuration.expiration_time(std::chrono::seconds(120));
        couchbase::transactions::transactions transactions(cluster, configuration);
        // #end::config-expiration[]
    }

    {
        // #tag::config-cleanup[]
        couchbase::transactions::transaction_config configuration;
        configuration.cleanup_client_attempts(false);
        configuration.cleanup_lost_attempts(false);
        configuration.cleanup_window(std::chrono::seconds(120));
        couchbase::transactions::transactions transactions(cluster, configuration);
        // #end::config-cleanup[]
    }

      {
        // #tag::create[]
        try {
            transactions.run([&](couchbase::transactions::attempt_context& ctx) {
                // 'ctx' permits getting, inserting, removing and replacing documents,
                // along with committing and rolling back the transaction

                // ... Your transaction logic here ...
                nlohmann::json j;
                ExpenseDataBase::expense_to_json(j, Expense(500.0));
                ctx.insert(collection, "test-doc", j);

                // This call is optional -- if you leave it off,
                // the transaction will be committed anyway.
                ctx.commit();
            });
        } catch (couchbase::transactions::transaction_failed& e) {
            std::cerr << "Transaction did not reach commit point: " << e.what() << "\n";
        }
        // #end::create[]
    }

    // {
    //     // #tag::examples[]
    //     try {
    //         transactions.run([&](couchbase::transactions::attempt_context& ctx) {
    //             // Inserting a doc:
    //             ctx.insert(collection, "doc-a", nlohmann::json({}));

    //             // Getting documents:
    //             // Use ctx.get_optional() if the document may or may not exist
    //             auto doc_opt = ctx.get_optional(collection, "doc-a");
    //             if (doc_opt) {
    //                 couchbase::transactions::transaction_get_result& doc = doc_opt.value();
    //             }

    //             // Use ctx.get if the document should exist, and the transaction
    //             // will fail if it does not
    //             couchbase::transactions::transaction_get_result doc_a = ctx.get(collection, "doc-a");

    //             // Replacing a doc:
    //             couchbase::transactions::transaction_get_result doc_b = ctx.get(collection, "doc-b");
    //             nlohmann::json content = doc_b.content<nlohmann::json>();
    //             content["transactions"] = "are awesome";
    //             ctx.replace(collection, doc_b, content);

    //             // Removing a doc:
    //             couchbase::transactions::transaction_get_result doc_c = ctx.get(collection, "doc-c");
    //             ctx.remove(collection, doc_c);

    //             ctx.commit();
    //         });
    //     } catch (couchbase::transactions::transaction_failed& e) {
    //         std::cerr << "Transaction did not reach commit point: " << e.what() << "\n";
    //     }
    //     // #end::examples[]
    // }

    // {
    //     // tag::insert[]
    //     transactions.run([&](couchbase::transactions::attempt_context& ctx) {
    //         std::string id = "doc_id";
    //         nlohmann::json value{
    //             { "foo", "bar" },
    //         };
    //         ctx.insert(collection, id, value);
    //     });
    //     // end::insert[]
    // }

    // {
    //     // #tag::get[]
    //     transactions.run([&](couchbase::transactions::attempt_context& ctx) {
    //         std::string id = "doc-a";
    //         auto doc_opt = ctx.get_optional(collection, id);
    //         if (doc_opt) {
    //             couchbase::transactions::transaction_get_result& doc = doc_opt.value();
    //         }
    //     });
    //     // #end::get[]
    // }

    // {
    //     // #tag::getReadOwnWrites[]
    //     transactions.run([&](couchbase::transactions::attempt_context& ctx) {
    //         std::string id = "doc_id";
    //         nlohmann::json value{
    //             { "foo", "bar" },
    //         };
    //         ctx.insert(collection, id, value);
    //         // document must be accessible
    //         couchbase::transactions::transaction_get_result doc = ctx.get(collection, id);
    //     });
    //     // #end::getReadOwnWrites[]
    // }

    // {
    //     // tag::replace[]
    //     transactions.run([&](couchbase::transactions::attempt_context& ctx) {
    //         std::string id = "doc-a";
    //         couchbase::transactions::transaction_get_result doc = ctx.get(collection, id);
    //         nlohmann::json content = doc.content<nlohmann::json>();
    //         content["transactions"] = "are awesome";
    //         ctx.replace(collection, doc, content);
    //     });
    //     // end::replace[]
    // }

    // {
    //     // #tag::remove[]
    //     transactions.run([&](couchbase::transactions::attempt_context& ctx) {
    //         std::string id = "doc-a";
    //         auto doc_opt = ctx.get_optional(collection, id);
    //         if (doc_opt) {
    //             ctx.remove(collection, doc_opt.value());
    //         }
    //     });
    //     // #end::remove[]
    // }

    // {
    //     int cost_of_item = 10;
    //     // #tag::rollback[]
    //     transactions.run([&](couchbase::transactions::attempt_context& ctx) {
    //         couchbase::transactions::transaction_get_result customer = ctx.get(collection, "customer-name");

    //         auto content = customer.content<nlohmann::json>();
    //         int balance = content["balance"].get<int>();
    //         if (balance < cost_of_item) {
    //             ctx.rollback();
    //         }
    //         // else continue transaction
    //     });
    //     // #end::rollback[]
    // }

    // {
    //     // #tag::max_bucket_instances[]
    //     couchbase::cluster c("couchbase://localhost", "transactor", "mypass", cluster_options().max_bucket_instances(10));
    //     auto coll = c.bucket("transact")->default_collection();
    //     // #end::max_bucket_instances[]

    //     // #tag::threads[]
    //     std::list<std::thread> threads;
    //     std::atomic<int> counter {0};
    //     for (int i=0; i<10; i++) {
    //       threads.emplace_back([&]() {
    //         transactions.run([&](couchbase::transactions::attempt_context& ctx) {
    //           std::string id = "doc_a";
    //           auto doc = ctx.get(coll, id);
    //           auto doc_content = doc.value();
    //           doc_content["counter"] = ++counter;
    //           ctx.replace(coll, doc, doc_content);
    //         });
    //       });
    //     }
    //     for (auto& t: threads) {
    //       if (t.joinable()) {
    //         t.join();
    //       }
    //     }
    //     //#end::threads[]
    // }
}

void ExpenseDataBase::add_expense(Expense e){
    
    couchbase::transactions::transactions transactions(*cluster, {});
    
    try {
        transactions.run([&](couchbase::transactions::attempt_context& ctx) {

            ctx.insert(get_default_collection(), std::to_string(e.uuid), e.to_json());
            ctx.commit();

        });
    } catch (couchbase::transactions::transaction_failed& e) {
        std::cerr << "Transaction did not reach commit point: " << e.what() << "\n";
    }
}

void ExpenseDataBase::rm_expense(int uuid){

    couchbase::transactions::transactions t(*cluster, {});

    try {
        t.run([&](couchbase::transactions::attempt_context& ctx) {
    
            auto doc = ctx.get_optional(get_default_collection(), std::to_string(uuid));
    
            if (doc){
                ctx.remove(get_default_collection(), doc.value());
                ctx.commit();
            }
        });
    } catch (couchbase::transactions::transaction_failed& e) {
        std::cerr << "Transaction did not reach commit point: " << e.what() << "\n";
    }
}