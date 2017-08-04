//
// Created by lhjlbjyjd on 01.08.17.
//

#ifndef SBORKA_SERIALISER_H
#define SBORKA_SERIALISER_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <map>

using namespace std;

class Serializer{
private:
    map<string, string> specsMap = {
            {"Диагональ экрана", "ScreenSize"},
            {"Разрешение экрана", "ScreenResolution"},
            {"Покрытие экрана", "ScreenCover"},
            {"Центральный процессор", "CPU"},
            {"Модель центрального процессора", "CPUModel"},
            {"Количество ядер", "CPUCores"},
            {"Частота центрального процессора", "CPUClock"},
            {"Объем ОЗУ", "RAM"},
            {"Тип оперативной памяти", "RAMType"},
            {"Частота оперативной памяти", "RAMClock"},
            {"Объем накопителя", "Memory"},
            {"Тип накопителя", "MemoryType"},
            {"Производитель видеокарты", "GPUManufacturer"},
            {"Модель графического процессора", "GPUModel"},
            {"Объем видео памяти", "GRAMSize"},
            {"Тип видеокарты", "GPUType"},
            {"Операционная система", "OS"},
            {"Разрешение веб-камеры", "CamResolution"},
            {"Оптический привод", "DiscReader"},
            {"Тип беспроводной сети Wi-Fi", "WifyType"},
            {"Bluetooth", "BluetoothVersion"},
            {"HDMI", "HDMI"},
            {"Количество USB 2.0", "Usb2Count"},
            {"Количество USB 3.0", "Usb3Count"},
            {"Кардридер", "Cardreader"},
            {"Время автономной работы", "WorkTime"},
            {"Материал корпуса", "Material"},
            {"Цвет", "Colour"},
            {"Вес", "Weigth"},
            {"Гарантийный срок", "Guarantee"},
            {"Страна производства", "MadeInCountry"},
            {"Бренд", "Manufacturer"},
            {"Модель графического процессора", "GPUModel"}
    };
public:
    string serialize(string s){
        for (auto it = specsMap.begin(); it != specsMap.end(); ++it)
        {
            if(it->first.compare(s) == 0){
                return it->second;
            }
        }
        return "Undefined";
    }

    string serializeValues(string key, string value){
        if(key.compare("ScreenSize") == 0)
            return removeBadSymbols(key, " '");
        if(key.compare("ScreenResolution") == 0)
            return removeBadSymbols(key, " FullHD");
        if(key.compare("CPUCores") == 0)
            return removeBadSymbols(key, " ядероа");
        if(key.compare("CPUClock") == 0)
            return removeBadSymbols(key, "Гц");
        if(key.compare("RAM") == 0)
            return removeBadSymbols(key, " ГБ");
        if(key.compare("RAMClock") == 0)
            return removeBadSymbols(key, "  МГц");
        if(key.compare("Memory") == 0)
            return removeBadSymbols(key, " ГБ");
        if(key.compare("BluetoothVersion") == 0)
            return removeBadSymbols(key, "Версия ");
        if(key.compare("Usb2Count") == 0)
            return removeBadSymbols(key, true);
        if(key.compare("Usb3Count") == 0)
            return removeBadSymbols(key, true);
        if(key.compare("Guarantee") == 0)
            return removeBadSymbols(key, true);

    }

    string removeBadSymbols(string s, string badSymbols){
        string result = "";
        for(unsigned long i = 0; i < s.length(); i++){
            bool isBad = false;
            for(int j = 0; j < badSymbols.length(); j++){
                if(s[i] == badSymbols[j]){
                    isBad = true;
                }
            }
            if(!isBad)
                s.push_back(s[i]);
        }
        return result;
    }

    string removeBadSymbols(string s, bool numbersOnly){
        string result = "";
        for(unsigned long i = 0; i < s.length(); i++){
            if(numbersOnly) {
                if (isdigit(s[i])) {
                    s.push_back(s[i]);
                }
            }
        }
        return result;
    }
};

#endif //SBORKA_SERIALISER_H
