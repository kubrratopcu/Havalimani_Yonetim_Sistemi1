#include <iostream>
#include <queue>
#include <unordered_map>
#include <map>
#include "Modeller.h"

using namespace std;

// Fonksiyon Bildirimleri (Forward Declarations),
// KuleYonetimi.cpp'den
void ucaklariYukle(priority_queue<Ucak>& kule);
void rotalariYukle(map<string, vector<pair<string, int>>>& graf);

// YolcuSistemi.cpp'den
struct YolcuNode; // Sadece tipini belirtiyoruz
void yolculariYukle(unordered_map<string, Yolcu>& harita, YolcuNode*& kok);
void yolculariListele(YolcuNode* kok);

// KargoSistemi.cpp'den
void seferleriYukle(map<string, Sefer>& seferSistemi);
void bagajlariYukle(map<string, Sefer>& seferSistemi);

int main() {
    cout << "--- HAVALIMANI YONETIM SISTEMI BASLATILIYOR ---" << endl;

    // 1. Veri Yapılarını Tanımlıyoruz
    priority_queue<Ucak> kuleKuyrugu;
    map<string, vector<pair<string, int>>> ucusGrafi;
    unordered_map<string, Yolcu> yolcuHaritasi;
    YolcuNode* bstKoku = nullptr;
    map<string, Sefer> seferler;

    // 2. Fonksiyonları Çağırıp Dosyaları Okuyoruz
    ucaklariYukle(kuleKuyrugu);
    rotalariYukle(ucusGrafi);
    yolculariYukle(yolcuHaritasi, bstKoku);
    seferleriYukle(seferler);
    bagajlariYukle(seferler);

    cout << "\n--- TEST ASAMASI ---" << endl;

    // Kübra'nın Testi: En öncelikli uçak hangisi?
    if (!kuleKuyrugu.empty()) {
        Ucak ilkInen = kuleKuyrugu.top();
        cout << "[!] Kule: Inis izni verilen ilk ucak ID: " << ilkInen.id
             << " Havayolu: " << ilkInen.havayolu
             << " (Yakit: " << ilkInen.yakit << ")" << endl;
    }

    // Beyza'nın Testi: Yolcular alfabetik sıralı mı?
    cout << "[!] Yolcu Listesi (Alfabetik): " << endl;
    yolculariListele(bstKoku);

    cout << "\nSistem 2. Gun entegrasyonunu basariyla tamamladi!" << endl;

    return 0;
}