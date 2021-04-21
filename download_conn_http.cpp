#include <iostream>
#include <curl/curl.h>
#include <curl/easy.h>
#include "download_utils.h"
#include "download_conn.h"
#include "download_file.h"
#include "download_conn_http.h"

using namespace std;

int Download_conn_http::global_init(void)
{
    (void)curl_global_init(CURL_GLOBAL_ALL);

    return ERROR_TYPE_SUCCESS;
}

int Download_conn_http::init(const std::string& download_url)
{
    m_url = download_url;
    global_init();

    return ERROR_TYPE_SUCCESS;
}

void Download_conn_http::fini()
{
    (void)curl_global_cleanup();

    return;
}

long Download_conn_http::get_content_length()
{
    double filelength = INVALID_FILELENGTH;
    CURL* curl = NULL;

    curl = curl_easy_init();
    if (NULL == curl)
    {
        return filelength;
    }
    cout << "Url: " << m_url << endl;

    //Need header, no body.
    curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);

    CURLcode res = curl_easy_perform(curl);
    if (CURLE_OK == res)
    {
        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &filelength);
    }

    curl_easy_cleanup(curl);

    return filelength;
}

//For new conn, CURLOPT_WRITEFUNCTION.
size_t Download_conn_http::conn_write_fun(void* ptr, size_t size, size_t nmemb, void* pdata)
{
    size_t written = 0;

    written = Download_file<Download_conn_http>::get_instance()->file_write(ptr, (int)size, (int)nmemb, pdata);

    return written;
}

//New conn.
void* Download_conn_http::new_conn(void* user_data)
{
    CURL* curl = curl_easy_init();

    //Curl set.
    curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, conn_write_fun);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, user_data);

    //Does not set timeout for DNS resolution.
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    //Detail.
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    //When the transmission speed is less than 1 byte / s for 5 seconds, the connection is terminated!
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 10L);

    return curl;
}

//For thread work.
int Download_conn_http::conn_perform(void* conn, const std::string& range)
{
    CURL* curl = (CURL*)conn;
    CURLcode res;

    //Update range.
    res = curl_easy_setopt(curl, CURLOPT_RANGE, range.c_str());
    res = curl_easy_perform(curl);
    if (CURLE_OK != res)
    {
        cout << "Conn perform failed: " << res << "," << curl_easy_strerror(res) << endl;
    }

    curl_easy_cleanup(curl);

    return ERROR_TYPE_SUCCESS;
}
