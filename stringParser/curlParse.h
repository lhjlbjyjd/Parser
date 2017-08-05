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

private:
    CURL *curl;
    CURLcode res;
    string readBuffer = "";


    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

public:

    curlParse(){
        curl = curl_easy_init();
    }
    ~curlParse(){
        curl_easy_cleanup(curl);
    }

    void setUrl(string url){
        this -> url = url;
    }


    string* getStringFromDoc()
    {
        if(curl) {
            readBuffer = "";
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
        }
        return &readBuffer;
    }

};

#endif // CURLPARSE_H_INCLUDED
