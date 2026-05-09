#ifndef HAVAYOLUSISTEMI_H
#define HAVAYOLUSISTEMI_H

#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <unordered_map>
#include <stack>
#include <string>
#include <fstream>
#include <sstream>
#include "Modeller.h"

using namespace std;

class HavayoluSistemi {
private:
    // Sınıfın kendi özel veri yapıları
    map<string, Sefer> seferSistemi;
    priority_queue<Ucak> kule;
    map<string, vector<pair<string, int>>> graf;
    unordered_map<string, Yolcu> harita;
    YolcuNode* yolcuAgaciKoku;

public:
    HavayoluSistemi();
    ~HavayoluSistemi();

    // Temel yükleme ve işlem fonksiyonları
    void seferleriYukle();
    void bagajlariYukle();
    void bagajlariTahliyeEt(string seferNo);

    // Bellek temizliği için yardımcılar
    void agaciTemizle(YolcuNode* kok);
    void seferListesiniTemizle(PnrNode*& bas);
}; // <--- BU NOKTALI VİRGÜLÜ SAKIN SİLME, HATALARIN KAYNAĞI BU OLABİLİR!

#endif