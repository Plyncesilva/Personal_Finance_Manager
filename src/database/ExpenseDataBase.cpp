#include <ExpenseDataBase.hh>

const std::string ExpenseDataBase::USERNAME = "admin"; // TODO this must be hidden somehow
const std::string ExpenseDataBase::PASSWORD = "admin123"; // TODO this must be hidden somehow
const std::string ExpenseDataBase::DB_URL = "couchbase://localhost"; // TODO this must be hidden somehow
const std::string ExpenseDataBase::BUCKET_NAME = "expenses"; // TODO this must be hidden somehow
const std::string ExpenseDataBase::SCOPE_NAME = "records"; // TODO this must be hidden somehow
const std::string ExpenseDataBase::COLLECTION_NAME = "global"; // TODO this must be hidden somehow

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

void ExpenseDataBase::query_setup(lcb_INSTANCE** instance) const{

    lcb_CREATEOPTS *create_options = nullptr;
    check(lcb_createopts_create(&create_options, LCB_TYPE_BUCKET), "build options object for lcb_create");
    check(lcb_createopts_credentials(create_options, USERNAME.c_str(), USERNAME.size(), PASSWORD.c_str(), PASSWORD.size()), "assign credentials");
    check(lcb_createopts_connstr(create_options, DB_URL.c_str(), DB_URL.size()), "assign connection string");
    check(lcb_createopts_bucket(create_options, BUCKET_NAME.c_str(), BUCKET_NAME.size()), "assign bucket name");

    check(lcb_create(instance, create_options), "create lcb_INSTANCE");
    check(lcb_createopts_destroy(create_options), "destroy options object");
    check(lcb_connect(*instance), "schedule connection");
    check(lcb_wait(*instance, LCB_WAIT_DEFAULT), "wait for connection");
    check(lcb_get_bootstrap_status(*instance), "check bootstrap status");

}

std::vector<nlohmann::json> ExpenseDataBase::query(std::string statement) const {
    Rows result{};
    lcb_CMDQUERY *cmd = nullptr;

    check(lcb_cmdquery_create(&cmd), "create QUERY command");
    check(lcb_cmdquery_statement(cmd, statement.c_str(), statement.size()), "assign statement for QUERY command");
    check(lcb_cmdquery_callback(cmd, query_callback), "assign callback for QUERY command");
    check(lcb_query(instance, &result, cmd), "schedule QUERY command");
    check(lcb_cmdquery_destroy(cmd), "destroy QUERY command");
    lcb_wait(instance, LCB_WAIT_DEFAULT);

    std::vector<nlohmann::json> out;

    for (const auto &row : result.rows) {
        out.push_back(nlohmann::json::parse(row));
    }

    return out;
}

int ExpenseDataBase::max_uuid() const {

    const std::string statement = "SELECT max(uuid) FROM " + get_collection_name();

    std::vector<nlohmann::json> j = query(statement);

    if ((j[0]).at("$1").is_null()){
        return -1;
    }

    return (j[0]).at("$1");
}

std::string ExpenseDataBase::get_collection_name() const {
    return BUCKET_NAME + "." + SCOPE_NAME + "." + COLLECTION_NAME;
}

std::shared_ptr<couchbase::bucket> ExpenseDataBase::get_bucket(){
    return cluster->bucket(BUCKET_NAME);
}

std::shared_ptr<couchbase::collection> ExpenseDataBase::get_default_collection(){
    return get_bucket()->default_collection();
}

std::shared_ptr<couchbase::collection> ExpenseDataBase::get_collection(std::string cname){
    return get_bucket()->collection(cname); 
}

ExpenseDataBase::ExpenseDataBase () {
    couchbase::transactions::set_transactions_log_level(couchbase::log_level::OFF);
    cluster = std::make_shared<couchbase::cluster>(DB_URL, USERNAME, PASSWORD);
    
    bucket = cluster->bucket(BUCKET_NAME);
    
    collection = bucket->collection(SCOPE_NAME + "." + COLLECTION_NAME);
    
    couchbase::transactions::transaction_config c;
    transactions = std::make_shared<couchbase::transactions::transactions>(*cluster, c);

    query_setup(&instance);
}

ExpenseDataBase::~ExpenseDataBase () {
    lcb_destroy(instance);
}

void ExpenseDataBase::add_expense(Expense e){
    
    try {
        transactions->run([&](couchbase::transactions::attempt_context& ctx) {
            ctx.insert(collection, e.get_uuid_str(), e.to_json());
            ctx.commit();
        });
    } catch (couchbase::transactions::transaction_failed& e){
        std::cerr << "Transaction did not reach commit point: " << e.what() << "\n";
    };
}

void ExpenseDataBase::rm_expense(std::string uuid){

    try {
        transactions->run([&](couchbase::transactions::attempt_context& ctx) {
    
            auto doc = ctx.get_optional(collection, uuid);

            if (doc){
                ctx.remove(collection, doc.value());
                ctx.commit();
            }
        });
    } catch (couchbase::transactions::transaction_failed& e) {
        std::cerr << "Transaction did not reach commit point: " << e.what() << "\n";
    }
}

void ExpenseDataBase::add_expense_collection(ExpenseCollection ec){
    for (Expense e : ec){
        add_expense(e);
    }
}
