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

#include "KuleYonetimi.h"
#include "Modeller.h"

using namespace std;

class HavayoluSistemi {
private:
    // --- VERİ YAPILARI ---
    map<string, Sefer> seferSistemi;
    KuleYonetimi kuleYonetimi;                   // ÖNEMLİ: CPP'deki hata buradan geliyordu, ekledik.
    map<string, vector<pair<string, int>>> graf;
    unordered_map<string, Yolcu> harita;
    YolcuNode* yolcuAgaciKoku;

public:
    // --- CONSTRUCTOR & DESTRUCTOR ---
    HavayoluSistemi();
    ~HavayoluSistemi();

    // --- TEMEL FONKSİYONLAR ---
    void yolculariYukle();
    void seferleriYukle();
    void bagajlariYukle();
    void rotalariYukle();

    // --- ALGORİTMALAR VE İŞLEMLER ---
    std::string siradakiUcagiIndir();
    string enKisaRota(string kalkis, string varis);
    Yolcu pnrIleYolcuBul(string pnr);
    string yolcununUcusBilgisiniGetir(string pnr);
    vector<string> bagajlariTahliyeEt(string seferNo);

    // --- GETTER ---
    // Sadece bir tane getGraf bıraktım, hata veren buydu:
    std::map<std::string, std::vector<std::pair<std::string, int>>>& getGraf() { return graf; }

    // --- BELLEK YÖNETİMİ ---
    void agaciTemizle(YolcuNode* kok);
    void seferListesiniTemizle(PnrNode*& bas);
};

#endif // HAVAYOLUSISTEMI_H