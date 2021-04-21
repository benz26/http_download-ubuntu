#include <iostream>
#include <pthread.h>
#include "download_utils.h"
#include "download_file.h"
#include "download_conn.h"
#include "download_conn_http.h"
#include "download_md5.h"
#include "download_thread.h"

using namespace std;

//Main.
int main(void)
{
    int    ret           = ERROR_TYPE_FAILED;
    string download_url  = "";
    string file_path     = "";
    int    thread_num    = 0;
    Download_conn_http * pconn_inst = NULL;

    //Input parameter.
    thread_num   = 1;
    download_url = "http://gdown.baidu.com/data/wisegame/cf3a36db382df851/446ccf3a36db382df851e28a20c86241.apk";

    //"446ccf3a36db382df851e28a20c86241.apk".
    file_path = download_url.substr(download_url.find_last_of("/") + 1, 100);

    do {
        //Init.
        pconn_inst = Download_conn_http::get_instance();
        (void)pconn_inst->init(download_url);

        ret = Download_file<Download_conn_http>::get_instance()->init(file_path, pconn_inst);
        if (0 > ret)
        {
            cout << "File module init failed, ret: " << ret << endl;
            break;
        }

        Download_thread<Download_conn_http>::get_instance()->init(thread_num);

        //Download.
        cout << "Download start..." << endl;

        ret = Download_thread<Download_conn_http>::get_instance()->thread_handle(pconn_inst);
        if (0 > ret)
        {
            cout << "Download handle, ret: " << ret << endl;
        }
    }while(0);

    //Fini.
    Download_thread<Download_conn_http>::get_instance()->fini();
    pconn_inst->fini();
    Download_file<Download_conn_http>::get_instance()->fini();


    //Md5.
    ret = Download_md5::get_instance()->md5_get(file_path);
    if (0 <= ret)
    {
        cout << "Download succed!" << endl;
    }

    pthread_exit(0);

    return ERROR_TYPE_SUCCESS;
}
