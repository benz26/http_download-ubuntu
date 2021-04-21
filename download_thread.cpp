#include <iostream>
#include <pthread.h>
#include "download_utils.h"
#include "download_thread.h"
#include "download_file.h"

using namespace std;

template<typename T>
void Download_thread<T>::init(const int thread_num)
{
    int filelength = Download_file<T>::get_instance()->get_filelength();

    m_thread_num = thread_num;

    if (0 == filelength % thread_num)
    {
        //Be divided.
        m_remain_flag = false;

        //For example, 270/10, m_block_size is 27.
        m_block_size = filelength / thread_num;
    }
    else
    {
        //Is not divided.
        m_remain_flag = true;

        //For example, 257/10, m_block_size is 26.
        m_block_size = (filelength / thread_num) + 1;
    }

    //Thread ctrl.
    m_pthread_nodes = new thread_node[thread_num];

    return;
}

//For pthread_create(), callback.
template<typename T>
void* Download_thread<T>::thread_work(void* param)
{
    thread_node* pnode = (thread_node*)param;
    T* pconn_class = (T*)pnode->pconn_class;

    char tmp[32] = { 0 };
    sprintf(tmp, "%d-%d", pnode->start, pnode->end);
    string range = tmp;

    (void)pconn_class->conn_perform(pnode->conn, range);

    pthread_exit(NULL);

    return NULL;
}

//Called by main.
template<typename T>
int Download_thread<T>::thread_handle(T* pconn_inst)
{
    int ret = ERROR_TYPE_FAILED;
    pthread_attr_t attr;
    thread_node* pthread_nodes = m_pthread_nodes;

    //Initializeand set the thread to joinable.
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Create I threads.
    for (int i = 0; i < m_thread_num; i++)
    {
        //Fill in start and end fields.
        pthread_nodes[i].start = i * m_block_size;

        if ((m_remain_flag)
            && (i == m_thread_num - 1))
        {
            int filelength          = Download_file<T>::get_instance()->get_filelength();
            pthread_nodes[i].end    = filelength - 1;
            pthread_nodes[i].nBlock = filelength - i * m_block_size;
        }
        else
        {
            // 257byte,10 threads;0~25,end is 25.
            pthread_nodes[i].end    = (i + 1) * m_block_size - 1;
            pthread_nodes[i].nBlock = m_block_size;
        }

        //Fill in Conn, tindex fields of pthrea dnode.
        pthread_nodes[i].conn   = (void*)(pconn_inst->new_conn((void*)&pthread_nodes[i]));
        pthread_nodes[i].tIndex = i;
        pthread_nodes[i].pconn_class = pconn_inst;

        //Pthread_create.
        ret = pthread_create(&(pthread_nodes[i].tid), &attr, thread_work, &pthread_nodes[i]);
        if (0 != ret)
        {
            cout << "Pthread_create failed, ret: " << ret << " ,m_thread_num: " << i << endl;
        }
    }

    //Delete the thread propertyand wait for other threads.
    pthread_attr_destroy(&attr);
    for (int i = 0; i < m_thread_num; i++)
    {
        void* status;
        ret = pthread_join(pthread_nodes[i].tid, &status);
        if (0 != ret)
        {
            cout << "Error join thread, ret: " << ret << " status:  " << status << endl;
        }
    }

    return ret;
}

template<typename T>
void Download_thread<T>::fini()
{
    if (NULL != m_pthread_nodes)
    {
        delete[]m_pthread_nodes;
        m_pthread_nodes = NULL;
    }

    return;
}
