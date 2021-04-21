#ifndef _DOWNLOAD_FILE_
#define _DOWNLOAD_FILE_

//Download_file.
template<typename T>
class Download_file {
public:
    static Download_file* get_instance()
    {
        static Download_file instance;
        return &instance;
    }

    //External call.
    int init(const std::string& file_path, T* pconn);
    int fini();
    int get_filelength(void);

    //Internal call.
    int get_filelength_conn(T* pconn);
    int file_write(void* ptr, int size, int nmemb, void* pdata);

private:
    long    m_filelength;
    int     m_fd;             //Fd of open file.
    int     m_downloaded;     //Downloaded bytes.
    int     m_percent;        //Download progress percent.

    Download_file() :m_filelength(0), m_fd(0), m_downloaded(0), m_percent(0)
    {

    };

    ~Download_file()
    {
    };
};

#include "download_conn.h"
#include "download_conn_http.h"

//Instantiated
template class Download_file<Download_conn_http>;
#endif
