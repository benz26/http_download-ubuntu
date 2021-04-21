#ifndef _DOWNLOAD_CONN_
#define _DOWNLOAD_CONN_

//Download_conn.
class Download_conn {
public:
    static Download_conn* get_instance(void)
    {
        static Download_conn instance;
        return &instance;
    }

    //External call.
    virtual int   init(const std::string& download_url);
    virtual void  fini();
    virtual long  get_content_length();
    virtual void* new_conn(void* user_data);
    virtual int   conn_perform(void* conn,const std::string& range);

protected:
    std::string m_url;

    Download_conn():m_url("")
    {
    };

    ~Download_conn()
    {
    };
};

#endif
