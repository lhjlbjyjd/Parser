#ifndef CURLPARSE_H_INCLUDED
#define CURLPARSE_H_INCLUDED

#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <curl/curl.h>

using namespace std;

class curlParse{

    string url;

public:
    curlParse(string url)
    {
        this -> url = url;
    }

    void setUrl(string url){
        this -> url = url;
    }


    string* getStringFromDoc()
    {
        CURL *curl;
        CURLcode res;

        if(curl) {

            curl = curl_easy_init();
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        return &readBuffer;



    }

private:
    string readBuffer = "";


    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

};

#endif // CURLPARSE_H_INCLUDED
