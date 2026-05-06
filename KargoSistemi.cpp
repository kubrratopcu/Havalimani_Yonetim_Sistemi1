#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "Modeller.h"

using namespace std;

// Önce Seferleri sisteme yüklüyoruz
void seferleriYukle(map<string, Sefer>& seferSistemi) {
    ifstream dosya("seferler.txt");
    string satir;
    if (!dosya.is_open()) return;

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string seferNo, ucakId, kalkis, varis, zaman;
        getline(ss, seferNo, ',');
        getline(ss, ucakId, ',');
        getline(ss, kalkis, ',');
        getline(ss, varis, ',');
        getline(ss, zaman, ',');

        Sefer s;
        s.seferNo = seferNo;
        s.ucakId = stoi(ucakId);
        s.ucusZamani = stoll(zaman);
        // Örnek: Manuel yolcu eklersek diye PNR111 bu seferde varsayalım
        s.yolcuPnrListesi.push_back("PNR111"); 
        
        seferSistemi[seferNo] = s;
    }
    dosya.close();
    cout << "[-] Sefer Sistemi: Seferler yuklendi." << endl;
}

// Bagajları okuyup ilgili seferin Stack ine atıyoruz
void bagajlariYukle(map<string, Sefer>& seferSistemi) {
    ifstream dosya("bagajlar.txt");
    string satir;
    if (!dosya.is_open()) return;

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string bId, pnr, agirlik;
        getline(ss, bId, ',');
        getline(ss, pnr, ',');
        getline(ss, agirlik, ',');

        Bagaj b = {stoi(bId), pnr, stof(agirlik)};

        // Bagajın sahibini seferlerde ara ve o seferin Stack'ine ekle
        for (auto& cift : seferSistemi) {
            for (string yPnr : cift.second.yolcuPnrListesi) {
                if (yPnr == pnr) {
                    cift.second.kargoBolumu.push(b);
                }
            }
        }
    }
    dosya.close();
    cout << "[-] Kargo Sistemi: Bagajlar ucaklarin yiginina (Stack) yuklendi." << endl;
}