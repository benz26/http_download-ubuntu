#ifndef _DOWNLOAD_MD5_
#define _DOWNLOAD_MD5_

#define MD5_BUFF_LEN 1024 * 16

//Download_md5.
class Download_md5 {
public:
    static Download_md5* get_instance()
    {
        static Download_md5 instance;
        return &instance;
    }

    //External call.
    int md5_get(const std::string& filepath);

    //Internal call.
    int get_file_md5(const std::string& filename, std::string& md5_value);

private:
    Download_md5()
    {
    };

    ~Download_md5()
    {
    };
};

#endif