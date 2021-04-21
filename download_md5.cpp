#include <iostream>
#include <fstream>
#include <openssl/md5.h>
#include <cstring>
#include "download_utils.h"
#include "download_md5.h"

using namespace std;

int Download_md5::get_file_md5(const string& filename, string& md5_value)
{
    md5_value.clear();

    std::ifstream file(filename.c_str(), std::ifstream::binary);
    if (!file)
    {
        cout << "Creat file stream failed! file name: " << filename.c_str() << endl;
        return ERROR_TYPE_FAILED;
    }

    MD5_CTX md5_context;
    MD5_Init(&md5_context);

    char buf[MD5_BUFF_LEN];
    while (file.good()) {
        file.read(buf, sizeof(buf));
        MD5_Update(&md5_context, buf, file.gcount());
    }

    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(result, &md5_context);

    char hex[35];
    memset(hex, 0, sizeof(hex));
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        sprintf(hex + i * 2, "%02x", result[i]);
    }
    hex[32] = '\0';
    md5_value.assign(hex, 32);

    return ERROR_TYPE_SUCCESS;
}

int Download_md5::md5_get(const string& filepath)
{
    string md5_value;

    int ret = get_file_md5(filepath, md5_value);
    if (0 > ret)
    {
        cout << "Get file md5 failed! file name: " << filepath << endl;
        return ERROR_TYPE_FAILED;
    }

    cout << "The md5 value       = " << md5_value << endl;
    cout << "The corret MD5 value= 446ccf3a36db382df851e28a20c86241" << endl;

    return ERROR_TYPE_SUCCESS;
}
