



#include <iostream>
#include <ctime>
#include <fstream>
#include <cstring>



#include "stringParser/comfyParse.h"

using namespace std;


int main() {
string url = "https://comfy.ua/notebook/";
comfyParse *co = new comfyParse(url);

delete co;

}




