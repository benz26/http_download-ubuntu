#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "download_utils.h"
#include "download_file.h"
#include "download_conn.h"
#include "download_thread.h"

using namespace std;

template<typename T>
int Download_file<T>::get_filelength_conn(T* pconn)
{
    return pconn->get_content_length();
}

template<typename T>
int Download_file<T>::get_filelength(void)
{
    return m_filelength;
}

template<typename T>
int Download_file<T>::init(const std::string& file_path, T* pconn)
{
    int  ret        = ERROR_TYPE_FAILED;
    long filelength = INVALID_FILELENGTH;

    //Get filelength.
    filelength = get_filelength_conn(pconn);
    if (0 >= filelength)
    {
        cout << "The file is empty! filelength: " << filelength << "." << endl;
        return ret;
    }

    //Open file.
    int fd = open(file_path.c_str(), O_RDWR|O_CREAT|O_TRUNC, S_IWUSR|S_IWRITE|S_IREAD);
    if (0 > fd)
    {
        cout << "Open file failed, file path:" << file_path << "!" << endl;
        return ret;
    }

    //Set.
    m_filelength = filelength;
    m_fd         = fd;

    return ERROR_TYPE_SUCCESS;
}

//Called by conn callback.
template<typename T>
int Download_file<T>::file_write(void* ptr, int size, int nmemb, void* pdata)
{
    thread_node* node  = (thread_node*)pdata;
    int    fd          = m_fd;
    int    byte_counts = 0;
    int    offset      = 0;

    cout << "Start to write size*nmemb: "
         << size  << "*" << nmemb << " bytes"
         << ", start: "  << node->start
         << ", end: "    << node->end
         << endl;

    if (node->start > node->end)
    {
        //Download completed，do nothing.
        return 0;
    }

    if (node->start + size * nmemb <= node->end + 1)
    {
        //Write from start.
        byte_counts = size * nmemb;
    }
    else
    {
        //Received more than remaining fields.
        byte_counts = node->end - node->start + 1;
    }
    offset      =  node->start;

    //Update start index.
    node->start += byte_counts;

    int ret = pwrite(fd, ptr, byte_counts, offset);
    if (ERROR_TYPE_FAILED == ret)
    {
        cout << "Pwrite error!" << endl;
        exit(-1);
    }

    //Download progress.
    m_downloaded +=  ret;
    m_percent = (int)(100 * ((1.0)*m_downloaded) / ((1.0)*m_filelength));

    //If change 'int' to 'size_t', 'size_t' printing error, but 'size_t' works normally.
    cout << "Download progress:" << m_percent << "%, " << m_downloaded << "/" << m_filelength << endl << endl;

    return ret;
}

template<typename T>
int Download_file<T>::fini()
{
    int fd = m_fd;

    if (0 < fd)
    {
        close(fd);
    }

    return ERROR_TYPE_SUCCESS;
}
