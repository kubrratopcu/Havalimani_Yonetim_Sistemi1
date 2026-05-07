#pragma once

#include <string>
#include <vector>
#include <stack>

using namespace std;

// 1. UÇAK YAPISI
struct Ucak {
    int id;
    string havayolu;
    int yakit;
    long long varisZamani;

    // Priority Queue (Öncelikli Kuyruk) için karşılaştırma operatörü
    bool operator<(const Ucak& diger) const {
        if (yakit == diger.yakit) {
            return varisZamani > diger.varisZamani;
        }
        return yakit > diger.yakit;
    }
};

// 2. YOLCU YAPISI
struct Yolcu {
    string ad;
    string soyad;
    string pnr;
    string koltukNo;
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
    struct PnrNode* next;
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