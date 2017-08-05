#ifndef STRINGPARSE_H_INCLUDED
#define STRINGPARSE_H_INCLUDED

#include <iostream>
#include <cstring>
#include <vector>
#include <utility>

using namespace std;

class stringParse{

public:
    string *html;


    stringParse(string *html){
        this->html = html;
    }



    // ищет первую строку во основной и возвращает вектор long long int, в котором записаны
    // позиции вхождения первой строки во вторую, кр4 алгоритм КМП
    vector <long long int> doKmp(string a)
    {
// сливаем две строки для алгоритма кмп
        string s = a + "@" + *html;

        long long int *m, l = s.length(), p = a.length();
        vector <long long int> srtPositions;
        m = new long long int [l];
        for(long long int i=0; i<=p; i++) m[i]=0;

        for(long long int i = p+1; i < l ; i++){
            long long int j = m[i-1];
            while(j>0 && s[i]!=s[j]) j  = m[j];
            if(s[i]==s[j]) j++;
            m[i]=j;
            if(j==p) srtPositions.push_back(i-2*p);


        }
        delete m;
        return srtPositions;
    }




// search for tags with specified attribute - возвращает массив начальных
// и конечных позиций тэгов, подходящих условию
//( конечная позиция - это позиция закрывающей
// скобки '>', а не позиция завершающего тэга "</tag>"
// Чтобы получить тело между открывающим и закрывающим тэгом
// используется функции: getTagBody, getSpcBody

// третий параметр добавляет возможность начинать не с начала документа,
// а с указанной позиции

    vector <pair <long long int, long long int> > srhSpcTag(string tag, string attr, long long int srtPos)
    {
// сливаем две строки для алгоритма кмп

        vector <pair <long long int, long long int> > positions;

        if(!(html -> length()))
        {
            cout<<"Sorry, empty string, function terminates" << endl;
            return positions;
        }


        string s = "<" + tag + "@";

        char *c = &((*html)[srtPos]);

        long long int *m, d = s.length(), l = html -> length() - srtPos + d, p = d-1;



        string tAttr, tValue;

        int pos=0, g = attr.length();

        for(int i=0; i<g; i++)
        {
            if(attr[i]=='='){
                pos=i+1;
                break;
            }
            else tAttr+=attr[i];
        }
        for(int i=pos; i<g; i++) tValue +=attr[i];

        m = new long long int [l];
        for(long long int i=0; i<d; i++) m[i]=0;

        for(long long int i = d; i < l ; i++){

            char k = c[i-d];
            s += k;

            long long int j = m[i-1];
            while(j>0 && k!=s[j]) j  = m[j];
            if(k==s[j]) j++;
            m[i]=j;

            if(j==p)
            {
                long long int begPos =srtPos + i - 2*p, endPos = getTagEnd(begPos);

                string fValue = attrSearch(tAttr,begPos,endPos);

                long long int flag = srhAttrPos(&tValue,&fValue);

                if(flag != -1) positions.push_back( make_pair(begPos,endPos) );

            }


        }
        delete m;
        return positions;
    }


    vector <pair <long long int, long long int> > srhSpcTag(string tag, string attr)
    {
        return srhSpcTag(tag, attr, 0);
    }





    // возвращает индекс вхождения аттрибута(одного !), при этом останавливается, встречая
    // конец тэга '>' в исходной строке
    long long int srhAttrPos(string *a, long long int begPos, long long int endPos){
        string s = *a + "@";

        char *c = &((*html)[begPos]);

        long long int *m,
                p = a -> length(),
                d = p+1,
                l = endPos - begPos + d;


        long long int srtPosition = -1;
        m = new long long int [l];

        for(long long int i=0; i<d; i++) m[i]=0;

        for(long long int i = d; i < l ; i++){

            char k = c[i-d];
            s+=k;

            if(k=='>') break;
            long long int j = m[i-1];
            while(j>0 && k!=s[j]) j  = m[j];
            if(k==s[j]) j++;
            m[i]=j;
            if(j==p){
                srtPosition = begPos + i-2*p;
                break;
            }
        }
        delete m;
        return srtPosition;
    }

    // возвращает индекс вхождения аттрибута(одного !), при этом останавливается, встречая
    // конец тэга '>' в строке, на которую указывает параметр-указатель на строку 'b'

    long long int srhAttrPos(string *a, string *b){

        string s = *a + "@" + *b;

        long long int *m, l = s.length(), p = a -> length();
        long long int srtPosition = -1;
        m = new long long int [l];
        for(long long int i=0; i<p+1; i++) m[i]=0;

        for(long long int i = p+1; i < l ; i++){
            if(s[i]=='>') break;
            long long int j = m[i-1];
            while(j>0 && s[i]!=s[j]) j  = m[j];
            if(s[i]==s[j]) j++;
            m[i]=j;
            if(j==p){
                srtPosition = i-2*p;
                break;
            }
        }
        delete m;
        return srtPosition;



    }





    // ищет  конец тага, затем копирует  само тело тага в строку и возвращает его
    // параметр 1 - позиция вхождения начала тага в основную строку
    // параметр 2 - сам таг без кавычек(!)
    string getTagBody(long long int tagPos, string tag)
    {
        string tagStr;
        string endTag = "</" + tag;
        string srtTag = "<" + tag;
        long long int b = html -> length(),
                c = endTag.length(),
                d = srtTag.length(),
                tagEndPos = b,
                depth = 0;
        ;
        int fl, ag;
        for(long long int i = tagPos;; i++){
            int fl = 1;
            for(int j = 0; j<c; j++)
                if((*html)[i+j]!=endTag[j])
                {
                    fl = 0;
                    break;
                }
            if(fl) depth--;
            if(fl && !depth)
            {
                tagEndPos = i;

                break;
            }
            else
            {
                int ag = 1;
                for(int j = 0; j<d; j++)
                    if((*html)[i+j]!=srtTag[j])
                    {
                        ag = 0;
                        break;
                    }
                if(ag) depth++;

            }
        }
        depth = 0;
        long long int srtTagEnd = tagPos;
        for(;;srtTagEnd++)
        {
            char sym = (*html)[srtTagEnd];
            if(sym=='<') depth++;
            else if(sym=='>') depth--;

            if(!depth) break;
        }

        for(long long int i = srtTagEnd+1; i<tagEndPos; i++) tagStr+=(*html)[i];
        return tagStr;
    }


    long long int getTagEnd(long long int tagPos)
    {

        char *c = &((*html)[tagPos]);

        long long int depth = 0;
        long long int i;

        for(i=0;;i++){

            if(c[i]=='<') depth++;
            else if(c[i]=='>') depth--;



            if(!depth) return i+tagPos;
        }




    }
    // ищет аттрибут
    string attrSearch(string attr, long long int begPos, long long int endPos){

        long long int srtPos = srhAttrPos(&attr,begPos,endPos);

        if(srtPos==-1) return "";

        string attrVal;
        long long int k = srtPos + attr.length()+2;
        char *c  = &((*html)[k]);
        for(long long  i = 0;;i++)
        {
            if(c[i]=='\"')  break;
            attrVal+=c[i];
        }

        return attrVal;
    }

    // ищет подходящие по требованиям тэги и заполняет массив строк
    // через указатель на него, переданный функции.

    void getSpcBody(string tag, string attr, vector<string> *ftr, long long int srtPos){

        vector < pair< long long int, long long int> >
                t = srhSpcTag(tag,attr,srtPos);

        for(int i=0; i<t.size(); i++)
            ftr -> push_back(getTagBody(t[i].first,tag));

    }


    void getSpcBody(string tag, string attr, vector<string> *ftr)
    {
        getSpcBody(tag,attr,ftr,0);
    }

    unsigned long getPageLength(){
        return html->length();
    }




    virtual ~stringParse(){
        //destructor
        //cout<<"dest string ";
    }
};
#endif // STRINGPARSE_H_INCLUDED