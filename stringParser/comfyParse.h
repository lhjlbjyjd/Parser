#ifndef COMFYPARSE_H_INCLUDED
#define COMFYPARSE_H_INCLUDED

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <utility>

#include "stringParse.h"
#include "curlParse.h"
#include "Serializer.h"

using namespace std;

class comfyParse{

private:
    vector<string> brandsList;
    curlParse *o;

public:

    string url;
    long long int srtPos = 0;

    comfyParse(string href){
        o = new curlParse();
        url = href;
        sPageParse(href);
    }

    ~comfyParse(){
        delete o;
    }

// begin page parse - функция для получения ссылок с начальной страницы
    void bPageParse(string &href){

        o->setUrl(href);
        string *page;
        page = o ->getStringFromDoc();



        if(page -> length()) cout<<"Start page load success "<<href<<"\n";

        stringParse *k = new stringParse(page);

        vector < pair< long long int, long long int> >
                t = k -> srhSpcTag("a","class=menu-categories-list",srtPos);

// experimental speed-up
        srtPos = t[t.size()-1].second;



        for(int i=0; i<t.size(); i++){

            long long int begPos = t[i].first, endPos = t[i].second;

            string href = k -> attrSearch("href",begPos,endPos);

            hrefVld(href);



            sPageParse(href);

        }

        delete k;


    }

// second page parse - функция для получения количества моделей
    void sPageParse(string &href){
        o->setUrl(href);
        string *page;
        page =  o ->getStringFromDoc();


        if(page -> length()) cout<<"Products page load success "<<href<<"\n";
// получаем строчку, в которой содержится количество моделей
// и обрабатываем число
        vector <string> mdl;

        stringParse *strPrs = new stringParse(page);
        strPrs -> getSpcBody("span","id=productsCount", &mdl, srtPos);
        delete strPrs;

        tidyUp(&mdl);

// возможная ошибка
        if(!mdl.size()) return;

        string s = mdl[0];

        long long int mdlNum = 0;

        for(int i=0; i<s.length(); i++)
        {
            if(s[i]==' ') break;
            mdlNum *=10;

            mdlNum += (long long int) (s[i]-'0');

        }

// теперь будем скакать по страницам
        long long int cnt = 0;

        string hrefObj = href + "?p=";

        for(long long int i=1; cnt<mdlNum; i++)
        {
            string k;
            long long int u = i;
            while(u>0){
                int j = u % 10;
                u/=10;
                k+= (char) (j+'0');
            }
            string kRev = hrefObj;
            for(int j = k.length()-1; j>-1; j--) kRev += k[j];

            objPageParse(kRev, cnt);

        }

    }

// object page parse - функция для получения данных со страницы товара

    void objPageParse(string &href, long long int &cnt)
    {
        long long int startTime = time(nullptr);
        o->setUrl(href);
        string *page;
        page =  o ->getStringFromDoc();

        if(page -> length()) cout << "Object page load success "<< href<<"\n";

        stringParse *k = new stringParse(page);

        vector < pair< long long int, long long int> >
                t = k -> srhSpcTag("a","class=product-item__img",srtPos);

        for(int i=0; i<t.size(); i++){

            //long long int startTime = time(nullptr);

            long long int begPos = t[i].first, endPos = t[i].second;

            cnt++;

            string hrefFinal = k -> attrSearch("href",begPos,endPos);
            string title = k -> attrSearch("title",begPos,endPos);
            hrefVld(hrefFinal);

            vector <string> ftr;
            fPageParse(hrefFinal, &ftr);


            ofstream out;

            string path ="./results/" + title +".txt";
            char *c = &(path[0]);
            out.open(c);

            for(int i=0; i<ftr.size(); i++)
                out<<ftr[i]<<"\n";

            out.close();

        }

        cout << "Time elapsed for object: " << time(nullptr)-startTime << " sec." << endl;

        delete k;

    }




// final page parse - функция для получения данных со страницы товара

    void fPageParse(string &href,vector <string> *ftr){
// feature vector - содержит заголовок и особенности товара
        cout << href << endl;
        o->setUrl(href);
        string *page;
        page = o ->getStringFromDoc();

        vector <string> prs;

        stringParse *k = new stringParse(page);


        if(page -> length()){
            cout<<"Final page load success: ";
            vector<long long int> lol = k->doKmp("<title");
            string title = k->getTagBody(lol[0], "title");
            title.erase (title.end()-201, title.end());
            cout << title << endl;
        }

        k -> getSpcBody("dl","class=features-item__list", &prs, srtPos);
        delete k;

        for(int i=0; i<prs.size(); i++){
            stringParse *strPrs = new stringParse(&(prs[i]));

            strPrs -> getSpcBody("dt","class=title",ftr);

            strPrs -> getSpcBody("dd","class=value",ftr);

            delete strPrs;
        }
        tidyUp(ftr);

        //ДОБАВЛЕНО КРИВОРУКИМ ДОЛБОЁБОМ
        //serialiseContent(ftr);
    }

    void serialiseContent(vector <string> *ftr){
        Serializer *serializer = new Serializer();
        bool pair = true;
        for(unsigned long i=0; i<ftr -> size(); i++)
        {
            if(pair) {
                string s;
                s = serializer->serialize((*ftr)[i]);
                (*ftr)[i] = s;
            }else{
                string s;
                s = serializer->serializeValues((*ftr)[i-1], (*ftr)[i]);
                (*ftr)[i] = s;
            }
            pair = !pair;
        }
        delete serializer;
    }

    void tidyUp(vector <string> *ftr){

        for(int i=0; i<ftr -> size(); i++)
        {
            string s;

            long long int d = (*ftr)[i].length();
            for(int j = 0; j<d; j++)
            {
                char c = (*ftr)[i][j];
                if(c=='<') break;
                if(c=='\n' || c=='\r') continue;
                if(c!=' ') s+=c;
                else if(j!=0 && (*ftr)[i][j-1]!=' ' && (*ftr)[i][j-1]!='\n') s+=c;

            }
            (*ftr)[i] = s;
        }

    }

    void getBrands(string* page){
        stringParse *k = new stringParse(page);
        //k -> getSpcBody("dl","class=features-item__list", &prs, srtPos);
    }


// href validation - если ссылка не абсолютна, делает её таковой.
    void hrefVld(string &href){
        if(href[0]=='/') href = url+href;
    }

};


#endif // COMFYPARSE_H_INCLUDED