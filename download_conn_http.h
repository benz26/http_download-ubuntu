#ifndef _DOWNLOAD_CONN_HTTP_
#define _DOWNLOAD_CONN_HTTP_

//Download_conn_http.
class Download_conn_http :public Download_conn {
public:
    static Download_conn_http* get_instance(void)
    {
        static Download_conn_http instance;
        return &instance;
    }

    //External call.
    virtual int    init(const std::string& download_url);
    virtual void   fini();
    virtual long   get_content_length();
    virtual void*  new_conn(void* user_data);
    virtual int    conn_perform(void* conn, const std::string& range);

    //Internal call.
    static size_t  conn_write_fun(void* ptr, size_t size, size_t nmemb, void* pdata);
    int            global_init();

private:
    Download_conn_http()
    {
    };

    ~Download_conn_http()
    {
    };
};

#endif