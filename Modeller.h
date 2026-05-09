#pragma once

#include <string>
#include <vector>
#include <stack>

using namespace std;

// 1. UÇAK YAPISI
// 1. UÇAK YAPISI
struct Ucak {
    int id;
    string havayolu;
    int yakit;
    long long varisZamani;
    bool acilDurum; // --- YENİ EKLENEN DEĞİŞKEN ---

    // Priority Queue (Öncelikli Kuyruk) için karşılaştırma operatörü
    bool operator<(const Ucak& diger) const {

        // 1. KURAL: Acil durum varsa her şeyi boşver, o uçağı en öne al!
        if (this->acilDurum != diger.acilDurum) {
            return diger.acilDurum; // Eğer 'diger' uçak acil durumdaysa o üste çıkar.
        }

        // 2. KURAL: İkisinin de durumu aynıysa (ikisi de normal veya acilse), yakıtı AZ olana öncelik ver
        if (this->yakit != diger.yakit) {
            return this->yakit > diger.yakit;
        }

        // 3. KURAL: Yakıtlar da eşitse, varış zamanı erken olanı öne al
        return this->varisZamani > diger.varisZamani;
    }
};

// 2. YOLCU YAPISI
struct Yolcu {
    string ad;
    string soyad;
    string pnr;
    string koltukNo;
};
// İkili Arama Ağacı (BST) Düğüm Yapısı(bellek temızlıgı ıcın)
struct YolcuNode {
    Yolcu veri;
    YolcuNode *sol;
    YolcuNode *sag;
};
// 3. BAGAJ YAPISI (Stack İçin)
struct Bagaj {
    int id;
    string pnr_sahibi;
    float agirlik; // --- İŞTE BU SATIRI EKLEDİK ---
};



// 4. LINKED LIST (Bağlı Liste) DÜĞÜM YAPISI
struct PnrNode {
    string pnr;
    struct PnrNode *next;
};

// 5. SEFER YAPISI
struct Sefer {
    string seferNo;
    int ucakId;
    long long ucusZamani;
    bool koltukDurumu[20][6] = {false};
    vector<string> yolcuPnrListesi;
    stack<Bagaj> kargoBolumu;

    // Linked List Başlangıç Noktası
    PnrNode* yolcuListesiBasi = nullptr;
};