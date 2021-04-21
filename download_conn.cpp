#include <iostream>
#include "download_utils.h"
#include "download_conn.h"

using namespace std;

//Init.
int Download_conn::init(const std::string& download_url)
{
    m_url = download_url;
    return ERROR_TYPE_SUCCESS;
}

//Fini.
void Download_conn::fini()
{
    return;
}

//Get filelength.
long Download_conn::get_content_length()
{
    long filelength = INVALID_FILELENGTH;

    return filelength;
}

//New conn.
void* Download_conn::new_conn(void* user_data)
{
    void* conn = NULL;
    return conn;
}

//For thread work.
int Download_conn::conn_perform(void* conn, const std::string& range)
{
    return ERROR_TYPE_SUCCESS;
}
