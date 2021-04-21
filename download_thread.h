#ifndef _DOWNLOAD_THREAD_
#define _DOWNLOAD_THREAD_

//Control a thread to write.
struct thread_node
{
    int     start;        //The location where the thread started writing the file.
    int     end;          //Write the file to the end position.
    int     nBlock;       //The number of bytes the thread needs to write.
    pthread_t tid;
    void*   conn;
    int     tIndex;
    void*   pconn_class; //Address of derived class object.
};

//Download_thread.
template<typename T>
class Download_thread {
public:
    static Download_thread* get_instance()
    {
        static Download_thread instance;
        return &instance;
    }

    //External call.
    void init(const int thread_num);
    void fini();
    int  thread_handle(T* pconn_inst);

    //Internal call.
    static void* thread_work(void* param);

private:
    int  m_thread_num;             //Thread num.
    int  m_block_size;             //Most bytes num downloaded by each thread.
    bool m_remain_flag;            //Is (filelength/thread num) divided or not, true means that is not be divided.
    thread_node* m_pthread_nodes;  //Thread running data.

    Download_thread() :m_thread_num(10), m_block_size(0), m_remain_flag(false), m_pthread_nodes(NULL)
    {
    };

    ~Download_thread()
    {
    };
};

#include "download_conn.h"
#include "download_conn_http.h"

//Instantiated
template class Download_thread<Download_conn_http>;

#endif
